#pragma once
#include "DxLib.h"

class CandleGauge {
private:
    float x, y;
    float width, height;
    float current;
    unsigned int color;

public:
    CandleGauge(float px, float py, float w, float h);
    void Update(float speed, bool isActive);
    void Draw();
};
