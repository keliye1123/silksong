#include <bits/stdc++.h>
#include "file.h"
#include "graphics.h"
#include "menu.h"
#include "scene_1.h"
#include "script.h"
using namespace std;

int main() {

    initgraph(WIDTH,HEIGHT);
    setbkcolor(BLACK);
    cleardevice();
    cout << WIDTH << " " << HEIGHT << endl;
    while (1) {
    menu();
    }

    system("pause");

}