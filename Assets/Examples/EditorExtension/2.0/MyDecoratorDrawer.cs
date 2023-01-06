using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
    using UnityEditor;

#endif
public class MyDecoratorDrawer : MonoBehaviour
{

    public int a = 1;
    public int b = 2;
    public int c = 3;

    // this shows our custom Decorator Drawer between the groups of properties
    [ColorSpace(0, 7, 180, 1, 1, 0.5f)]
    public string d = "d";
    public string e = "e";
    public string f = "f";
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}


#if UNITY_EDITOR

public class ColorSpace : PropertyAttribute
{
    public float spaceHeight;
    public float lineHeight;
    public float lineWidth;
    public Color lineColor = Color.red;

    public ColorSpace(float spaceHeight, float lineHeight, float lineWidth, float r, float g, float b)
    {
        this.spaceHeight = spaceHeight;
        this.lineHeight = lineHeight;
        this.lineWidth = lineWidth;

        // unfortunately we can't pass a color through as a Color object
        // so we pass as 3 floats and make the object here
        this.lineColor = new Color(r, g, b);
    }
}

[CustomPropertyDrawer(typeof(ColorSpace))]
public class MyDecoratorDrawer_ : DecoratorDrawer
{
    private ColorSpace ColorSpace
    {
        get { return ((ColorSpace)attribute); }
    }

    public override float GetHeight()
    {
        return base.GetHeight()+ColorSpace.spaceHeight;
    }


    public override void OnGUI(Rect position)
    {
        // calculate the rect values for where to draw the line in the inspector
        float lineX = (position.x + (position.width / 2)) - ColorSpace.lineWidth / 2;
        float lineY = position.y + (ColorSpace.spaceHeight / 2);
        float lineWidth = ColorSpace.lineWidth;
        float lineHeight = ColorSpace.lineHeight;

        // Draw the line in the calculated place in the inspector
        // (using the built in white pixel texture, tinted with GUI.color)
        Color oldGuiColor = GUI.color;
        GUI.color = ColorSpace.lineColor;
        EditorGUI.DrawRect(new Rect(lineX, lineY, lineWidth, lineHeight),ColorSpace.lineColor);
//        EditorGUI.DrawPreviewTexture(new Rect(lineX, lineY, lineWidth, lineHeight), Texture2D.whiteTexture);
        GUI.color = oldGuiColor;
    }
}


#endif
