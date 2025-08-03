#include "Framework/Editor/Inspector/CameraInspector.h"
#include "Framework/Component/Camera.h"

using namespace framework;

namespace editor
{

    void CameraInspector::Update(float deltaTime)
    {
        ComponentInspector::Update(deltaTime);
    }

    void CameraInspector::Inspect(framework::GameObject *node)
    {
        Camera *camera = node->GetComponent<Camera>();
        if (camera)
        {
            // Clear Color
            ImGui::Text("Clear Flags");
            ImGui::ColorEdit4("Background", &camera->clearColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

            ImGui::Separator();

            // Projection Type (需要通过直接访问私有成员或添加getter)
            ImGui::Text("Projection");
            static const char *projectionTypes[] = {"Perspective", "Orthographic"};
            static int currentProjection = 0; // 默认透视投影
            if (ImGui::Combo("##ProjectionType", &currentProjection, projectionTypes, 2))
            {
                camera->SetProjectionType((ProjectionType)currentProjection);
            }

            ImGui::Separator();

            // Projection specific settings
            if (currentProjection == 0) // Perspective
            {
                // Field of View
                static float fov = 45.0f;
                if (ImGui::SliderFloat("Field of View", &fov, 10.0f, 179.0f, "%.1f°"))
                {
                    camera->SetFieldOfView(fov);
                }
            }
            else // Orthographic
            {
                // Orthographic Size (使用SetOrthographicProjection)
                static float orthoSize = 10.0f;
                if (ImGui::DragFloat("Size", &orthoSize, 0.1f, 0.1f, 100.0f, "%.2f"))
                {
                    // 重新设置正交投影
                    camera->SetOrthographicProjection(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
                }
            }

            // Clipping Planes
            ImGui::Separator();
            ImGui::Text("Clipping Planes");

            static float nearPlane = 0.1f;
            if (ImGui::DragFloat("Near", &nearPlane, 0.01f, 0.01f, 99.0f, "%.3f"))
            {
                camera->SetNearPlane(nearPlane);
            }

            static float farPlane = 100.0f;
            if (ImGui::DragFloat("Far", &farPlane, 1.0f, nearPlane + 0.01f, 1000.0f, "%.1f"))
            {
                camera->SetFarPlane(farPlane);
            }

            // Aspect Ratio
            ImGui::Separator();
            static float aspectRatio = 16.0f / 9.0f;
            if (ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.01f, 0.1f, 10.0f, "%.3f"))
            {
                camera->SetAspectRatio(aspectRatio);
            }

            // Main Camera
            ImGui::Separator();
            bool isMainCamera = camera->IsMainCamera();
            if (ImGui::Checkbox("Main Camera", &isMainCamera))
            {
                camera->SetMainCamera(isMainCamera);
            }

            // Camera Information (Read-only)
            ImGui::Separator();
            ImGui::Text("Camera Information");
            if (ImGui::CollapsingHeader("Projection Matrix"))
            {
                auto projMat = camera->GetProjectionMatrix();
                ImGui::Text("%.3f %.3f %.3f %.3f",
                            projMat[0][0], projMat[1][0], projMat[2][0], projMat[3][0]);
                ImGui::Text("%.3f %.3f %.3f %.3f",
                            projMat[0][1], projMat[1][1], projMat[2][1], projMat[3][1]);
                ImGui::Text("%.3f %.3f %.3f %.3f",
                            projMat[0][2], projMat[1][2], projMat[2][2], projMat[3][2]);
                ImGui::Text("%.3f %.3f %.3f %.3f",
                            projMat[0][3], projMat[1][3], projMat[2][3], projMat[3][3]);
            }
        }
    }

    std::string CameraInspector::GetComponentDisplayName() const
    {
        return "Camera";
    }
}
