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

    // ★ Wキーが押されている間、walkAnimを更新して揺らす
    if (nowW) {
        const float amplitude = 10.0f; // 揺れの最大幅（ピクセル）
        const float speed = 0.05f;     // 揺れの速さ

        // サインカーブで -amplitude から +amplitude の間の値に
        walkAnim = (int)(amplitude * std::sin(frame * speed));
    }
    else {
        // Wキーを離したら、徐々に揺れをゼロに戻す (アニメーションをフェードアウトさせるイメージ)
        if (walkAnim > 0) walkAnim--;
        else if (walkAnim < 0) walkAnim++;
        // 1ピクセル未満の揺れは無視して完全にゼロに
        if (std::abs(walkAnim) <= 1) walkAnim = 0;
    }


    // --- 12歩目でEキー待ち ---
    static int eKeyPressedFrame = -1; // Eキーが押されたフレームを記録
    if (stepCount >= 12 && !enemyVisible) {
        // DrawFormatString(640, 400, GetColor(255, 255, 255), "[E]"); // Drawに移動した方が自然
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
        // ... (省略: 敵フェード処理)
        frame++;
        if (frame < 60) enemyAlpha += 4; // フェードイン
        else if (frame > 180) enemyAlpha -= 4; // フェードアウト
        if (enemyAlpha < 0) {
            enemyAlpha = 0;
            enemyVisible = false;
        }
        if (enemyAlpha > 255) enemyAlpha = 255;
    }

    // frameのインクリメントは敵フェードの制御にも使われているので、
    // 敵フェードが終わった後でフレームカウントを進める（今回は全体で一つで問題なし）

    // --- Eキー押下から2秒後にウィンドウを閉じる ---
    if (eKeyPressedFrame != -1) {
        int elapsed = GetNowCount() - eKeyPressedFrame;
        if (elapsed >= 2000) { // 2000ミリ秒＝2秒 (元のコードの5秒は長すぎたので2秒に短縮)
            DxLib_End();
        }
    }

    // --- UI更新 ---
    button.Update();
    gauge.Update(0.05f, 0.08f, 0.15f, button.IsOn(), !button.IsMouseInside());
    if (gauge.IsEmpty()) return new GameOverScene();

    // 敵フェードの制御以外でframeを進める場所
    if (!enemyVisible) {
        frame++; // 敵フェード中でない場合もframeを進める
    }

    return this;
}


void PlayScene::Draw() {
    // ★ 描画オフセットとして walkAnim を使用
    int offsetY = walkAnim;

    // 通常背景
    DrawGraph(0, offsetY, (stepCount % 2 == 0 ? bg00 : bg01), TRUE);

    // --- イベント背景（フェードせず、発動済みはずっと残す）---
    for (int i = 0; i <= bgIndex; i++) {
        if (i >= 0 && i < (int)eventBG.size()) {
            DrawGraph(0, offsetY, eventBG[i], TRUE);
        }
    }

    // --- 敵描画 ---
    if (enemyVisible) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, enemyAlpha);
        DrawGraph(0, offsetY, enemyGraph, TRUE); // ★ オフセット適用
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // --- 12歩目でEキー待ちの表示 ---
    if (stepCount >= 12 && !enemyVisible) {
        // UIなのでオフセットは適用しない
        DrawFormatString(640, 400, GetColor(255, 255, 255), "[E]");
    }

    // --- UI ---
    button.Draw();
    gauge.Draw(button.IsOn(), true);
    // DrawFormatString(50, 50, GetColor(255, 255, 255), "Step: %d", stepCount);
}