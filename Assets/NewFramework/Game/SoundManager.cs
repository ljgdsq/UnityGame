using Base;
using UnityEngine;

namespace GameFramework.Game
{
    public class SoundManager:Singleton<SoundManager>
    {

        private static readonly float BGM_DADETIME = 0.6f;
        private int BGM;
        
        public void PlayBGM(int id)
        {
            if (BGM!=0)
            {
                FadeMusic(BGM,0.5f);
            }
            Debug.Log("PlayBGM " +id);
            BGM = id;
        }


        public void FadeMusic(int id, float? duration)
        {
            Debug.Log("FadeMusic " +id);
        }
    }
}