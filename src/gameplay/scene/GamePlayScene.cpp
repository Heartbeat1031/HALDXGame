// Created by lclcl on 25-5-22.

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
#include "WinScene.h"

#include <cmath> // std::sin, std::cos, std::sqrt

void GamePlayScene::Init() {
    // 発射位置初期化（旧变量保留，不再直接使用）
    BaseFirePosition = {0, 4.0f, -10.0f};

    // 地面を追加するサンプル
    auto &ground = AddGameObject<GameObject>();
    ground.AddComponent<ModelC>("assets/models/ground_19.obj");
    ground.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    ground.AddComponent<BoxCollisionC>(
        JPH::EMotionType::Static, // 静的な物体
        Vector3(10, 1, 10)
    );
    ground.GetComponentRef<BoxCollisionC>().SetOffsetTransform(
        OffsetTransform{
            Vector3(0, -2, 0),
            Vector3(0, 0, 0)
        }
    );
    ground.GetComponentRef<BoxCollisionC>().SetPosition(Vector3(0, -1, 0));

    // プレイヤー & Idol
    miku = &AddGameObject<Player>();
    if (miku->HasComponent<BoxCollisionC>()) {
        miku->GetComponentRef<BoxCollisionC>().SetPosition(Vector3(-5, 8, 0));
    }
    idol = &AddGameObject<Idol>();
    idol->GetComponentRef<BoxCollisionC>().SetPosition(Vector3(5, 10, 0));
    idol->IsDead = false;
}

void GamePlayScene::FireBullet(const Vector3 StartPosition, const Vector3 Direction, const float Speed) {
    Bullet &bullet = halgame->GetScene()->AddGameObject<Bullet>();
    bullet.GetComponentRef<CapsuleCollisionC>().SetPosition(StartPosition);
    bullet.Fire(Direction, Speed);
}

void GamePlayScene::Update(float dt) {
    if (ImGui::Begin("メッシュ")) {
        ImGui::Text("三人称モード");
        ImGui::Text("マウスの右ボタンを押したままビューをドラッグします");
        ImGui::Text("WASDキーで移動します");
        ImGui::Text("FキーはBulletを発射します（※現仕様では自動発射に切替）");
    }
    ImGui::End();

    // 三人称カメラ
    ImGuiIO &io = ImGui::GetIO();
    halgame->m_pCamera->SetTarget(miku->GetComponentRef<TransformC>().GetWorldPosition() + Vector3(0, 2, 0));
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);

    // 地面网格
    int drawSize = 10;
    for (int x = -drawSize; x < drawSize; x++) {
        halgame->DrawLine(
            Vector3(x, 0.01f, -drawSize),
            Vector3(x, 0.01f,  drawSize),
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f)
        );
        halgame->DrawLine(
            Vector3(-drawSize, 0.01f, x),
            Vector3( drawSize, 0.01f, x),
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f)
        );
    }

    // Idol 死亡则切场景
    if (idol != nullptr && idol->HP <= 0) {
        halgame->SetScene<GameoverScene>();
    }
    //  if (idol!=nullptr&&idol->GetComponentRef<TransformC>().GetWorldPosition().y<-10.0f) {
    //     idol->IsDead = true;
    //      halgame->SetScene<GameoverScene>();
    // }

    // =========================
    // 自动发射：每秒 4 发、共 180 发、360° 扫射
    // =========================
    static float sFireTimer  = 0.0f;
    static int   sFiredCount = 0;

    constexpr int   kTotalShots   = 180;     // 总数
    constexpr float kInterval     = 0.25f;   // 每秒4发
    constexpr float kBulletSpeed  = 20.0f;   // 子弹速度

    const Vector3 center = Vector3(0.0f, 4.0f, 0.0f);   // 中心点 (0,4,0)；若要 0.4f，把这里改成 0.4f
    const Vector3 first  = Vector3(0.0f, 4.0f, -10.0f); // 首发点
    const float   radius = 10.0f; // (first - center).Length(); // 也可动态算

    // 面板显示调试信息（可删）
    if (ImGui::Begin("Auto Fire Debug")) {
        ImGui::Text("Shots: %d / %d", sFiredCount, kTotalShots);
        ImGui::Text("Timer: %.3f", sFireTimer);


    }
    ImGui::End();

    sFireTimer += dt;
    while (sFireTimer >= kInterval && sFiredCount < kTotalShots) {
        // 步进角度（度）：0° 对应 (0,4,-10)，顺时针（+θ）绕 XZ 平面
        const float angleDeg = sFiredCount * (360.0f / static_cast<float>(kTotalShots));
        const float angleRad = angleDeg * (DirectX::XM_PI / 180.0f);

        // 令 0° 落在 -Z 方向：offset = (sinθ, 0, -cosθ) * R
        const float ox = std::sin(angleRad) * radius;
        const float oz = -std::cos(angleRad) * radius;

        const Vector3 pos = center + Vector3(ox, 0.0f, oz);

        // 径向单位方向（从中心指向发射点）
        const float len = std::sqrt(ox * ox + oz * oz);
        const Vector3 dir = (len > 0.0f) ? Vector3(-ox / len, 0.0f, -oz / len)
                                         : Vector3(0.0f, 0.0f, -1.0f);

        FireBullet(pos, dir, kBulletSpeed);

        ++sFiredCount;
        sFireTimer -= kInterval;
    }
    if (sFiredCount >= kTotalShots) {
        halgame->SetScene<WinScene>();
    }

    // 如需循环不停地打一圈再一圈，把上面 while 保持，外面再放开下面这段：
    // if (sFiredCount >= kTotalShots) { sFiredCount = 0; }
}

void GamePlayScene::Uninit() {
}
