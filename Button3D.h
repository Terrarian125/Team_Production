#pragma once
#include "DxLib.h"

class Button3D {
private:
    int x, y, r;
    bool isOn;

public:
    Button3D() : x(0), y(0), r(40), isOn(false) {}
    void Create(int px, int py, int radius);
    void Update();
    void Draw();
    bool GetState() const { return isOn; }
};
