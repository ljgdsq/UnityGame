// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Collections.Generic;
using UnityEngine;

namespace NewFramework.Event {
    /// <summary>
    ///  EventRouter 
    /// </summary>
    /// <remarks>
    /// 日期: 2025年8月22日
    /// </remarks>
    public class EventRouter : IEventRouter {
        private Dictionary<int, LinkedList<EventHandler>> dict = new();

        public IDisposable OnEvent(int id, Action action) {
            return Register(id, EventHandler.Create(action));
        }

        public IDisposable OnEvent<T1>(int id, Action<T1> action) {
            return Register(id, EventHandler.Create<T1>(action));
        }

        public IDisposable OnEvent<T1, T2>(int id, Action<T1, T2> action) {
            return Register(id, EventHandler.Create<T1, T2>(action));
        }

        public IDisposable OnEvent<T1, T2, T3>(int id, Action<T1, T2, T3> action) {
            return Register(id, EventHandler.Create<T1, T2, T3>(action));
        }

        public void SendEvent(int id) {
            Execute(id, handler => handler.Invoke());
        }

        public void SendEvent<T1>(int id, T1 arg1) {
            Execute(id, handler => handler.Invoke(arg1));
        }

        public void SendEvent<T1, T2>(int id, T1 arg1, T2 arg2) {
            Execute(id, handler => handler.Invoke(arg1, arg2));
        }

        public void SendEvent<T1, T2, T3>(int id, T1 arg1, T2 arg2, T3 arg3) {
            Execute(id, handler => handler.Invoke(arg1, arg2, arg3));
        }

        private IDisposable Register(int id, EventHandler handler) {
            if (!dict.TryGetValue(id, out var list)) {
                list = new LinkedList<EventHandler>();
                dict[id] = list;
            }

            handler.List = list;
            list.AddLast(handler);
            return handler;
        }

        private void Execute(int id, Action<EventHandler> action) {
            if (dict.TryGetValue(id, out var list)) {
                foreach (var handler in list) {
                    try {
                        action(handler);
                    }
                    catch (Exception e) {
                        Debug.LogError($"EventRouter: Error executing event {id} handler: {e.Message}");
                    }
                }
            }
        }

        private class EventHandler : IDisposable {
            private Delegate Handler;
            public LinkedList<EventHandler> List;

            public static EventHandler Create(Action action) {
                return new EventHandler { Handler = action };
            }

            public static EventHandler Create<T1>(Action<T1> action) {
                return new EventHandler { Handler = action };
            }

            public static EventHandler Create<T1, T2>(Action<T1, T2> action) {
                return new EventHandler { Handler = action };
            }

            public static EventHandler Create<T1, T2, T3>(Action<T1, T2, T3> action) {
                return new EventHandler { Handler = action };
            }

            public void Invoke() {
                if (Handler is Action action) action();
            }

            public void Invoke<T1>(T1 arg1) {
                if (Handler is Action<T1> action) action(arg1);
            }

            public void Invoke<T1, T2>(T1 arg1, T2 arg2) {
                if (Handler is Action<T1, T2> action) action(arg1, arg2);
            }

            public void Invoke<T1, T2, T3>(T1 arg1, T2 arg2, T3 arg3) {
                if (Handler is Action<T1, T2, T3> action) action(arg1, arg2, arg3);
            }

            public void Dispose() {
                if (List != null) {
                    List.Remove(this);
                    List = null;
                }

                Handler = null;
            }
        }
    }
}
