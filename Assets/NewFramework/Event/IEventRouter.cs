// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;

namespace NewFramework.Event {

    /// <summary>
    ///  IEventRouter 
    /// </summary>
    /// <remarks>
    /// 日期: 2025年8月22日
    /// </remarks>
    public interface IEventRouter {
        IDisposable OnEvent(int id, Action action);
        IDisposable OnEvent<T1>(int id, Action<T1> action);
        IDisposable OnEvent<T1,T2>(int id, Action<T1,T2> action);
        IDisposable OnEvent<T1,T2,T3>(int id, Action<T1,T2,T3> action);
        
        void SendEvent(int id);
        void SendEvent<T1>(int id, T1 arg1);
        void SendEvent<T1,T2>(int id, T1 arg1, T2 arg2);
        void SendEvent<T1,T2,T3>(int id, T1 arg1, T2 arg2, T3 arg3);
    }
}
