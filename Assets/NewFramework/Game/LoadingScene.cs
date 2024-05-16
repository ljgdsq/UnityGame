using System;
using UnityEngine;

namespace GameFramework.Game
{
    public class LoadingScene:BaseScene
    {

        protected AsyncOperation _asyncTask;
        public override void OnEnterScene()
        {
            base.OnEnterScene();
        }

        public override void OnExitScene()
        {
            base.OnExitScene();
        }

        protected override void OnAwake()
        {
           
        }

        protected override void OnStart()
        {
          
        }

        public static void Show(AsyncOperation op)
        {
            
        }
        public static void Hide(){}

        private void ShowProgress(float value)
        {
            
        }

        private void ProgressDone()
        {
            _asyncTask = null;
            Hide();
        }
        
        private void Update()
        {
            if (_asyncTask!=null)
            {
                if (_asyncTask.isDone)
                {
                    ShowProgress(_asyncTask.progress);
                    ProgressDone();
                }
                else
                {
                    ShowProgress(_asyncTask.progress);
                }
            }
        }
    }
}