#pragma once
#include "SceneBase.h"

class LogoScene : public SceneBase {
    int lg;
    int frame;
public:
    LogoScene();
    ~LogoScene();
    SceneBase* Update() override;
    void Draw() override;
};
