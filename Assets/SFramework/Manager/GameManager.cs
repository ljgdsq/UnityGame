using System.Collections;
using UnityEngine;

namespace SFramework.Manager
{
    public class GameManager
    {
        private static GameManager _instance;
        public static GameManager Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new GameManager();
                return _instance;
            }
        }
        public MonoBehaviour Behaviour;
        
        public void StartCoroutine(IEnumerator enumerator)
        {
            Behaviour.StartCoroutine(enumerator);
        }
    }
}