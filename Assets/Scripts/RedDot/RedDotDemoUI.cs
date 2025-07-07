// RedDotDemoUI.cs
using UnityEngine;
using UnityEngine.UI;

public class RedDotDemoUI : MonoBehaviour
{
    public Button addNormalMailBtn;
    public Button addImportantMailBtn;
    public Button readMailBtn;
    
    private RedDotManager manager;

    private void Start()
    {
        manager = FindObjectOfType<RedDotManager>();
        
        addNormalMailBtn.onClick.AddListener(() => manager.AddNormalMail());
        addImportantMailBtn.onClick.AddListener(() => manager.AddImportantMail());
        readMailBtn.onClick.AddListener(() => manager.ReadMail());
    }
}