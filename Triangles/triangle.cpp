#include "triangle.h"

vec2 calulateNormal(const vec2 &p1, const vec2 &p2) {
    vec2 edge = { p2.x - p1.x, p2.y - p1.y };
    vec2 normal = { -edge.y, edge.x };
    return normal;
}

void projectPoints(const triangle &t, const vec2 &normal, float &min, float &max) {
    min = t.points[0].x * normal.x + t.points[0].y * normal.y;
    max = min;
    for (int i = 1; i < 3; i++) {
        float scalar = t.points[i].x * normal.x + t.points[i].y * normal.y;
        if (scalar < min) {
            min = scalar;
        }
        else if (scalar > max) {
            max = scalar;
        }
    }
}

bool isColliding(const triangle& triangle1, const triangle& triangle2) {
    for (int i = 0; i < 3; i++) {
        vec2 normal = calulateNormal(triangle1.points[i], triangle1.points[(i + 1) % 3]);

        float min1, max1, min2, max2;
        projectPoints(triangle1, normal, min1, max1);
        projectPoints(triangle2, normal, min2, max2);

        if (max1 < min2 || max2 < min1) {
            return false;
        }
    }

    for (int i = 0; i < 3; i++) {
        vec2 normal = calulateNormal(triangle2.points[i], triangle2.points[(i + 1) % 3]);

        float min1, max1, min2, max2;
        projectPoints(triangle1, normal, min1, max1);
        projectPoints(triangle2, normal, min2, max2);

        if (max1 < min2 || max2 < min1) {
            return false;
        }
    }

    return true;
}
