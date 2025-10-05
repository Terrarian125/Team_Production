#include "DxLib.h"
#include <math.h>

// �V�[���̎��
enum Scene {
    SCENE_LOGO,
    SCENE_TITLE,
    SCENE_LOAD,
    SCENE_PLAY,
    SCENE_CLEAR,
    SCENE_GAMEOVER
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DX���C�u����������
    ChangeWindowMode(TRUE);
    SetGraphMode(1200, 844, 32);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    // ���\�[�X�ǂݍ���
    int bg = LoadGraph("Data\\bg.png");                  // �w�i
    if (bg == -1) {
        DxLib_End();
        return -1; // �摜�ǂݍ��ݎ��s���͏I��
	}
	int lg = LoadGraph("Data\\lg.png");                       // ���S
    int walkSE = LoadSoundMem("footstep.wav");     // ����

    Scene currentScene = SCENE_LOGO;
    int frameCount = 0;        // ���S���ԊǗ�
    int stepCount = 0;         // �����J�E���g
    int lightX = 400, lightY = 300; // �����d���̒��S���W

    // ���C�����[�v
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        switch (currentScene) {
        case SCENE_LOGO:
            DrawString(200, 250, "���S�V�[��", GetColor(255, 255, 255));
            DrawGraph(0, 0, lg, TRUE);
            frameCount++;
            if (frameCount > 360) { // 6�b��Ƀ^�C�g����
                currentScene = SCENE_TITLE;
                frameCount = 0;
            }
            break;

        case SCENE_TITLE:
            DrawString(200, 200, "===������===", GetColor(255, 255, 0));
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
    // ESC�L�[�ŃQ�[���I�[�o�[
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        currentScene = SCENE_GAMEOVER;
        break;
    }

    // --- ���s�A�j���[�V�����p ---
    static int walkAnimFrame = 0;  // �A�j���[�V�����J�E���^
    int camOffsetY = 0;

    // �A�j���[�V�������͏㉺��
    if (walkAnimFrame > 0) {
        // sin�g���ۂ��㉺�� (1��ŏ㉺���Ė߂�)
        camOffsetY = (int)(sin((walkAnimFrame / 10.0) * DX_PI) * 5); 
        walkAnimFrame--;
    }

    // --- �`�� ---
    DrawGraph(0, camOffsetY, bg, TRUE);

    // --- ���͏��� ---
    static int prevW = 0;
    int nowW = CheckHitKey(KEY_INPUT_W);
    if (nowW && !prevW) {
        stepCount++;
        walkAnimFrame = 10; // 10�t���[���̕��s�A�j���J�n

        // �����i���E���݁j
        if (stepCount % 2 == 0) SetPanSoundMem(walkSE, -100); // ��
        else SetPanSoundMem(walkSE, 100);                     // �E
        PlaySoundMem(walkSE, DX_PLAYTYPE_BACK);
    }
    prevW = nowW;

    // --- �N���A���� ---
    if (stepCount >= 100) {
        currentScene = SCENE_CLEAR;
    }

    // �f�o�b�O�p
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
