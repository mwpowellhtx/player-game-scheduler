using System;
using System.Collections.Generic;
using System.Linq;

namespace GameScheduler.Collections.Generic
{
    public class BidirectionalList<T> : CollectionBase<T>, IBidirectionalList<T>
    {
        /// <summary>
        /// List backing field.
        /// </summary>
        private readonly IList<T> _list;

        /// <summary>
        /// Item delegate that occurs after either adding or removing.
        /// Syntactically, there is no difference between this and
        /// <see cref="BeforeItemDelegate"/>, except for the timing, and
        /// convenience whether to specify after, before, or combinations.
        /// </summary>
        /// <param name="item"></param>
        /// <see cref="BeforeItemDelegate"/>
        public delegate void AfterItemDelegate(T item);

        /// <summary>
        /// Item delegate that occurs before either adding or removing.
        /// Syntactically, there is no difference between this and
        /// <see cref="AfterItemDelegate"/>, except for the timing, and
        /// convenience whether to specify after, before, or combinations.
        /// </summary>
        /// <param name="item"></param>
        /// <see cref="AfterItemDelegate"/>
        public delegate void BeforeItemDelegate(T item);

        /// <summary>
        /// 
        /// </summary>
        private readonly BeforeItemDelegate _beforeAdd;

        /// <summary>
        /// 
        /// </summary>
        private readonly AfterItemDelegate _afterAdd;

        /// <summary>
        /// 
        /// </summary>
        private readonly BeforeItemDelegate _beforeRemove;

        /// <summary>
        /// 
        /// </summary>
        private readonly AfterItemDelegate _afterRemove;

        /// <summary>
        /// Default Constructor
        /// </summary>
        /// <param name="afterAdd"></param>
        /// <param name="afterRemove"></param>
        /// <param name="beforeAdd"></param>
        /// <param name="beforeRemove"></param>
        public BidirectionalList(
            AfterItemDelegate afterAdd = null,
            AfterItemDelegate afterRemove = null,
            BeforeItemDelegate beforeAdd = null,
            BeforeItemDelegate beforeRemove = null)
            : this(new List<T>(), afterAdd, afterRemove, beforeAdd, beforeRemove)
        {
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="list"></param>
        /// <param name="afterAdd"></param>
        /// <param name="afterRemove"></param>
        /// <param name="beforeAdd"></param>
        /// <param name="beforeRemove"></param>
        internal BidirectionalList(IList<T> list,
            AfterItemDelegate afterAdd = null,
            AfterItemDelegate afterRemove = null,
            BeforeItemDelegate beforeAdd = null,
            BeforeItemDelegate beforeRemove = null)
            : base(list)
        {
            _list = list;
            _afterAdd = afterAdd ?? (x => { });
            _afterRemove = afterRemove ?? (x => { });
            _beforeAdd = beforeAdd ?? (x => { });
            _beforeRemove = beforeRemove ?? (x => { });
        }

        private void ListAction(Action<IList<T>> action)
        {
            action(_list);
        }

        private TResult ListFunction<TResult>(Func<IList<T>, TResult> func)
        {
            return func(_list);
        }

        #region Collection Members

        public override void Add(T item)
        {
            ListAction(x =>
            {
                _beforeAdd(item);
                x.Add(item);
                _afterAdd(item);
            });
        }

        public override void Clear()
        {
            ListAction(x =>
            {
                while (x.Any())
                {
                    var item = x.First();
                    _beforeRemove(item);
                    x.Remove(item);
                    _afterRemove(item);
                }
            });
        }

        public override bool Remove(T item)
        {
            return ListFunction(x =>
            {
                _beforeRemove(item);
                var result = x.Remove(item);
                _afterRemove(item);
                return result;
            });
        }

        #endregion

        #region List Members

        public int IndexOf(T item)
        {
            return ListFunction(x => x.IndexOf(item));
        }

        public void Insert(int index, T item)
        {
            ListAction(x =>
            {
                _beforeAdd(item);
                x.Insert(index, item);
                _afterAdd(item);
            });
        }

        public void RemoveAt(int index)
        {
            ListAction(x =>
            {
                var item = x[index];
                _beforeRemove(item);
                x.RemoveAt(index);
                _afterRemove(item);
            });
        }

        public T this[int index]
        {
            get { return ListFunction(x => x[index]); }
            set
            {
                ListAction(x =>
                {
                    var old = x[index];
                    _beforeRemove(old);
                    _beforeAdd(value);

                    x[index] = value;
                    _afterRemove(old);
                    _afterAdd(value);
                });
            }
        }

        #endregion
    }

    /// <summary>
    /// BidirectionalListExtensionMethods class.
    /// </summary>
    public static class BidirectionalListExtensionMethods
    {
        /// <summary>
        /// Returns the <see cref="BidirectionalList{T}"/> based on the given
        /// <see cref="IList{T}"/>.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="list"></param>
        /// <param name="afterAdd"></param>
        /// <param name="afterRemove"></param>
        /// <param name="beforeAdd"></param>
        /// <param name="beforeRemove"></param>
        /// <returns></returns>
        public static IBidirectionalList<T> ToBidirectionalList<T>(this IList<T> list,
            BidirectionalList<T>.AfterItemDelegate afterAdd = null,
            BidirectionalList<T>.AfterItemDelegate afterRemove = null,
            BidirectionalList<T>.BeforeItemDelegate beforeAdd = null,
            BidirectionalList<T>.BeforeItemDelegate beforeRemove = null)
        {
            return new BidirectionalList<T>(list,
                afterAdd, afterRemove, beforeAdd, beforeRemove);
        }
    }
}
