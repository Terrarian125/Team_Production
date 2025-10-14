#define NOMINMAX
#include "CandleGauge.h"
#include <cmath>    // sinŠÖ”—p
#include <cstdlib>  // randŠÖ”—p

CandleGauge::CandleGauge(float px, float py, float w, float h)
    : x(px), y(py), width(w), height(h), current(w),
    color(GetColor(255, 150, 0)) {
}

void CandleGauge::Update(float speed, bool isActive) {
    if (isActive) {
        current -= speed;
        if (current < 0) current = 0;
    }
}

void CandleGauge::Draw() {
    // ”wŒi
    DrawBox(x - width, y, x, y + height, GetColor(60, 60, 60), TRUE);

    // ƒQ[ƒW
    DrawBox(x - current, y, x, y + height, color, TRUE);

    // ˜gü
    DrawBox(x - width, y, x, y + height, GetColor(0, 0, 0), FALSE);

    // ƒQ[ƒWã‚Ì‰Ši‚ä‚ç‚¬j
    if (current > 0) {
        int flameHeight = height + 10;
        int flameWidth = 10;

        int flameX = int(x - current + flameWidth / 2);
        int flameY = int(y - flameHeight / 2);

        // 2–{‚Ì‰Š‚ğ‚ä‚ç‚¬‚³‚¹‚é
        for (int i = 0; i < 3; i++) {
            int offsetX = (rand() % 7) - 3;  // -3`3
            int offsetY = int(sin(GetNowCount() * 0.005 + i) * 5);
            DrawCircle(flameX + offsetX, flameY + offsetY, flameWidth, GetColor(255, 220, 100), TRUE);
        }
    }
}
