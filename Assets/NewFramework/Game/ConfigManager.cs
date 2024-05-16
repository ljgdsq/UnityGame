using System.IO;
using Base;
using cfg;
using SimpleJSON;
using UnityEngine;

namespace GameFramework.Game
{
    public class ConfigManager:Singleton<ConfigManager>
    {

        public cfg.Tables Tables;


        public override void Init()
        {
            base.Init();
            Tables = new Tables(LoadByteBuf);
        }
        
        private static JSONNode LoadByteBuf(string file)
        {
        
            Debug.Log($"load json {file}");
            return JSON.Parse(File.ReadAllText(Application.dataPath + "/../../GenerateDatas/json/" + file + ".json", System.Text.Encoding.UTF8));
        }
    }
}