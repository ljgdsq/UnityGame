using System.Collections.Generic;

namespace StateMachine
{
    public abstract class State
    {
        public string Name;

        public abstract void Enter();

        public abstract void Update();

        public abstract void Exit();

        public abstract bool CanEnter();

        public abstract bool CanExit();
    }

    public class StateMachine
    {
        private Dictionary<string, State> _states = new Dictionary<string, State>();

        private State _currentState;
        private State _requestState;

        public void AddState(State state)
        {
            if (!_states.ContainsKey(state.Name))
            {
                _states.Add(state.Name, state);
            }
        }

        public State GetState(string name)
        {
            _states.TryGetValue(name, out var state);
            return state;
        }

        public bool RequestState(string name)
        {
            var next = GetState(name);
            if (next != null)
            {
                if (CanTransaction(_currentState, next))
                {
                    _requestState = next;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }


        public void ResetStateForce(string name)
        {
            _currentState?.Exit();
            var state = GetState(name);
            state?.Enter();
            _currentState = state;
            _requestState = null;
        }

        public void UpdateState()
        {
            if (_requestState != null)
            {
                if (CanTransaction(_currentState, _requestState))
                {
                    _currentState?.Exit();
                    _requestState.Enter();
                    _currentState = _requestState;
                    _requestState = null;
                }
                else
                {
                    _currentState?.Update();
                }
            }
            else
            {
                _currentState?.Update();
            }
        }

        private static bool CanTransaction(State pre, State next)
        {
            if (pre != null)
            {
                if (!pre.CanExit())
                {
                    return false;
                }
            }

            if (next != null)
            {
                if (!next.CanEnter())
                {
                    return false;
                }
            }

            return true;
        }
    }
}