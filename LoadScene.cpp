#include "LoadScene.h"
#include "PlayScene.h"

LoadScene::LoadScene() {
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
    if (frame >= 180 && nowMouse && !prevMouse)
        return new PlayScene();
    prevMouse = nowMouse;
    return this;
}
void LoadScene::Draw() {
    DrawGraph(0, 0, lo, TRUE);
}
