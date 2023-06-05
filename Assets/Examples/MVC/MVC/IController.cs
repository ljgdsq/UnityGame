using System;
using Loxodon.Framework.Observables;

public abstract class IController<TView,TModel> where TView:IView where TModel: IModel
{
    public readonly TView View;
    public readonly TModel Model;

    public IController(TView view, TModel model)
    {
        View = view;
        Model = model;
    }

    private void NotifyViewDataChange(string fieldName)
    {
        View.UpdateView(Model,fieldName);
    }

    protected void BindModelChangeProp<T>(ObservableProperty<T> property,string name="")
    {
        property.ValueChanged += (arg1, arg2) => { NotifyViewDataChange(name); };
    }
    
    protected abstract void  Bind();

    public void Init()
    {
        Bind();
        NotifyViewDataChange(String.Empty);
    }
}