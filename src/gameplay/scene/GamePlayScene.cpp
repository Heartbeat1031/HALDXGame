//
// Created by lclcl on 25-5-22.
//

#include "GamePlayScene.h"

#include "BoxCollisionC.h"
#include "Global.h"
#include "Idol.h"
#include "Player.h"
#include "ModelC.h"
#include "CapsuleCollisionC.h"
#include "TransformC.h"
#include <DirectXMath.h>

#include "Bullet.h"
#include "GameoverScene.h"

void GamePlayScene::Init() {
    //発射位置初期化
    BaseFirePosition = {0, 4.0f, -10.0f};

    // 地面を追加するサンプル
    auto &ground = AddGameObject<GameObject>();
    // 地面のモデルコンポーネントを追加します
    ground.AddComponent<ModelC>("assets/models/ground_19.obj");
    // 地面の座標を設定します
    ground.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    ground.AddComponent<BoxCollisionC>(
        JPH::EMotionType::Static, // 静的な物体
        Vector3(10, 1, 10) // サイズを設定します
    );
    ground.GetComponentRef<BoxCollisionC>().SetOffsetTransform(
        OffsetTransform{
            Vector3(0, -2, 0), // 地面の位置オフセット
            Vector3(0, 0, 0) // 回転オフセットはなし
        }
    );
    ground.GetComponentRef<BoxCollisionC>().SetPosition(Vector3(0, -1, 0));

    // ゲームオブジェクトを追加します
    miku = &AddGameObject<Player>();
    if (miku->HasComponent<BoxCollisionC>()) {
        miku->GetComponentRef<BoxCollisionC>().SetPosition(Vector3(-5, 8, 0));
    }
    idol = &AddGameObject<Idol>();
    idol->GetComponentRef<BoxCollisionC>().SetPosition(Vector3(5, 10, 0));
}

void GamePlayScene::FireBullet(Vector3 StartPosition, Vector3 Direction, float Speed) {
    Bullet &bullet = halgame->GetScene()->AddGameObject<Bullet>();
    bullet.GetComponentRef<CapsuleCollisionC>().SetPosition(StartPosition);
    bullet.Fire(Direction, Speed);
};

void GamePlayScene::Update(float dt) {
    if (ImGui::Begin("メッシュ")) {
        ImGui::Text("三人称モード");
        ImGui::Text("マウスの右ボタンを押したままビューをドラッグします");
        ImGui::Text("WASDキーで移動します");
        ImGui::Text("FキーはBulletを発射します");
    }
    ImGui::End();

    // 三人称カメラの操作
    ImGuiIO &io = ImGui::GetIO();
    // カメラのターゲットをミクの位置に設定します
    halgame->m_pCamera->SetTarget(miku->GetComponentRef<TransformC>().GetWorldPosition() + Vector3(0, 2, 0));
    // オブジェクトの周りを回転する
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);

    int drawSize = 10;
    // 地面に小さな立方体を描画します
    for (int x = -drawSize; x < drawSize; x++) {
        halgame->DrawLine(
            Vector3(x, 0.01f, -drawSize),
            Vector3(x, 0.01f, drawSize),
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f)
        );
        halgame->DrawLine(
            Vector3(-drawSize, 0.01f, x),
            Vector3(drawSize, 0.01f, x),
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f)
        );
    }
    if (idol != nullptr && idol->HP <= 0) {
        halgame->SetScene<GameoverScene>();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_F, false)) {
        FireBullet(Vector3(0, 4.0f, -10.0f), Vector3{0.0f,0.0f, 1.0f}, 20.0f);
        FireBullet(Vector3(0, 4.0f, 10.0f), Vector3{0.0f,0.0f, -1.0f}, 20.0f);
        FireBullet(Vector3(-10, 4.0f, 0), Vector3{1.0,0.0f, 0.0f}, 20.0f);
        FireBullet(Vector3(10, 4.0f, 0), Vector3{-1.0f,0.0f, 0.0f}, 20.0f);
    }
}

void GamePlayScene::Uninit() {
}
