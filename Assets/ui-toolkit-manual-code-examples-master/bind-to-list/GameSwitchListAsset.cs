using System.Collections.Generic;
using UnityEngine;

namespace UIToolkitExamples
{
    [CreateAssetMenu(menuName = "UIToolkitExamples/GameSwitchList")]
    public class GameSwitchListAsset : ScriptableObject
    {
        public List<GameSwitch> switches;

        public void Reset()
        {
            switches = new List<GameSwitch>()
            {
                new GameSwitch() { name = "Use Local Server", enabled = false },
                new GameSwitch() { name = "Show Debug Menu", enabled = false },
                new GameSwitch() { name = "Show FPS Counter", enabled = true },
            };
        }

        public bool IsSwitchEnabled(string switchName) => switches.Find(s => s.name == switchName).enabled;
    }
}