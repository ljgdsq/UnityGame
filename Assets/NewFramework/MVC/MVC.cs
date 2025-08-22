// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Collections.Generic;
using NewFramework.Event;

namespace NewFramework.MVC {
    /// <summary>
    ///  Model 
    /// </summary>
    /// <remarks>
    /// 日期: 2025年8月22日
    /// </remarks>
    public interface IModel {
    }

    public interface IView {
    }

    public abstract class GameView : IView {
        protected virtual void OnCreate() { }
        protected virtual void OnInit() { }
        
        protected virtual void OnShow() { }
        protected virtual void OnHide() { }
        protected virtual void OnDestroy() { }
        protected virtual void OnUpdate() { }
    }
    public abstract class GameView<C> : GameView where C : GameCtl {
        protected C ctrl;

        protected void OnNotify(int ev, Action action) {
            ctrl.OnEvent(ev, action);
        }
        
        protected void OnNotify<T>(int ev, Action<T> action) {
            ctrl.OnEvent<T>(ev, action);
        }
    }

    public class ModelProperties<T> {
        private T _value;
        private event Action<T> _onValueChanged;

        public ModelProperties(T initialValue = default) {
            _value = initialValue;
        }

        public T Value {
            get => _value;
            set {
                if (!EqualityComparer<T>.Default.Equals(_value, value)) {
                    _value = value;
                    _onValueChanged?.Invoke(_value);
                }
            }
        }

        public void Subscribe(Action<T> handler) => _onValueChanged += handler;
        public void Unsubscribe(Action<T> handler) => _onValueChanged -= handler;
    }

    public abstract class GameCtl {
        private IEventRouter router = new EventRouter();

        public void OnEvent(int eventName, Action action) {
            this.router.OnEvent(eventName, action);
        }
        
        public void OnEvent<T>(int eventName, Action<T> action) {
            this.router.OnEvent(eventName, action);
        }

        public void SendEvent(int eventName) {
            this.router.SendEvent(eventName);
        }
        
        public void SendEvent<T>(int eventName,T arg) {
            this.router.SendEvent(eventName,arg);
        }

        public virtual void OnViewCreated(IView view) {
        }

        public virtual void OnViewDestroyed(IView view) {
        }

        public virtual void OnViewShow(IView view) {
        }

        public virtual void OnViewHide(IView view) {
        }
    }

    public abstract class GameCtl<TModel> : GameCtl where TModel : IModel {
        public abstract TModel Model { get;}
    }
}
