
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
public sealed partial class Reward : Luban.BeanBase
{
    public Reward(JSONNode _buf) 
    {
        { if(!_buf["id"].IsNumber) { throw new SerializationException(); }  Id = _buf["id"]; }
        { if(!_buf["name"].IsString) { throw new SerializationException(); }  Name = _buf["name"]; }
        { if(!_buf["desc"].IsString) { throw new SerializationException(); }  Desc = _buf["desc"]; }
        { if(!_buf["price"].IsNumber) { throw new SerializationException(); }  Price = _buf["price"]; }
        { var _j = _buf["expire_time"]; if (_j.Tag != JSONNodeType.None && _j.Tag != JSONNodeType.NullValue) { { if(!_j.IsNumber) { throw new SerializationException(); }  ExpireTime = _j; } } else { ExpireTime = null; } }
        { if(!_buf["batch_useable"].IsBoolean) { throw new SerializationException(); }  BatchUseable = _buf["batch_useable"]; }
    }

    public static Reward DeserializeReward(JSONNode _buf)
    {
        return new Reward(_buf);
    }

    /// <summary>
    /// 这是id
    /// </summary>
    public readonly int Id;
    /// <summary>
    /// 名字
    /// </summary>
    public readonly string Name;
    /// <summary>
    /// 描述
    /// </summary>
    public readonly string Desc;
    /// <summary>
    /// 价格
    /// </summary>
    public readonly int Price;
    /// <summary>
    /// 过期时间
    /// </summary>
    public readonly long? ExpireTime;
    /// <summary>
    /// 能否批量使用
    /// </summary>
    public readonly bool BatchUseable;
   
    public const int __ID__ = -1850459313;
    public override int GetTypeId() => __ID__;

    public  void ResolveRef(Tables tables)
    {
        
        
        
        
        
        
    }

    public override string ToString()
    {
        return "{ "
        + "id:" + Id + ","
        + "name:" + Name + ","
        + "desc:" + Desc + ","
        + "price:" + Price + ","
        + "expireTime:" + ExpireTime + ","
        + "batchUseable:" + BatchUseable + ","
        + "}";
    }
}

}