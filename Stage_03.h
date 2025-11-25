#pragma once
#include "SceneBase.h"
#include "Button3D.h"
#include "CandleGauge.h"
#include <vector>

class Stage_03 : public SceneBase {
private:
    Button3D button;
    CandleGauge gauge;

    int stepCount;
    int walkAnim;
    int frame;
    int eventState;
    int bgAlpha;
    int bgIndex;

    bool enemyVisible;
    int enemyAlpha;

    int bg00, bg01;
    std::vector<int> eventBG;
    int enemyGraph;
    int stepSE;

    // --- フェード用追加 ---
    int fadeAlpha; // 0～255
    int fadeSpeed; // アルファ変化速度

public:
    Stage_03();
    ~Stage_03();

    // 背景の揺れのための変数
    float bgShakeOffset;
    float bgShakeSpeed;
    float bgShakeMax;

    virtual SceneBase* Update() override;
    virtual void Draw() override;
};
