using System;
using cfg;
using UnityEngine;

namespace GameFramework.Game
{
    public abstract class BaseScene : MonoBehaviour
    {
        public string SceneName;
        
        protected void Awake()
        {
            OnAwake();
        }

        public virtual void OnEnterScene()
        {
            var config=ConfigManager.Instance.Tables.TBScene.DataList.Find(s => s.Name == SceneName);
            SoundManager.Instance.PlayBGM(config.Music);
   
        }

        public virtual void OnExitScene()
        {
            
        }

        protected abstract void OnAwake();
      

        protected void Start()
        {
            OnStart();
        }

        protected abstract void OnStart();

    }
}