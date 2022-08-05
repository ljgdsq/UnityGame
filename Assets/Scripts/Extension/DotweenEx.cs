using DG.Tweening;

namespace Extension
{
    public  class DotweenEx
    {
        public static Tween DoTime(float time)
        {
            return DOTween.To(() => time, (v) => { }, time,time);
        }
    }
}