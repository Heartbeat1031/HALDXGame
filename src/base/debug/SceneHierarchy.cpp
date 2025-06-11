//
// Created by clcc777 on 25-6-11.
//

#include "SceneHierarchy.h"

#include "GameObject.h"
#include "Global.h"
#include "imgui.h"
#include "TransformC.h"

SceneHierarchy::SceneHierarchy() {
    m_selectedid = -1;
}

void SceneHierarchy::Draw() {
    ImGui::Begin("Hierarchy");
    GameObject *root = halgame->GetScene()->GetRoot();
    if (root != nullptr) {
        UID rootid = root->GetUID();
        DrawSceneNode(rootid, m_selectedid);
    } else {
        ImGui::Text("No Scene Loaded");
    }
    ImGui::End();
    // 选中节点的详细信息
    ImGui::Begin("Inspector");
    DrawInspector(m_selectedid);
    ImGui::End();
}

void SceneHierarchy::DrawSceneNode(const UID rootid, UID &selectedid) {
    TransformC *root = halgame->GetScene()->GetComponent<TransformC>(rootid);
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
    std::string m_Name = root->GetGameObject()->GetName() + " (" + std::to_string(rootid) + ")";
    bool nodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void *>(static_cast<uintptr_t>(rootid)), flags, m_Name.c_str());
    if (ImGui::IsItemClicked()) {
        selectedid = rootid;
    }
    if (nodeOpen) {
        for (TransformC *child: childrens) {
            DrawSceneNode(child->GetUID(), selectedid);
        }
        ImGui::TreePop();
    }
}

//void SceneHierarchy::DrawInspector(UID selectedid) {
//    if (selectedid == -1) {
//        ImGui::Text("No Object Selected");
//        return;
//    }
//
//    GameObject *go = halgame->GetScene()->GetGameObject<GameObject>(selectedid);
//    if (!go) {
//        ImGui::Text("GameObject not found.");
//        return;
//    }
//
//    ImGui::Text("名前 : %s (%d)", go->GetName().c_str(), selectedid);
//
//    // ------ Transform ------
//    TransformC &tf = go->GetComponentRef<TransformC>();
//    ImGui::Text("Transform");
//
//    ImGui::DragFloat3("Position", &tf.position.x, 0.1f);
//    ImGui::DragFloat3("Rotation", &tf.rotation.x, 0.5f);
//    ImGui::DragFloat3("Scale",    &tf.scale.x,    0.05f);
//}

void SceneHierarchy::DrawInspector(UID selectedid) {
    if (selectedid == -1) {
        ImGui::Text("No Object Selected");
        return;
    }

    GameObject* go = halgame->GetScene()->GetGameObject<GameObject>(selectedid);
    if (!go) {
        ImGui::Text("GameObject not found.");
        return;
    }

    ImGui::Text("名前 : %s (%d)", go->GetName().c_str(), selectedid);

    // ------ Transform ------
    TransformC& tf = go->GetComponentRef<TransformC>();
    ImGui::Text("Transform");

    float pos[3] = {
        tf.GetLocalPosition().x,
        tf.GetLocalPosition().y,
        tf.GetLocalPosition().z
    };
    float rot[3] = {
        tf.GetLocalRotationEuler().x,
        tf.GetLocalRotationEuler().y,
        tf.GetLocalRotationEuler().z
    };
    float scale[3] = {
        tf.GetLocalScale().x,
        tf.GetLocalScale().y,
        tf.GetLocalScale().z
    };

    if (ImGui::DragFloat3("Position", pos, 0.1f)) {
        tf.SetLocalPosition({ pos[0], pos[1], pos[2] });
    }

    if (ImGui::DragFloat3("Rotation", rot, 0.5f)) {
        tf.SetLocalRotationEuler({ rot[0], rot[1], rot[2] });
    }

    if (ImGui::DragFloat3("Scale", scale, 0.05f)) {
        tf.SetLocalScale({ scale[0], scale[1], scale[2] });
    }
}
