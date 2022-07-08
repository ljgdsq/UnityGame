namespace Save
{
    public class PlayerPrefsSave:IKvSave
    {
        public bool Save(string key, object value)
        {
            return true;
        }

        public object Get(string key, object defaultValue)
        {
            return null;
        }
    }
}