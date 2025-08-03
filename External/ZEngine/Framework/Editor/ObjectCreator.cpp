#include "Framework/Editor/ObjectCreator.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Graphic/MeshRenderer.h"
#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Core/GameObject.h"
using namespace framework;

namespace editor
{

    framework::GameObject *ObjectCreator::CreateCube(const std::string &name)
    {
        auto cube = new framework::GameObject(name);
        auto meshAsset = AssetManager::GetInstance().LoadAsset<MeshAsset>("Models/primitives/Cube.obj");
        auto materialAsset = AssetManager::GetInstance().LoadAsset<MaterialAsset>("Internal/Materials/Basic.mat");


        cube->AddComponent<MeshFilter>();
        auto meshFilter = cube->GetComponent<MeshFilter>();
        if (meshFilter)
        {
            meshFilter->SetMesh(meshAsset);
        }

        cube->AddComponent<MeshRenderer>();
        auto meshRenderer = cube->GetComponent<MeshRenderer>();
        if (meshRenderer)
        {
            meshRenderer->SetMaterial(materialAsset);
        }

        return cube;
    }
} // namespace editor
