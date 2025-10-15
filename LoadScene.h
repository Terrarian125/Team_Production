#pragma once
#include "SceneBase.h"

class LoadScene : public SceneBase {
    int lo;
    int frame;
    bool prevMouse;
public:
    LoadScene();
    ~LoadScene();
    SceneBase* Update() override;
    void Draw() override;
};
