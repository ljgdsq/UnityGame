using System;
using System.IO;
using UnityEngine;

namespace Utility {
    /// <summary>
    ///  GamePlayerPrefs 
    /// </summary>
    /// <remarks>
    /// 日期: 2025年6月17日
    /// </remarks>
    public class GamePlayerPrefs {
        public static T LoadData<T>(string fileName, string subFolder = null) {
            var basePath = Application.persistentDataPath;
            if (!string.IsNullOrEmpty(subFolder)) {
                basePath = Path.Combine(basePath, subFolder);
            }

            var filePath = Path.Combine(basePath, fileName);
            if (File.Exists(filePath)) {
                try {
                    return JsonUtility.FromJson<T>(File.ReadAllText(filePath));
                } catch (Exception e) {
                    Debug.Log(e);
                }
            }

            return default;
        }

        public static void SaveData<T>(string fileName, T data, string subFolder = null) {
            var basePath = Application.persistentDataPath;
            if (!string.IsNullOrEmpty(subFolder)) {
                basePath = Path.Combine(basePath, subFolder);
            }

            if (!Directory.Exists(basePath)) {
                Directory.CreateDirectory(basePath);
            }

            var filePath = Path.Combine(basePath, fileName);
            try {
                File.WriteAllText(filePath, JsonUtility.ToJson(data));
            } catch (Exception e) {
                Debug.Log(e);
            }
        }

        public static void CleanPath(string path) {
            try {
                var dirPath = Path.Combine(Application.persistentDataPath, path);
                if (Directory.Exists(dirPath)) {
                    Directory.Delete(dirPath, true);
                }
            } catch (Exception e) {
                Debug.Log(e);
            }
        }
    }
}