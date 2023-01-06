using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "TestAsset.asset",menuName = "CreateAsset/TestAsset")]
public class TestAsset : ScriptableObject
{
  public string id;
  public string name;
  public int tag;
}
