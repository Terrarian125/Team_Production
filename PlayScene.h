#pragma once
#include "SceneBase.h"
#include "Button3D.h"
#include "CandleGauge.h"
#include <vector>

struct EnemyEvent {
    int stepTrigger;  // �����ڂŏo����
    int graph;        // �G�摜�n���h��
    int x, y;         // �`����W
};

class PlayScene : public SceneBase {
    int bg;
	int goal;
    int stepSE;
    int stepCount;
    int walkAnim;
    int frame;
    bool enemyVisible;
    int enemyAlpha;

    Button3D button;
    CandleGauge gauge;

    std::vector<EnemyEvent> enemies; // �G���X�g
    int currentEnemyIndex;            // ���ݏo�����̓G

public:
    PlayScene();
    ~PlayScene();
    SceneBase* Update() override;
    void Draw() override;
};
