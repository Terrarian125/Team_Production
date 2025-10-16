#include "CandleGauge.h"
#include <cmath>
#include <cstdlib>

CandleGauge::CandleGauge(float px, float py, float w, float h)
    : x(px), y(py), width(w), height(h), current(w) {
}

void CandleGauge::Update(float offSpeed, float onSpeed, float awaySpeed, bool isOn, bool isMouseAway) {
    if (isOn && isMouseAway) current -= awaySpeed;
    else if (isOn) current -= onSpeed;
    else current -= offSpeed;

    if (current < 0.0f) current = 0.0f;
    if (current > width) current = width;
}

void CandleGauge::Draw(bool isOn, bool reverse) {
    DrawBox((int)x, (int)y, (int)(x + width), (int)(y + height), GetColor(40, 40, 40), TRUE);

    unsigned int barColor = GetColor(255, 150, 0);

    if (reverse) {
        // ¶¨‰E‚ÉŒ¸­
        DrawBox((int)x, (int)y, (int)(x + current), (int)(y + height), barColor, TRUE);
    }
    else {
        // ‰E¨¶
        DrawBox((int)(x + width - current), (int)y, (int)(x + width), (int)(y + height), barColor, TRUE);
    }

    DrawBox((int)x, (int)y, (int)(x + width), (int)(y + height), GetColor(0, 0, 0), FALSE);

    if (current > 0 && isOn) {
        int flameBaseX = reverse ? (int)(x + current) : (int)(x + width - current);
        int flameBaseY = (int)y;

        int flameCount = 5;
        for (int i = 0; i < flameCount; ++i) {
            int offsetX = (rand() % 7 - 3);
            int offsetY = (rand() % 7 - 3);
            int radius = 8 - i;
            DrawCircle(flameBaseX + offsetX, flameBaseY - i * 5 + offsetY, radius, GetColor(255, 220, 100), TRUE);
        }
    }
}
