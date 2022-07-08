namespace Save
{
    public interface IKvSave
    {
        bool Save(string key, object value);
        object Get(string key, object defaultValue);
    }
}