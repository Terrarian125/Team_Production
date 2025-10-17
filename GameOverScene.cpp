#include "GameOverScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
	: frame(0)
{
	go00 = LoadGraph("Data/gameover00.png");
	go01 = LoadGraph("Data/gameover01.png");
}

GameOverScene::~GameOverScene()
{
	DeleteGraph(go00);
	DeleteGraph(go01);
}

SceneBase* GameOverScene::Update()
{
    if (CheckHitKey(KEY_INPUT_RETURN)) return new TitleScene();
    return this;
}
void GameOverScene::Draw() 
{
	if (frame < 300) {
		frame++;
		DrawGraph(0, 0, go00, TRUE);
		int alpha = (int)(255.0f * frame / 300.0f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1200, 675, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else {
		DrawGraph(0, 0, go01, TRUE);
	}
}
