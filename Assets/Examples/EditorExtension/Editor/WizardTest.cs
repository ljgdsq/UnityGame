using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine;

using UnityEditor;

public class WizardTest : ScriptableWizard {

    public int changeStartHpValue = 150;
    public int ChangeSinkSpeed = 11;
    public const string changeStartHpValueKey = "changeStartHpValue";
    public const string ChangeSinkSpeedKey = "ChangeSinkSpeed";

    //创建对话框扩展
    [MenuItem("Tools/CreateWizard")]
    static void CreateWizard()
    {
        DisplayWizard<WizardTest>("统一修改敌人","Change And Close","Change");
        //EnemyChange为类名，对话框名字，Create按钮的名字，OtherButton按钮名字
        //第2、3参数都可以省略
        //Create点击后会关闭对话框，OtherButton按钮点击后不会关闭对话框
    }

    //检测Create按钮点击的自带方法
    private void OnWizardCreate()
    {
        GameObject[] enemyPrefabs =  Selection.gameObjects;

        int count = 0;
        EditorUtility.DisplayProgressBar("进度", count + "/" + enemyPrefabs.Length, 0);

        foreach (GameObject go in enemyPrefabs)
        {
            //获取物体的EnemyHealth脚本
//            CompleteProject.EnemyHealth hp = go.GetComponent<CompleteProject.EnemyHealth>();
//            Undo.RecordObject(hp, "Change Health And Speed");   //开始记录对hp的更改，之后对其的更改都可以撤销

            //改变物体的属性
//            hp.startingHealth = changeStartHpValue;
//            hp.sinkSpeed = ChangeSinkSpeed;

            count++;
            EditorUtility.DisplayProgressBar("进度", count + "/" + enemyPrefabs.Length, (float)count/enemyPrefabs.Length);
        }
        EditorUtility.ClearProgressBar();
    }

    //检测Other Button按钮方法
    private void OnWizardOtherButton()
    {
        OnWizardCreate();
        ShowNotification(new GUIContent(Selection.gameObjects.Length + "值被修改了"));
    }

	//打开对话框或改变对话框内容时调用此方法
    private void OnWizardUpdate()
    {
        helpString = "";
        errorString = "";
        if(Selection.gameObjects.Length > 0)
        {
            helpString = "您当前选择了" + Selection.gameObjects.Length + "个敌人";
        }
        else
        {
            errorString = "请选择至少一个敌人";
        }
        EditorPrefs.SetInt("changeStartHpValue", changeStartHpValue);
        EditorPrefs.SetInt("ChangeSinkSpeed", ChangeSinkSpeed);
    }

	//当窗口被创建时调用
    private void OnEnable()
    {
        changeStartHpValue = EditorPrefs.GetInt(changeStartHpValueKey, changeStartHpValue);
        ChangeSinkSpeed = EditorPrefs.GetInt(ChangeSinkSpeedKey, ChangeSinkSpeed);
        
    }

    //打开修改选中的物体时调用此方法
    private void OnSelectionChange()
    {
        OnWizardUpdate();
    }
}