//https://github.com/Terrarian125/Team_Production
#include "DxLib.h"
#include "LogoScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);
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
