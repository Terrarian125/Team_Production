#define NOMINMAX
#include "DxLib.h"
#include <cmath>
#include <ctime>
//#include <cstdlib>
#include <string>
#include "CandleGauge.h"
#include "Button3D.h"

// =====================
// �V�[���̎��
// =====================
enum Scene {
    SCENE_LOGO,
    SCENE_TITLE,
    SCENE_LOAD,
    SCENE_PLAY,
    SCENE_CLEAR,
    SCENE_GAMEOVER
};

// =====================
// ���C���֐�
// =====================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DX���C�u����������
    ChangeWindowMode(TRUE);
    SetGraphMode(1920, 1080, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    // ���\�[�X�ǂݍ���
    int bg = LoadGraph("Data\\bg.png");     // �w�i
    int lg = LoadGraph("Data\\lg.png");     // ���S
    int ti = LoadGraph("Data\\ti.png");     // �^�C�g��
    int lo = LoadGraph("Data\\Load.png");   // ���[�h
    int cl = LoadGraph("Data\\Clear.png");  // �N���A
    int walkSE = LoadSoundMem("step.mp3"); // ����

    //if (bg == -1 || lg == -1 || ti == -1 || lo == -1 || cl == -1 || walkSE == -1) {
    //    MessageBox(NULL, "���\�[�X�ǂݍ��݂Ɏ��s���܂����B", "Error", MB_OK);
    //    DxLib_End();
    //    return -1;
    //}


    Scene currentScene = SCENE_LOGO;
    int frameCount = 0;
    int stepCount = 0;

    // �v���C�V�[���p
    Button3D button;
    button.Create(100, 500, 40);

    CandleGauge gauge(750, 100, 200, 30);
    float decreaseSpeed = 0.2f;

    // ���C�����[�v
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        switch (currentScene) {
        case SCENE_LOGO: {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, frameCount / 1.4);
            DrawGraph(0, 0, lg, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            frameCount++;
            if (frameCount > 300) {
                currentScene = SCENE_TITLE;
                frameCount = 0;
            }
            break;
        }

        case SCENE_TITLE: {
            DrawGraph(0, 0, ti, TRUE);
            static bool prevMouseLeft = false;
            bool nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
            if (nowMouseLeft && !prevMouseLeft) currentScene = SCENE_LOAD;
            prevMouseLeft = nowMouseLeft;
            break;
        }

        case SCENE_LOAD: {
            static int loadFrameCount = 0;
            static bool prevMouseLeft = false;
            loadFrameCount++;
            DrawGraph(0, 0, lo, TRUE);

            bool nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
            if (loadFrameCount >= 180) {
                if (nowMouseLeft && !prevMouseLeft) {
                    currentScene = SCENE_PLAY;
                    loadFrameCount = 0;
                }
            }
            prevMouseLeft = nowMouseLeft;
            break;
        }

        case SCENE_PLAY: {
            // ESC�L�[�ŃQ�[���I�[�o�[
            if (CheckHitKey(KEY_INPUT_ESCAPE)) {
                currentScene = SCENE_GAMEOVER;
                break;
            }

            static int walkAnimFrame = 0;
            int camOffsetY = 0;
            if (walkAnimFrame > 0) {
                camOffsetY = (int)(sin((walkAnimFrame / 10.0) * DX_PI) * 5);
                walkAnimFrame--;
            }

            DrawGraph(0, camOffsetY, bg, TRUE);

            // --- ���͏��� ---
            static int prevW = 0;
            int nowW = CheckHitKey(KEY_INPUT_W);
            if (nowW && !prevW) {
                stepCount++;
                walkAnimFrame = 10;

                if (stepCount % 2 == 0)
                    SetPanSoundMem(walkSE, -100);
                else
                    SetPanSoundMem(walkSE, 100);

                PlaySoundMem(walkSE, DX_PLAYTYPE_BACK);
            }
            prevW = nowW;

            // --- �Q�[�W�ƃ{�^���X�V ---
            button.Update();
            gauge.Update(decreaseSpeed, button.GetState());

            button.Draw();
            gauge.Draw();

            DrawFormatString(50, 50, GetColor(255, 255, 255), "Step: %d", stepCount);

            if (stepCount >= 30) currentScene = SCENE_CLEAR;
            break;
        }

        case SCENE_CLEAR:
            DrawGraph(0, 0, cl, TRUE);
            if (CheckHitKey(KEY_INPUT_RETURN)) currentScene = SCENE_TITLE;
            break;

        case SCENE_GAMEOVER:
            DrawString(200, 250, "=== Game Over... ===", GetColor(255, 0, 0));
            DrawString(200, 350, "Press ENTER to Return to Title", GetColor(255, 255, 255));
            if (CheckHitKey(KEY_INPUT_RETURN)) currentScene = SCENE_TITLE;
            break;
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
