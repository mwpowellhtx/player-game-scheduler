using System;
using System.Data;
using NHibernate;

namespace GameScheduler.Data
{
    public interface IUnitOfWork : IDisposable
    {
        void Commit();
    }

    public class UnitOfWork : IUnitOfWork
    {
        private readonly ITransaction _transaction;

        internal UnitOfWork(ISession session,
            IsolationLevel isolationLevel = IsolationLevel.ReadCommitted)
        {
            _transaction = session.BeginTransaction(isolationLevel);
        }

        public void Commit()
        {
            if (_transaction == null || !_transaction.IsActive) return;
            try
            {
                _transaction.Commit();
            }
            catch (Exception)
            {
                //TODO: danger: swallowed exception
                _transaction.Rollback();
                throw;
            }
        }

        public void Dispose()
        {
            Commit();
        }
    }
}
