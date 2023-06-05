using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;


public class PlayerView : MonoBehaviour,IView
{
    
    [SerializeField]
    private Button _button;
    [SerializeField]
    private Text _text;


    public UnityAction OnClick;

    private void Start()
    {
        _button.onClick.AddListener(() =>
        {
            OnClick?.Invoke();
        });
    }

    public void UpdateView(IModel model, string filedName = "")
    {
        var m = model as PlayerModel;
        _text.text = m.Level.ToString();
    }

}
