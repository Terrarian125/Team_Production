#pragma once
#include "SceneBase.h"

class GameOverScene : public SceneBase {
	int go00;
	int go01;
	int frame;
public:
	GameOverScene();
	~GameOverScene();
    SceneBase* Update() override;
    void Draw() override;
};
