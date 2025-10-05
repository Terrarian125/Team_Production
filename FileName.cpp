#include "DxLib.h"
#include <math.h>

// シーンの種類
enum Scene {
    SCENE_LOGO,
    SCENE_TITLE,
    SCENE_LOAD,
    SCENE_PLAY,
    SCENE_CLEAR,
    SCENE_GAMEOVER
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DXライブラリ初期化
    ChangeWindowMode(TRUE);
    SetGraphMode(1200, 844, 32);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    // リソース読み込み
    int bg = LoadGraph("Data\\bg.png");                  // 背景
    if (bg == -1) {
        DxLib_End();
        return -1; // 画像読み込み失敗時は終了
	}
	int lg = LoadGraph("Data\\lg.png");                       // ロゴ
    int walkSE = LoadSoundMem("footstep.wav");     // 足音

    Scene currentScene = SCENE_LOGO;
    int frameCount = 0;        // ロゴ時間管理
    int stepCount = 0;         // 歩数カウント
    int lightX = 400, lightY = 300; // 懐中電灯の中心座標

    // メインループ
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        switch (currentScene) {
        case SCENE_LOGO:
            DrawString(200, 250, "ロゴシーン", GetColor(255, 255, 255));
            DrawGraph(0, 0, lg, TRUE);
            frameCount++;
            if (frameCount > 360) { // 6秒後にタイトルへ
                currentScene = SCENE_TITLE;
                frameCount = 0;
            }
            break;

        case SCENE_TITLE:
            DrawString(200, 200, "===漢試し===", GetColor(255, 255, 0));
            DrawString(200, 300, "Press [F] to Start", GetColor(255, 255, 255));
            if (CheckHitKey(KEY_INPUT_F)) {
                currentScene = SCENE_LOAD;
            }
            break;

        case SCENE_LOAD:
            DrawString(200, 200, "=== Now Loading ===", GetColor(0, 255, 255));
            DrawString(200, 300, "Press W to Walk", GetColor(255, 255, 255));
            DrawString(200, 330, "ESC to Exit", GetColor(255, 255, 255));
            DrawString(200, 400, "Press  [P] to Play", GetColor(255, 255, 255));
            if (CheckHitKey(KEY_INPUT_P)) {
                currentScene = SCENE_PLAY;
            }
            break;

        case SCENE_PLAY: {
    // ESCキーでゲームオーバー
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        currentScene = SCENE_GAMEOVER;
        break;
    }

    // --- 歩行アニメーション用 ---
    static int walkAnimFrame = 0;  // アニメーションカウンタ
    int camOffsetY = 0;

    // アニメーション中は上下動
    if (walkAnimFrame > 0) {
        // sin波っぽい上下動 (1回で上下して戻る)
        camOffsetY = (int)(sin((walkAnimFrame / 10.0) * DX_PI) * 5); 
        walkAnimFrame--;
    }

    // --- 描画 ---
    DrawGraph(0, camOffsetY, bg, TRUE);

    // --- 入力処理 ---
    static int prevW = 0;
    int nowW = CheckHitKey(KEY_INPUT_W);
    if (nowW && !prevW) {
        stepCount++;
        walkAnimFrame = 10; // 10フレームの歩行アニメ開始

        // 足音（左右交互）
        if (stepCount % 2 == 0) SetPanSoundMem(walkSE, -100); // 左
        else SetPanSoundMem(walkSE, 100);                     // 右
        PlaySoundMem(walkSE, DX_PLAYTYPE_BACK);
    }
    prevW = nowW;

    // --- クリア判定 ---
    if (stepCount >= 100) {
        currentScene = SCENE_CLEAR;
    }

    // デバッグ用
    DrawFormatString(50, 50, GetColor(255, 255, 255), "Step: %d", stepCount);
    break;
}


        case SCENE_CLEAR:
            DrawString(200, 250, "=== Clear!! ===", GetColor(0, 255, 0));
            DrawString(200, 350, "Press ENTER to Return to Title", GetColor(255, 255, 255));
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                currentScene = SCENE_TITLE;
            }
            break;

        case SCENE_GAMEOVER:
            DrawString(200, 250, "=== Game Over... ===", GetColor(255, 0, 0));
            DrawString(200, 350, "Press ENTER to Return to Title", GetColor(255, 255, 255));
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                currentScene = SCENE_TITLE;
            }
            break;
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
