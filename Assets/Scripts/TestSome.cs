using UnityEngine;

public class TestSome :  MonoBehaviour
{
    private bool _isStart = false;
    private void Start()
    {
        Debug.developerConsoleVisible = false;
        _isStart = true;
    }


    private void Update()
    {
        if (_isStart)
        {
            

        }
    }
}