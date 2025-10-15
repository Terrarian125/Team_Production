#include "DxLib.h"
#include "LogoScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(1200, 675, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    SceneBase* scene = new LogoScene();

    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        SceneBase* next = scene->Update();
        if (next != scene) {
            delete scene;
            scene = next;
        }
        scene->Draw();

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
