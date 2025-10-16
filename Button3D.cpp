#include "Button3D.h"
#include <cmath>

Button3D::Button3D(float px, float py, float pr)
    : x(px), y(py), r(pr), isOn(false) {
}

bool Button3D::IsMouseInside() const {
    int mx, my;
    GetMousePoint(&mx, &my);
    float dx = mx - x;
    float dy = my - y;
    return (dx * dx + dy * dy <= r * r);
}

void Button3D::Update() {
    int mx, my;
    GetMousePoint(&mx, &my);

    static bool pressed = false;
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) && IsMouseInside()) {
        if (!pressed) { isOn = !isOn; pressed = true; }
    }
    else {
        pressed = false;
    }
}

void Button3D::Draw() {
    int segments = 64;
    int steps = 6;
    for (int i = 0; i < steps; ++i) {
        float ratio = 1.0f - (float)i / steps;
        int color = isOn
            ? GetColor(255, (int)(180 * ratio + 60 * (1 - ratio)), 60)
            : GetColor((int)(60 * ratio), (int)(80 * ratio), (int)(120 * ratio));
        DrawCircleAA(x, y, r - i * 2, segments, color, TRUE);
    }

    if (isOn) {
        float glow = 4.0f * sinf(GetNowCount() / 200.0f);
        DrawCircleAA(x, y, r + 5 + glow, segments, GetColor(255, 240, 150), FALSE);
    }
}
