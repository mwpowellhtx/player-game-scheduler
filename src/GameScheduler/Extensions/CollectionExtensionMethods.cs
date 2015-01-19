using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;

namespace GameScheduler.Extensions
{
    public static class CollectionExtensionMethods
    {
        /// <summary>
        /// Returns all combinations of a chosen amount of selected elements in the sequence.
        /// </summary>
        /// <typeparam name="T">The type of the elements of the input sequence.</typeparam>
        /// <param name="source">The source for this extension method.</param>
        /// <param name="select">The amount of elements to select for every combination.</param>
        /// <param name="repetition">True when repetition of elements is allowed.</param>
        /// <returns>All combinations of a chosen amount of selected elements in the sequence.</returns>
        /// <see cref="http://extensionmethod.net/csharp/ienumerable-t/combinations"/>
        public static IEnumerable<IEnumerable<T>> Combinations<T>(
            this IEnumerable<T> source, int select, bool repetition = false)
        {
            Contract.Requires(source != null);
            Contract.Requires(select >= 0);

            return select == 0
                ? new[] {new T[0]}
                : source.SelectMany((x, i) =>
                    source
                        .Skip(repetition ? i : i + 1)
                        .Combinations(select - 1, repetition)
                        .Select(c => new[] {x}.Concat(c)));
        }

        public static void Swap<T>(this T[] array, int i, int j)
        {
            var temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }

        public static IEnumerable<IEnumerable<T>> CombinationsIterative<T>(
            this IEnumerable<T> source, int k)
        {
            var s = source.ToArray();

            var bits = new BitArray(s.Length);

            var indexes = Enumerable.Range(0, s.Length);

            if (k == s.Length)
            {
                yield return s;
                yield break;
            }

            do
            {
                bits = bits.Increment();

                var selected = (from bool b in bits select b).ToArray();

                if (selected.All(x => x)) break;

                if (selected.Where(b => b).Count() != k) continue;

                var current = selected.Zip(indexes,
                    (b, i) => new {Bit = b, Index = i}).Where(x => x.Bit);

                yield return current.Select(x => s[x.Index]).ToArray();

            } while (true);
        }

        private static BitArray Increment(this BitArray array)
        {
            var result = new BitArray(array);

            for (var i = 0; i < result.Length && !(result[i] = !result[i++]);)
            {
                //'Do' nothing ?
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="values"></param>
        /// <param name="k"></param>
        /// <returns></returns>
        /// <see cref="http://en.wikipedia.org/wiki/Combination"/>
        public static long EstimatedCombinations<T>(this IEnumerable<T> values, long k)
        {
            long n = values.Count();
            return n.Factorial()/(k.Factorial()*(n - k).Factorial());
        }

        public static long Factorial(this long value)
        {
            var current = value;
            var result = 1L;
            while (--current > 1)
                result *= current + 1;
            return result;
        }
    }
}
