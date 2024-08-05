public class Tracking : MonoBehaviour
{

    public Transform Center;

    public Transform Target;
    
    public Transform Arrow;

    
    public float screenAspectRatio;
    // Start is called before the first frame update
    void Start()
    {
        screenAspectRatio = Screen.height*1.0f / Screen.width;
    }
    public float radius = 500f;  // 圆周运动的半径
    public float speed = 2f;   // 圆周运动的速度
    private float angle = 0f;  // 当前角度

    // Update is called once per frame
    void Update()
    {
        var dir = Target.position - Center.position;

        var aspectRatio = Mathf.Abs(dir.y / dir.x);
        if (aspectRatio>screenAspectRatio)
        {
            var x = Screen.height/ (aspectRatio*2);
            var y = UnityEngine.Device.Screen.height / 2;
            if (dir.x>0)
            {
                if (dir.y>0)
                {
                    Arrow.transform.localPosition = new Vector3(x, y, 0);
                }
                else
                {
                    Arrow.transform.localPosition = new Vector3(x, -y, 0);
                }
            }
            else
            {
                if (dir.y>0)
                {
                    Arrow.transform.localPosition = new Vector3(-x, y, 0);
                }
                else
                {
                    Arrow.transform.localPosition = new Vector3(-x, -y, 0);
                }
            }
        }
        else
        {
            var y = Screen.width * aspectRatio/2;
            var x = Screen.width/2;
            if (dir.x>0)
            {
                if (dir.y>0)
                {
                    Arrow.transform.localPosition = new Vector3(x, y, 0);
                }
                else
                {
                    Arrow.transform.localPosition = new Vector3(x, -y, 0);
                }
            }
            else
            {
                if (dir.y>0)
                {
                    Arrow.transform.localPosition = new Vector3(-x, y, 0);
                }
                else
                {
                    Arrow.transform.localPosition = new Vector3(-x, -y, 0);
                }
            }
        }

        {
            angle += speed * Time.deltaTime;

            // 计算新的位置
            float x = radius * Mathf.Cos(angle);
            float z = radius * Mathf.Sin(angle);

            // 更新物体的位置
            Target.localPosition = new Vector3(x, z, 0);
        }

    }
}
