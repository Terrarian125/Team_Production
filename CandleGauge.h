#pragma once
#include "DxLib.h"

class CandleGauge {
private:
    float x, y;
    float width, height;
    float current;

public:
    CandleGauge(float px, float py, float w, float h);
    void Update(float offSpeed, float onSpeed, float awaySpeed, bool isOn, bool isMouseAway);
    void Draw(bool isOn, bool reverse = false); // reverse=trueÇ≈ç∂Å®âEÇ…å∏è≠
};
