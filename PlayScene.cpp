#include "PlayScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include <cmath>
#include <DxLib.h>

PlayScene::PlayScene()
    : button(640, 550, 30), gauge(390, 600, 500, 25),
    stepCount(0), walkAnim(0), frame(0),
    eventState(0), bgAlpha(0), bgIndex(-1),
    enemyVisible(false), enemyAlpha(0)
{
    // 通常背景
    bg00 = LoadGraph("Data/road1 light.png");
    bg01 = LoadGraph("Data/road2 light.png");

    // イベント用背景（vector に格納）
    for (int i = 10; i <= 13; i++) {
        char path[64];
        sprintf_s(path, "Data/bg%d.png", i);
        eventBG.push_back(LoadGraph(path)); // ← 修正済み
    }

    enemyGraph = LoadGraph("Data/enemy00.png");
    stepSE = LoadSoundMem("Data/step.mp3");
}

PlayScene::~PlayScene() {
    DeleteGraph(bg00);
    DeleteGraph(bg01);
    for (auto g : eventBG) DeleteGraph(g);
    DeleteGraph(enemyGraph);
    DeleteSoundMem(stepSE);
}

SceneBase* PlayScene::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE)) return new GameOverScene();

    static int prevW = 0;
    int nowW = CheckHitKey(KEY_INPUT_W);

    // 敵出現中だけ操作禁止
    bool canControl = !enemyVisible;

    // --- 歩行入力 ---
    if (canControl && nowW && !prevW) {
        stepCount++;
        PlaySoundMem(stepSE, DX_PLAYTYPE_BACK);

        // 歩数に応じてイベント背景を追加（フェードなしで残る）
        if (stepCount >= 10 && stepCount <= 12) {
            bgIndex = stepCount - 10; // bg10～bg12
        }
    }
    prevW = nowW;

    // --- 12歩目でEキー待ち ---
    static int eKeyPressedFrame = -1; // Eキーが押されたフレームを記録
    if (stepCount >= 12 && !enemyVisible) {
        DrawFormatString(640, 400, GetColor(255, 255, 255), "[E]");
        if (CheckHitKey(KEY_INPUT_E)) {
            // Eキーで敵を出す＆bg13を表示
            enemyVisible = true;
            enemyAlpha = 0;
            frame = 0;

            bgIndex = 3; // bg13に切り替え（vectorの4枚目）
            eKeyPressedFrame = GetNowCount(); // 押された時間を記録
        }
    }

    // --- 敵フェード ---
    if (enemyVisible) {
        frame++;
        if (frame < 60) enemyAlpha += 4; // フェードイン
        else if (frame > 180) enemyAlpha -= 4; // フェードアウト
        if (enemyAlpha < 0) {
            enemyAlpha = 0;
            enemyVisible = false;
        }
        if (enemyAlpha > 255) enemyAlpha = 255;
    }
    // --- Eキー押下から5秒後にウィンドウを閉じる ---
    if (eKeyPressedFrame != -1) {
        int elapsed = GetNowCount() - eKeyPressedFrame;
        if (elapsed >= 2000) { // 5000ミリ秒＝5秒
            DxLib_End();
        }
    }

    // --- UI更新 ---
    button.Update();
    gauge.Update(0.05f, 0.08f, 0.15f, button.IsOn(), !button.IsMouseInside());
    if (gauge.IsEmpty()) return new GameOverScene();

    return this;
}


void PlayScene::Draw() {
    // 通常背景
    DrawGraph(0, 0, (stepCount % 2 == 0 ? bg00 : bg01), TRUE);

    // --- イベント背景（フェードせず、発動済みはずっと残す）---
    for (int i = 0; i <= bgIndex; i++) {
        if (i >= 0 && i < (int)eventBG.size()) {
            DrawGraph(0, 0, eventBG[i], TRUE);
        }
    }

    // --- 敵描画 ---
    if (enemyVisible) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, enemyAlpha);
        DrawGraph(0, 0, enemyGraph, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ← 引数修正済み
    }

    // --- UI ---
    button.Draw();
    gauge.Draw(button.IsOn(), true);
   // DrawFormatString(50, 50, GetColor(255, 255, 255), "Step: %d", stepCount);
}
