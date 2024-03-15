using System;
using System.Threading;

namespace NewFramework
{
    public class LockFreeQueue<T>
    {
        public class SingleLinkNode<U> where U : T
        {
            public SingleLinkNode<U> Next;
            public U Item;
        }

        private static bool CAS<T>(ref T location,T comparand,T newValue) where T : class
        {
            return comparand==Interlocked.CompareExchange(ref location, comparand, newValue);
        }

        private SingleLinkNode<T> head;
        private SingleLinkNode<T> tail;

        private int count;
        
        public int Count
        {
            get { return count; }
        }

        public bool IsEmpty
        {
            get { return count <= 0; }
        }
        
        public LockFreeQueue()
        {
            head = new SingleLinkNode<T>();
            tail = head;
            count = 0;
        }


        public void Enqueue(T item)
        {
            SingleLinkNode<T> newNode = new SingleLinkNode<T>();
            newNode.Item = item;

            SingleLinkNode<T> oldTail=null;
            SingleLinkNode<T> oldTailNext;
            
            bool newNodeAdded = false;

            while (!newNodeAdded)
            {
                oldTail = tail;
                oldTailNext = oldTail.Next;

                if (tail==oldTail)
                {
                    if (oldTailNext==null)
                    {
                        newNodeAdded = CAS(ref tail.Next, null, newNode);
                    }
                }

            }

            CAS(ref tail, oldTail, newNode);
            Interlocked.Increment(ref count);

        }
    }
}