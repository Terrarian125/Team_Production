#pragma once
#include "SceneBase.h"
#include "Button3D.h"
#include "CandleGauge.h"
#include <vector>

struct EnemyEvent {
    int stepTrigger;  // 何歩目で出すか
    int graph;        // 敵画像ハンドル
    int x, y;         // 描画座標
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

    std::vector<EnemyEvent> enemies; // 敵リスト
    int currentEnemyIndex;            // 現在出現中の敵

public:
    PlayScene();
    ~PlayScene();
    SceneBase* Update() override;
    void Draw() override;
};
