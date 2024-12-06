using System.Collections;
using System.Collections.Generic;
using Goose.UIFramework;
using UnityEngine;


[UIConfig("TestViews/PersistentView", UIType.Window, UISize.Custom, UILayer.Dialog,isPersistent: true)]
public class TestPersistentView : UIBase { }