#pragma once
#include "SceneBase.h"

class GameOverScene : public SceneBase {
	int go01;
	int go02;
	int frame;
public:
	GameOverScene();
	~GameOverScene();
    SceneBase* Update() override;
    void Draw() override;
};
