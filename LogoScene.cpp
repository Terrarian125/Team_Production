#include "LogoScene.h"
#include "TitleScene.h"

LogoScene::LogoScene() {
    lg = LoadGraph("Data/lg.png");
    frame = 0;
}
LogoScene::~LogoScene() {
    DeleteGraph(lg);
}
SceneBase* LogoScene::Update() {
    frame++;
    if (frame > 300) return new TitleScene();
    return this;
}
void LogoScene::Draw() {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, frame / 1.4);
    DrawGraph(0, 0, lg, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
