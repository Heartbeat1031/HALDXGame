//
// Created by clcc777 on 25-6-11.
//

#include "SceneHierarchy.h"

#include "imgui.h"
#include "TransformC.h"

void SceneHierarchy::Draw() {
    ImGui::Begin("Scene Hierarchy");
    //DrawSceneNode(root, selectedNode);
    ImGui::End();
}

void SceneHierarchy::DrawSceneNode(const TransformC* root, const TransformC*& selected)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (root == selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    std::vector<TransformC *> childrens = root->GetChilds();
    if (childrens.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    bool nodeOpen = ImGui::TreeNodeEx((const TransformC*)root, flags, "%s", root->GetGameObject()->GetName());
    if (ImGui::IsItemClicked()) {
        selected = root;
    }
    if (nodeOpen) {
        for (TransformC* child : childrens) {
            DrawSceneNode(child, selected);
        }
        ImGui::TreePop();
    }
}
