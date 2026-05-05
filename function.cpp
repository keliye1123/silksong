//
// Created by 25738 on 2026/5/3.
//
#include "file.h"

bool In_Area(Area &a,Area &b) {
    if (abs(a.a_x - b.a_x) < (a.a_w + b.a_w)/2 && abs(a.a_y - b.a_y) < (a.a_h + b.a_h)/2) {
        return true;
    }
    return false;
}
