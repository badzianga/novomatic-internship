#pragma once
#include <array>

struct vec2 {
    float x, y;
};

struct triangle {
    std::array<vec2, 3> points;
};

// calculate normal to the triangle's edge
vec2 calulateNormal(const vec2 &p1, const vec2 &p2);

// calculate min and max scalars of the points projected on normal
void projectPoints(const triangle &t, const vec2 &normal, float &min, float &max);

// check if two triangles collide with each other
bool isColliding(const triangle& triangle1, const triangle& triangle2);
