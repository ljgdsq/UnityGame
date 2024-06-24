using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace GameFramework.Game
{
    public  class GameSceneData
    {
        public readonly string Name;
        public readonly bool CloseBeforeScene;
        private bool _show;
        private Scene _scene;
        public GameSceneData(string name, bool closeBeforeScene)
        {
            Name = name;
            CloseBeforeScene = closeBeforeScene;
        }


        public Scene Scene
        {
            get => _scene;
            set => _scene = value;
        }

        // public void Show()
        // {
        //     if (_show)
        //     {
        //         return;
        //     }
        //
        //     if (_scene.name==Name)
        //     {
        //         SceneManager.SetActiveScene(_scene);
        //     }
        // }
    }

}
