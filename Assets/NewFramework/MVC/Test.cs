// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
using UnityEngine;
using UnityEngine.UI;

namespace NewFramework.MVC {

    /// <summary>
    ///  Test 
    /// </summary>
    /// <remarks>
    /// 日期: 2025年8月22日
    /// </remarks>
    public class Test {
        public void test() {
            
        }
    }

    public class LoginData : IModel {
        public ModelProperties<string> UserName = new();
        public ModelProperties<string> Password = new();
    }
    
    public class LoginCtl : GameCtl<LoginData> {
        public override LoginData Model { get; } = new LoginData();

        public void Login(string username, string password) {
            if (username == "admin" && password == "admin") {
                SendEvent(1, "Login Success!");
            }
            
        }
    }
    
    public class LoginView : GameView<LoginCtl> {
        
        private InputField userNameInput;
        private InputField passwordInput;
        
        private Text nameText;
        private Text loginResult;
        protected override void OnInit() {
            base.OnInit();
            ctrl.Model.UserName.Subscribe((n) => {
                nameText.text = n;
            });
            OnNotify<string>(1,OnLoginResult);
            
        }
        
        private void OnLoginResult(string result) {
            loginResult.text = result;
        }

        protected override void OnUpdate() {
            base.OnUpdate();
            if (Input.GetMouseButtonDown(0)) {
                this.ctrl.Login(userNameInput.text, passwordInput.text);
            }
        }
    }
}
