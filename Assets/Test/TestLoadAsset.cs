using System.Collections;
using System.Collections.Generic;
using System.IO;
using DG.Tweening;
using Extension;
using UnityEngine;
using UnityEngine.UI;

public class TestLoadAsset : MonoBehaviour
{

    public Button Button;
    public Button Button2;
    // Start is called before the first frame update
    void Start()
    {


        DotweenEx.DoTime(0.1f).OnComplete(() =>
        {
            var prefabPath = Path.Combine(Application.streamingAssetsPath, "prefabs1");
            var mtPath= Path.Combine(Application.streamingAssetsPath, "textures1");

            var texBundle=AssetBundle.LoadFromFile(mtPath);
            var assetBundle = AssetBundle.LoadFromFile(prefabPath);


//            var mt=texBundle.LoadAsset<Material>("mt1");
//            var texs=texBundle.LoadAllAssets();
            var asset = assetBundle.LoadAsset<GameObject>("CubeA");
            Instantiate(asset);
            
        });
        
        Button.onClick.AddListener(() =>
        {
            var prefabPath = Path.Combine(Application.streamingAssetsPath, "prefabs1");
            var mtPath= Path.Combine(Application.streamingAssetsPath, "textures1");
            var texBundle=AssetBundle.LoadFromFile(mtPath);
            var assetBundle = AssetBundle.LoadFromFile(prefabPath);
            
        });
        Button2.onClick.AddListener(() =>
        {
            AssetBundle.UnloadAllAssetBundles(false);
        });
        
   
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
