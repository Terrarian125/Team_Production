#include "Button3D.h"
#include <cmath>
#include <cstdlib>

Button3D::Button3D(float px, float py, float pr)
    : x(px), y(py), r(pr), isOn(false),
    flameTimer(0), flameActive(false) {
}

bool Button3D::IsMouseInside() const {
    int mx, my;
    GetMousePoint(&mx, &my);
    float dx = mx - x;
    float dy = my - y;
    return (dx * dx + dy * dy <= r * r);
}

void Button3D::Update() {
    static bool pressed = false;
    static bool prevInside = true;

    bool inside = IsMouseInside();

    // --- クリックでON/OFF切り替え ---
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) && inside) {
        if (!pressed) {
            isOn = !isOn;
            pressed = true;
        }
    }
    else {
        pressed = false;
    }

    // --- マウスが出た瞬間に燃焼エフェクト開始 ---
    if (prevInside && !inside) {
        flameActive = true;
        flameTimer = 60; // 約1秒ほど燃える
    }

    prevInside = inside;

    // --- 炎の寿命を減らす ---
    if (flameActive) {
        if (--flameTimer <= 0) {
            flameActive = false;
            flameTimer = 0;
        }
    }
}

void Button3D::Draw() {
    int segments = 64;
    int steps = 6;

    // --- ボタン本体 ---
    for (int i = 0; i < steps; ++i) {
        float ratio = 1.0f - (float)i / steps;
        int color = isOn
            ? GetColor(255, (int)(180 * ratio + 60 * (1 - ratio)), 60)
            : GetColor((int)(60 * ratio), (int)(80 * ratio), (int)(120 * ratio));
        DrawCircleAA(x, y, r - i * 2, segments, color, TRUE);
    }

    // --- 通常時の発光 ---
    if (isOn) {
        float glow = 4.0f * sinf(GetNowCount() / 200.0f);
        DrawCircleAA(x, y, r + 5 + glow, segments, GetColor(255, 240, 150), FALSE);
    }

    // --- ?? 燃焼エフェクト ---
    if (flameActive) {
        int count = 15;  // 炎粒の数
        float intensity = (float)flameTimer / 60.0f; // 残り寿命に比例
        for (int i = 0; i < count; ++i) {
            float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
            float dist = r + (rand() % 30) * intensity;
            float fx = x + cosf(angle) * dist;
            float fy = y + sinf(angle) * dist - (rand() % 10);
            int size = (int)(5 + rand() % 6);
            int flameColor = GetColor(255, 200 + rand() % 55, 100);
            DrawCircleAA(fx, fy, size * intensity, segments, flameColor, TRUE);
        }
    }
}
