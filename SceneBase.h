#pragma once
#include "DxLib.h"

class SceneBase {
public:
    virtual ~SceneBase() = default;
    virtual SceneBase* Update() = 0;
    virtual void Draw() = 0;
};
