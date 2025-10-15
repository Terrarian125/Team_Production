#include "TitleScene.h"
#include "LoadScene.h"

TitleScene::TitleScene() {
    ti = LoadGraph("Data/ti.png");
    prevMouse = false;
}
TitleScene::~TitleScene() {
    DeleteGraph(ti);
}
SceneBase* TitleScene::Update() {
    bool nowMouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    if (nowMouse && !prevMouse)
        return new LoadScene();
    prevMouse = nowMouse;
    return this;
}
void TitleScene::Draw() {
    DrawGraph(0, 0, ti, TRUE);
}
