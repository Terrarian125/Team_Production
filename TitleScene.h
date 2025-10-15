#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase {
    int ti;
    bool prevMouse;
public:
    TitleScene();
    ~TitleScene();
    SceneBase* Update() override;
    void Draw() override;
};
