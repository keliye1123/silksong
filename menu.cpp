
#include "file.h"
#include "scene_1.h"
using namespace std;

void menu() {
    //加载图片
    Sprite openground,title_chinese,TeamCherryLogo_large,press;
    openground.Load_Sprite(WIDTH,HEIGHT,"UI/openground.png");
    title_chinese.Load_Sprite(WIDTH/2,HEIGHT/3,"UI/title_chinese.png");
    TeamCherryLogo_large.Load_Sprite(WIDTH/10,HEIGHT/10,"UI/Team Cherry Logo_large.png");
    press.Load_Sprite(WIDTH/10,HEIGHT/40,"UI/press_to_start.png");


    //声明音乐变量
    Sound m1,Confirm;
    m1.Load_Music("UI/Title.mp3",-1);
    Confirm.Load_Music("UI/Confirm.mp3",-1);

    //加载动画
    Animation a1,a2;
    a1.Load_Animation("UI/Arror",11,HEIGHT/40,HEIGHT/40);
    a2.Load_Animation("UI/ArrorR",11,HEIGHT/40,HEIGHT/40);



    //播放背景音乐
    m1.Play_Bgm();

    bool Open = false;
    while (true) {
        clock_t StartTime = clock();



        GetCursorPos(&pt);//鼠标坐标监测
        press.Set_Area(WIDTH/2,HEIGHT*0.8,WIDTH/10,HEIGHT/40);//区域检测

        if (press.Mouse_In_Area_Rectangle() && GetAsyncKeyState(VK_LBUTTON)) {//点击开始游戏检测
            m1.Close_Bgm();
            Confirm.Play_Music();
            Open = true;
        }

        //绘图区
        BeginBatchDraw();

        openground.Draw_Sprite(WIDTH/2,HEIGHT/2);
        title_chinese.Draw_Sprite(WIDTH/2,HEIGHT/4);
        TeamCherryLogo_large.Draw_Sprite(WIDTH*0.85,HEIGHT*0.8);
        press.Draw_Sprite(WIDTH/2,HEIGHT*0.8);


           if (Open) {
               a1.Play_Animation_Simple(WIDTH/2.25, HEIGHT*0.8,4,1,0);
               a2.Play_Animation_Simple(WIDTH/1.8, HEIGHT*0.8,4,1,0);
           }

        // press.Draw_Area_Rectangle();

        EndBatchDraw();

        if (a1.Close_signal()) {
            Sleep(25);
            while (scene_1()) {
            }
            return;
        }

        //控制帧率
        clock_t EndTime = clock();
        if (EndTime - StartTime < FPS) Sleep(FPS - (EndTime - StartTime));
        // EndTime = clock();
        // cout << EndTime - StartTime << endl;
    }
}