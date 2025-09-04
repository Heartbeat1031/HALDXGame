#include "TitleScene.h"

#include "BoxCollisionC.h"
#include "Global.h"
#include "ModelC.h"
#include "TransformC.h"
#include <DirectXMath.h>
#include "GameObject.h"

void TitleScene::Init() {
    auto& screen = AddGameObject<GameObject>();
    screen.AddComponent<ModelC>("assets\\models\\title.obj");
    screen.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    screen.AddComponent<BoxCollisionC>(
        JPH::EMotionType::Static, // �ÓI�ȕ���
        Vector3(10, 1, 10) // �T�C�Y��ݒ肵�܂�
    );
}

void TitleScene::Update() {
    // �O�l�̃J�����̑���
    ImGuiIO& io = ImGui::GetIO();
    // �J�����̃^�[�Q�b�g���~�N�̈ʒu�ɐݒ肵�܂�
    halgame->m_pCamera->SetTarget(Vector3(0, 0, 0));
    // �I�u�W�F�N�g�̎������]����
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);
}

void TitleScene::Uninit() {

}