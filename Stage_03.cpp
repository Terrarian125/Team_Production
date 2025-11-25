#include "Stage_03.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include <DxLib.h>
#include <cmath>
#include <cstdio>

Stage_03::Stage_03()
    : button(640, 550, 30), gauge(390, 600, 500, 25),
    stepCount(0), walkAnim(0), frame(0),
    eventState(0), bgAlpha(0), bgIndex(-1),
    enemyVisible(false), enemyAlpha(0),
    fadeAlpha(0), fadeSpeed(8),
    // 背景揺れ変数の初期化
    bgShakeOffset(0.0f), bgShakeSpeed(0.0f), bgShakeMax(5.0f)
{
    bg00 = LoadGraph("Data/Stage_03/road1 light.png");
    bg01 = LoadGraph("Data/Stage_03/road2 light.png");

    for (int i = 10; i <= 13; i++) {
        char path[64];
        sprintf_s(path, "Data/Stage_03/bg%d.png", i);
        eventBG.push_back(LoadGraph(path));
    }

    enemyGraph = LoadGraph("Data/Stage_03/enemy00.png");
    stepSE = LoadSoundMem("Data/Stage_03/step.mp3");
}

Stage_03::~Stage_03() {
    DeleteGraph(bg00);
    DeleteGraph(bg01);
    for (auto g : eventBG) DeleteGraph(g);
    DeleteGraph(enemyGraph);
    DeleteSoundMem(stepSE);
}

SceneBase* Stage_03::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE)) return new GameOverScene();

    // Wキーのクールタイム処理のための変数
    static int lastWPressTime = 0;
    int nowTime = GetNowCount();

    bool canControl = !enemyVisible;

    // Wキーが押されているか、かつ、前回押してから1000ms以上経過しているかを確認
    if (canControl && CheckHitKey(KEY_INPUT_W) && (nowTime - lastWPressTime >= 1000)) {
        stepCount++;
        PlaySoundMem(stepSE, DX_PLAYTYPE_BACK);

        if (stepCount >= 10 && stepCount <= 12) {
            bgIndex = stepCount - 10;
        }

        // Wキーを押した時刻を更新
        lastWPressTime = nowTime;

        // Wキーが押されたときに揺れを開始
        bgShakeOffset = -bgShakeMax;
        bgShakeSpeed = bgShakeMax / 10.0f;
    }

    // --- 背景の揺れを更新 ---
    if (bgShakeSpeed != 0.0f) {
        bgShakeOffset += bgShakeSpeed;

        // 上方向への揺れが最大に達したら反転
        if (bgShakeSpeed > 0 && bgShakeOffset >= bgShakeMax) {
            bgShakeOffset = bgShakeMax;
            bgShakeSpeed *= -1.0f;
        }
        // 下方向への揺れが最大に達したら反転
        else if (bgShakeSpeed < 0 && bgShakeOffset <= -bgShakeMax) {
            bgShakeOffset = -bgShakeMax;
            bgShakeSpeed *= -1.0f;
        }

        // 揺れがほぼ中央（0）に戻ったら停止
        // この条件は、揺れが上方向から下方向（bgShakeSpeed < 0）に切り替わって、
        // かつオフセットが中央付近（-1.0f から 1.0f）にあることをチェックしています。
        if (bgShakeSpeed < 0 && bgShakeOffset >= -1.0f && bgShakeOffset <= 1.0f) {
            bgShakeOffset = 0.0f;
            bgShakeSpeed = 0.0f;
        }
    }

    static int eKeyPressedFrame = -1;
    if (stepCount >= 12 && !enemyVisible) {
        DrawFormatString(640, 400, GetColor(255, 255, 255), "[E]");
        if (CheckHitKey(KEY_INPUT_E)) {
            enemyVisible = true;
            enemyAlpha = 0;
            frame = 0;
            bgIndex = 3; // bg13
            eKeyPressedFrame = GetNowCount();
        }
    }

    if (enemyVisible) {
        frame++;
        if (frame < 60) enemyAlpha += 4;
        else if (frame > 180) enemyAlpha -= 4;
        if (enemyAlpha < 0) {
            enemyAlpha = 0;
            enemyVisible = false;
        }
        if (enemyAlpha > 255) enemyAlpha = 255;
    }

    if (eKeyPressedFrame != -1) {
        int elapsed = GetNowCount() - eKeyPressedFrame;
        if (elapsed >= 2000) DxLib_End();
    }

    // --- UI 更新 ---
    button.Update();

    // --- 距離に応じた暗転 ---
    int mx, my;
    GetMousePoint(&mx, &my);
    float dx = mx - button.GetX();
    float dy = my - button.GetY();
    float dist = sqrtf(dx * dx + dy * dy);

    float maxDist = button.GetR() * 5.0f;
    fadeAlpha = (int)(255.0f * (dist / maxDist));
    if (fadeAlpha > 255) fadeAlpha = 255;
    if (fadeAlpha < 0) fadeAlpha = 0;

    // --- ゲージ更新 ---
    gauge.Update(0.05f, 0.08f, 0.15f, button.IsOn(), !button.IsMouseInside());
    if (gauge.IsEmpty()) return new GameOverScene();

    return this;
}

void Stage_03::Draw() {
    // --- 背景 ---
    // bgShakeOffset を加算して描画し、画像を上下に揺らす
    DrawGraph(0, (int)bgShakeOffset, (stepCount % 2 == 0 ? bg00 : bg01), TRUE);

    for (int i = 0; i <= bgIndex; i++) {
        if (i >= 0 && i < (int)eventBG.size()) DrawGraph(0, (int)bgShakeOffset, eventBG[i], TRUE);
    }

    // --- 敵 ---
    if (enemyVisible) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, enemyAlpha);
        DrawGraph(0, (int)bgShakeOffset, enemyGraph, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // --- UI 通常描画 ---
    gauge.Draw(button.IsOn(), true);

    // --- 暗転 ---
    if (fadeAlpha > 0) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        int highlightAlpha = 150;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, highlightAlpha);
        button.Draw();
        gauge.Draw(button.IsOn(), true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    else {
        button.Draw();
    }
}