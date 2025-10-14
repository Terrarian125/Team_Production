#include "Button3D.h"

void Button3D::Create(int px, int py, int radius) {
    x = px;
    y = py;
    r = radius;
    isOn = false;
}

void Button3D::Update() {
    int mx, my;
    GetMousePoint(&mx, &my);
    bool nowClick = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    int dx = mx - x;
    int dy = my - y;
    bool inside = (dx * dx + dy * dy) <= r * r;

    if (inside && nowClick) isOn = true;
    else isOn = false;
}

void Button3D::Draw() {
    unsigned int color = isOn ? GetColor(255, 100, 100) : GetColor(150, 150, 255);
    DrawCircle(x, y, r, color, TRUE);
    DrawCircle(x, y, r, GetColor(0, 0, 0), FALSE);
}
