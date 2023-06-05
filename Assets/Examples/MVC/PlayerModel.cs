using System.Collections;
using System.Collections.Generic;
using Loxodon.Framework.Observables;
using UnityEngine;

public class PlayerModel:IModel
{
    public ObservableProperty<int> Level;
    public ObservableProperty<string> Name;

    public PlayerModel(ObservableProperty<int> level, ObservableProperty<string> name)
    {
        Level = level;
        Name = name;
    }

    public void AddLevel()
    {
        Level.Value++;
    }
}
