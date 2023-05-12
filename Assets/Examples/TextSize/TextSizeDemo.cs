using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class TextSizeDemo : MonoBehaviour
{

    public Text _text;
    // Start is called before the first frame update
    void Start()
    {
        var preWidth =_text.preferredWidth;

        Debug.Log(preWidth);
        _text.text="1234567890";
         preWidth =_text.preferredWidth;

        Debug.Log(preWidth);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
