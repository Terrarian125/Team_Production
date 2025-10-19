#pragma once
#include "SceneBase.h"
#include "Button3D.h"
#include "CandleGauge.h"
#include <vector>

struct EnemyEvent {
    int stepTrigger;
    int graph;
    int x, y;
};

class PlayScene : public SceneBase {
    int bg00, bg01;
    int enemyGraph;
    int stepSE;
    int stepCount;
    int walkAnim;
    int frame;

    int eventState;   // 0=fadein,1=hold,2=fadeout
    int bgAlpha;
    int bgIndex;      // 現在表示中のイベント背景index
    bool enemyVisible;
    int enemyAlpha;

    Button3D button;
    CandleGauge gauge;
    std::vector<int> eventBG;

public:
    PlayScene();
    ~PlayScene();
    SceneBase* Update() override;
    void Draw() override;
};
