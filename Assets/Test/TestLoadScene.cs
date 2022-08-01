using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TestLoadScene : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
        string streamingAssetsAbPath = Path.Combine(Application.streamingAssetsPath, "StreamingAssets");
        AssetBundle streamingAssetsAb = AssetBundle.LoadFromFile(streamingAssetsAbPath);
       var  m_manifest = streamingAssetsAb.LoadAsset<AssetBundleManifest>("AssetBundleManifest");
        
        
        var assetBundle = AssetBundle.LoadFromFile(Application.streamingAssetsPath+"/test_prefab.ab");
        if (assetBundle)

        {
            var loadAsset = assetBundle.LoadAsset<GameObject>("Chess");
            GameObject.Instantiate(loadAsset);
            
            
                
        }
//        {
//            var allDependencies = m_manifest.GetAllDependencies("scene.ab");
//            foreach (var allDependency in allDependencies)
//            {
//               // AssetBundle.LoadFromFile(Application.streamingAssetsPath+"/"+allDependency);
//            }
//
//            //var loadAllAssets = assetBundle.LoadAsset("SceneA.unity");
//        }
//
//        SceneManager.LoadScene("SceneA");

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
