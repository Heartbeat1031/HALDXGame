//
// Created by clcc777 on 25-6-11.
//

#include "SceneHierarchy.h"
#include "imgui.h"
#include "TransformC.h"

SceneHierarchy::SceneHierarchy() {
    m_selectedid = -1;
    m_rootids = {};
    m_rawindex = 0;
}

void SceneHierarchy::Begin() {
    m_rawindex = 0;
}

void SceneHierarchy::AddRoot(UID rootid) {
    if (m_rootids.size() <= m_rawindex) {
        m_rootids.push_back(rootid);
    } else {
        m_rootids[m_rawindex] = rootid;
    }
    m_rawindex++;
}

void SceneHierarchy::Draw() {
    ImGui::Begin("Scene Hierarchy");
    for (int i = 0; i < m_rootids.size(); i++) {
        UID rootid = m_rootids[i];
        TransformC* root = halgame->GetScene()->GetComponent<TransformC>(rootid);
        if (root != nullptr) {
            DrawSceneNode(rootid, m_selectedid);
        }
    }
    ImGui::End();
}

void SceneHierarchy::DrawSceneNode(const UID rootid, UID& selectedid)
{
    TransformC* root = halgame->GetScene()->GetComponent<TransformC>(rootid);
    if (root == nullptr) {
        return;
    }
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (rootid == selectedid) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    std::vector<TransformC *> childrens = root->GetChilds();
    if (childrens.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    std::string m_Name =  root->GetGameObject()->GetName();
    bool nodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void *>(static_cast<uint32_t>(rootid)), flags, m_Name.c_str());
    if (ImGui::IsItemClicked()) {
        selectedid = rootid;
    }
    if (nodeOpen) {
        for (TransformC* child : childrens) {
            DrawSceneNode(child->GetUID(), selectedid);
        }
        ImGui::TreePop();
    }
}
