
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using Luban;
using SimpleJSON;


namespace cfg
{
public partial class tBReward
{
    private readonly System.Collections.Generic.Dictionary<int, Reward> _dataMap;
    private readonly System.Collections.Generic.List<Reward> _dataList;
    
    public tBReward(JSONNode _buf)
    {
        _dataMap = new System.Collections.Generic.Dictionary<int, Reward>();
        _dataList = new System.Collections.Generic.List<Reward>();
        
        foreach(JSONNode _ele in _buf.Children)
        {
            Reward _v;
            { if(!_ele.IsObject) { throw new SerializationException(); }  _v = Reward.DeserializeReward(_ele);  }
            _dataList.Add(_v);
            _dataMap.Add(_v.Id, _v);
        }
    }

    public System.Collections.Generic.Dictionary<int, Reward> DataMap => _dataMap;
    public System.Collections.Generic.List<Reward> DataList => _dataList;

    public Reward GetOrDefault(int key) => _dataMap.TryGetValue(key, out var v) ? v : null;
    public Reward Get(int key) => _dataMap[key];
    public Reward this[int key] => _dataMap[key];

    public void ResolveRef(Tables tables)
    {
        foreach(var _v in _dataList)
        {
            _v.ResolveRef(tables);
        }
    }

}

}
