#include "GameOverScene.h"
#include "TitleScene.h"

SceneBase* GameOverScene::Update() {
    if (CheckHitKey(KEY_INPUT_RETURN)) return new TitleScene();
    return this;
}
void GameOverScene::Draw() {
    DrawString(400, 300, "=== Game Over... ===", GetColor(255, 0, 0));
    DrawString(400, 350, "Press ENTER to Return to Title", GetColor(255, 255, 255));
}
