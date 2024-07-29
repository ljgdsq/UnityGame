using SFramework.Update.State;
using YooAsset;

namespace SFramework.Update
{
    
    public class PatchOperation: GameAsyncOperation
    {
        private readonly UniFramework.Machine.StateMachine _machine;


        public PatchOperation(string packageName, EPlayMode playMode)
        {
            _machine = new UniFramework.Machine.StateMachine(this);
            _machine.SetBlackboardValue("PackageName", packageName);
            _machine.SetBlackboardValue("PlayMode", playMode);
            _machine.SetBlackboardValue("BuildPipeline",EDefaultBuildPipeline.BuiltinBuildPipeline.ToString());
            
            _machine.AddNode<InitializePackageState>();
            _machine.AddNode<UpdatePackageVersionState>();
        }
        
        
        protected override void OnStart()
        {
            _machine.Run<InitializePackageState>();
        }

        protected override void OnUpdate()
        {
  
        }

        protected override void OnAbort()
        {
   
        }
    }
}