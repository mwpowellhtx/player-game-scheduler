using System;
using FluentNHibernate.Cfg;
using FluentNHibernate.Cfg.Db;
using NHibernate;

namespace GameScheduler.Data
{
    public static class DatabaseManager
    {
        private static readonly ISessionFactory SessionFactory;

        private static Repository GetRepository()
        {
            return new Repository(SessionFactory.OpenSession());
        }

        private static ISessionFactory BuildSessionFactory(string connectionString)
        {
            return Fluently.Configure()
                .Database(MsSqlConfiguration.MsSql2012.ConnectionString(connectionString))
                .Mappings(m => m.FluentMappings.AddFromAssemblyOf<Program>())
                .BuildSessionFactory();
        }

        static DatabaseManager()
        {
            const string cs = @"Server=(local);Initial Catalog=gamescheduler;Integrated Security=true";
            SessionFactory = BuildSessionFactory(cs);
        }

        public static void WithoutTransaction(Action<Repository> action)
        {
            using (var r = GetRepository())
            {
                action(r);
                r.Dispose();
            }
        }

        public static void Transaction(Action<IUnitOfWork, Repository> action)
        {
            using (var r = GetRepository())
            {
                using (var w = r.Work())
                {
                    action(w, r);
                    w.Dispose();
                }
                r.Dispose();
            }
        }
    }
}
