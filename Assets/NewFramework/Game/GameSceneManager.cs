using System;
using JetBrains.Annotations;
using SAssetbundle;
using UnityEngine;
using UnityEngine.SceneManagement;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace GameFramework.Game
{
    public class GameSceneManager
    {
        private GameSceneManager()
        {
            SceneManager.sceneLoaded += (scene, mode) =>
            {
                
                if (_needUnload)
                {
                    _needUnload = false;
                    ExitScene(CurrentSceneData.Scene);
                    SceneManager.UnloadSceneAsync(CurrentSceneData.Scene);
                }
                
                if (_nextSceneData!=null)
                {
                    _nextSceneData.Scene = scene;
                    SceneManager.SetActiveScene(_nextSceneData.Scene);
                    _currentSceneData = _nextSceneData;
                    _nextSceneData = null;

                }
                EnterScene(scene);
            
                GC.Collect();
                Resources.UnloadUnusedAssets();
                
            };
        }
        private static GameSceneManager _Instance;
        public static GameSceneManager Instance
        {
            get
            {
                if (_Instance==null)
                {
                    _Instance = new GameSceneManager();
                }

                return _Instance;
            }
        }


        private GameSceneData _currentSceneData;
        public GameSceneData CurrentSceneData
        {
            get
            {
                return _currentSceneData;
            }
        }

        private GameSceneData _nextSceneData;
        public GameSceneData NextSceneData => _nextSceneData;

        private GameSceneData _loadingSceneData;
        public GameSceneData LoadingSceneData => _loadingSceneData;

        private bool _needUnload;
        

        [CanBeNull]
        public AsyncOperation LoadSceneAsync(string name,bool closeBeforeScene=true,bool showLoading=true)
        {

            if (_nextSceneData!=null || !CheckScene(name))
            {
                return null;
            }

            _nextSceneData = new GameSceneData(name,closeBeforeScene);
            var op = SceneManager.LoadSceneAsync(1, LoadSceneMode.Additive);
            op.allowSceneActivation = false;
            
            
            if (showLoading)
            {
                LoadingScene.Show(op);
            }
            _needUnload = closeBeforeScene;
            if (closeBeforeScene)
            {
                if (showLoading)
                {
                    ExitScene(CurrentSceneData.Scene);
                    SceneManager.UnloadSceneAsync(CurrentSceneData.Scene); //todo wait this 
                    _needUnload = false;
                }
                else
                {
                    _needUnload = true;
                }
            }
            
            return op;
        }

        private void EnterScene(Scene scene)
        {
            var roots=scene.GetRootGameObjects();
            foreach (GameObject gameObject in roots)
            {
                var baseScene=gameObject.GetComponent<BaseScene>();
                if (baseScene)
                {
                    baseScene.OnEnterScene();
                    return;
                }
            }
        }

        private void ExitScene(Scene scene)
        {
            var roots=scene.GetRootGameObjects();
            foreach (GameObject gameObject in roots)
            {
                var baseScene=gameObject.GetComponent<BaseScene>();
                if (baseScene)
                {
                    baseScene.OnExitScene();
                    return;
                }
            }
        }

        public bool CheckScene(string name)
        {

#if UNITY_EDITOR
            var editorScene = SceneManager.GetSceneByName(name);
            if (editorScene.IsValid())
            {
                return true;
            }
            
#endif
            return AssetBundleManager.Instance.HasScene(name);
        }

    }
}