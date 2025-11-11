#pragma once
#include "DxLib.h"

class Button3D {
private:
    float x, y, r;
    bool isOn;
    bool flameActive;
    int flameTimer;

public:
    Button3D(float px = 0, float py = 0, float pr = 50.0f);
    void Update();
    void Draw();
    bool IsOn() const { return isOn; }
    bool IsMouseInside() const;
};
