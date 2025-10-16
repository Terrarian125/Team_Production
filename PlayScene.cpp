#include "PlayScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include <cmath>
#include <string>
#include <DxLib.h>

PlayScene::PlayScene()
	: button(640, 550, 30), gauge(390, 600, 500, 25), currentEnemyIndex(-1) 
{
    // ゲージの下中央に配置
    CandleGauge gauge(390, 600, 500, 25);
    // ボタンはゲージより上に配置
    Button3D button(640, 550, 30);

    bg = LoadGraph("Data/bg.png");
	goal = LoadGraph("Data/goal.png");
    stepSE = LoadSoundMem("Data/step.mp3");

    // 敵画像をまとめて読み込み座標も設定
    for (int i = 0; i <= 5; i++) { // 00～05まで
        char path[64];
        sprintf_s(path, "Data/enemy%02d.png", i);
        int g = LoadGraph(path);

        // 座標は適当に設定（あとで変更可能）
        int x = 200 + i * 50;
        int y = 100 + i * 30;

        enemies.push_back({ (i + 1) * 10, g, x, y }); // 10歩目に00, 20歩目に01...
    }
}

PlayScene::~PlayScene() {
    DeleteGraph(bg);
    DeleteSoundMem(stepSE);
    for (auto& e : enemies) DeleteGraph(e.graph);
}

SceneBase* PlayScene::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE)) return new GameOverScene();

    int camOffsetY = 0;
    if (walkAnim > 0) {
        camOffsetY = (int)(sin((walkAnim / 10.0) * DX_PI) * 5);
        walkAnim--;
    }

    static int prevW = 0;
    int nowW = CheckHitKey(KEY_INPUT_W);

    // 敵表示中は操作不可
    bool canControl = !enemyVisible;

    if (canControl && nowW && !prevW) {
        stepCount++;
        walkAnim = 10;
        SetPanSoundMem(stepSE, (stepCount % 2 == 0) ? -100 : 100);
        PlaySoundMem(stepSE, DX_PLAYTYPE_BACK);

        // 歩数に応じて敵出現判定
        for (size_t i = 0; i < enemies.size(); i++) {
            if (stepCount == enemies[i].stepTrigger) {
                enemyVisible = true;
                enemyAlpha = 0;
                frame = 0;
                currentEnemyIndex = (int)i;
            }
        }
    }
    prevW = nowW;

    // --- 敵フェード制御 ---
    if (enemyVisible && currentEnemyIndex >= 0) {
        frame++;
        if (frame < 60) enemyAlpha += 4;          // フェードイン
        else if (frame > 180) enemyAlpha -= 4;    // フェードアウト
        if (enemyAlpha < 0) enemyAlpha = 0;
        if (enemyAlpha > 255) enemyAlpha = 255;
        if (frame > 240) enemyVisible = false;
    }

    button.Update();
	gauge.Update(0.2f, 0.5f, 1.0f, button.IsOn(), !button.IsMouseInside());
    //gauge.Update(0.2f, button.GetState());

    if (stepCount >= 30) return new ClearScene();
    return this;
}

void PlayScene::Draw() {
    DrawGraph(0, 0, bg, TRUE);

    if (enemyVisible && currentEnemyIndex >= 0) {
        const EnemyEvent& e = enemies[currentEnemyIndex];
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, enemyAlpha);
        DrawGraph(e.x, e.y, e.graph, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    button.Draw();
	gauge.Draw(button.IsOn(), true);
    DrawFormatString(50, 50, GetColor(255, 255, 255), "Step: %d", stepCount);
}
