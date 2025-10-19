#pragma once
#include "SceneBase.h"
#include "CandleGauge.h"

class LoadScene : public SceneBase {
    int lo;
    int frame;
    bool prevMouse;
    CandleGauge gauge;

public:
    LoadScene();
    ~LoadScene();
    SceneBase* Update() override;
    void Draw() override;
};
