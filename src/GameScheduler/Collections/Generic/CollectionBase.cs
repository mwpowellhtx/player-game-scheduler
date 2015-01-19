using System;
using System.Collections;
using System.Collections.Generic;

namespace GameScheduler.Collections.Generic
{
    public class CollectionBase<T> : ICollection<T>
    {
        /// <summary>
        /// Collection backing field.
        /// </summary>
        private readonly ICollection<T> _collection;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="collection"></param>
        protected CollectionBase(ICollection<T> collection)
        {
            _collection = collection;
        }

        /// <summary>
        /// Collection functional helper method.
        /// </summary>
        /// <typeparam name="TResult"></typeparam>
        /// <param name="func"></param>
        /// <returns></returns>
        private TResult CollectionFunction<TResult>(Func<ICollection<T>, TResult> func)
        {
            return func(_collection);
        }

        /// <summary>
        /// Collection action helper method.
        /// </summary>
        /// <param name="action"></param>
        private void CollectionAction(Action<ICollection<T>> action)
        {
            action(_collection);
        }

        #region Collection Members

        public IEnumerator<T> GetEnumerator()
        {
            return CollectionFunction(x => x.GetEnumerator());
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public virtual void Add(T item)
        {
            CollectionAction(x => x.Add(item));
        }

        void ICollection<T>.Add(T item)
        {
            Add(item);
        }

        public virtual void Clear()
        {
            CollectionAction(x => x.Clear());
        }

        void ICollection<T>.Clear()
        {
            Clear();
        }

        public virtual bool Contains(T item)
        {
            return CollectionFunction(x => x.Contains(item));
        }

        public virtual void CopyTo(T[] array, int arrayIndex)
        {
            CollectionAction(x => x.CopyTo(array, arrayIndex));
        }

        public virtual bool Remove(T item)
        {
            return CollectionFunction(x => x.Remove(item));
        }

        public virtual int Count
        {
            get { return CollectionFunction(x => x.Count); }
        }

        public virtual bool IsReadOnly
        {
            get { return CollectionFunction(x => x.IsReadOnly); }
        }

        #endregion
    }
}
