using System.Collections;
using System.Collections.Generic;
using Loxodon.Framework.Views;
using UnityEngine;

public class PlayerController:IController<PlayerView,PlayerModel>
{
    public PlayerController(PlayerView view, PlayerModel model) : base(view, model)
    {
    }

    protected override void Bind()
    {
        View.OnClick = AddLevel;
        BindModelChangeProp(Model.Level);
    }

    private void AddLevel()
    {
        Model.AddLevel();
    }
}
