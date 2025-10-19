#include "LoadScene.h"
#include "PlayScene.h"
#include <DxLib.h>

LoadScene::LoadScene()
    : gauge(290, 620, 700, 25) // 画面中央下部に配置（座標は仮）
{
    lo = LoadGraph("Data/Load.png");
    frame = 0;
    prevMouse = false;
}

LoadScene::~LoadScene() {
    DeleteGraph(lo);
}

SceneBase* LoadScene::Update() {
    frame++;
    bool nowMouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    // ロードゲージを進行（約3秒で満タン）
    gauge.Update(-0.0f, -0.0f, -0.0f, true, false);
    float progress = frame / 180.0f; // 180フレーム（3秒）で完了
    if (progress > 1.0f) progress = 1.0f;

    // ゲージの伸びを強制的に上書き（左→右に上昇）
    // CandleGauge内部のcurrentを直接制御したいが、外からはできないため描画時に工夫
    // → Draw() 側で frame を利用して幅を変える

    if (frame >= 180 && nowMouse && !prevMouse)
        return new PlayScene();

    prevMouse = nowMouse;
    return this;
}

void LoadScene::Draw() {
    DrawGraph(0, 0, lo, TRUE);

    // --- キャンドルゲージ風のロードバー描画 ---
    int x = 290, y = 620, w = 700, h = 25;
    float progress = frame / 180.0f;
    if (progress > 1.0f) progress = 1.0f;

    // 背景
    DrawBox(x, y, x + w, y + h, GetColor(40, 40, 40), TRUE);

    // 左から右に進むバー
    DrawBox(x, y, x + (int)(w * progress), y + h, GetColor(255, 150, 0), TRUE);

    // 枠線
    DrawBox(x, y, x + w, y + h, GetColor(0, 0, 0), FALSE);

    // 炎エフェクト（キャンドル風）
    if (progress > 0.0f && progress < 1.0f) {
        int flameBaseX = x + (int)(w * progress);
        int flameBaseY = y;
        int flameCount = 5;
        for (int i = 0; i < flameCount; ++i) {
            int offsetX = (rand() % 7 - 3);
            int offsetY = (rand() % 7 - 3);
            int radius = 8 - i;
            DrawCircle(flameBaseX + offsetX, flameBaseY - i * 5 + offsetY, radius, GetColor(255, 220, 100), TRUE);
        }
    }

    // 「クリックで進む」表示
    if (frame >= 180) {
        DrawFormatString(560, 600, GetColor(255, 255, 255), "クリックで進む");
    }
}
