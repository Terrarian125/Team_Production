#include "ClearScene.h"
#include "TitleScene.h"

ClearScene::ClearScene() {
    cl = LoadGraph("Data/Clear.png");
}
ClearScene::~ClearScene() {
    DeleteGraph(cl);
}
SceneBase* ClearScene::Update() {
    if (CheckHitKey(KEY_INPUT_RETURN)) return new TitleScene();
    return this;
}
void ClearScene::Draw() {
    DrawGraph(0, 0, cl, TRUE);
}
