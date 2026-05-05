//
// Created by 25738 on 2026/4/19.
//

#ifndef HK_GAME_FILE_H
#define HK_GAME_FILE_H

#include <bits/stdc++.h>
#include "easyx/include/easyx.h"
#include "tools.hpp"
#include <windows.h>
using namespace std;

//声明鼠标变量
inline POINT pt;

//窗口大小
inline int WIDTH = GetSystemMetrics(SM_CXSCREEN);
inline int HEIGHT = GetSystemMetrics(SM_CYSCREEN);
// inline int WIDTH = 1280;
// inline int HEIGHT = 720;

//帧率
inline clock_t FPS = 1000/60;


//加载资源类
class Resource {

};


//坐标点类
class Point {

    //属性
public:
    int p_x = 0, p_y = 0;      //在图形窗口中的坐标

    //方法
public:
    void Set_Point(const int x,const int y) {     //设置坐标
        p_x = x;
        p_y = y;
    }

    void move() {
        p_x -= 1;
        p_y -= 1;
    }

};


//区域检测类
class Area :public Point {

    //属性
public:
    int a_x = 0,a_y = 0,a_w = 0, a_h = 0;       //以a_x,a_y为中心的区域的宽与高

    //方法
public:
   void Set_Area(const int x,const int y,const int w,const int h) {       //设置和更新区域的坐标，宽与高
       a_x = x + p_x;
       a_y = y + p_y;
       a_w = w;
       a_h = h;
   }

    void Draw_Area_Rectangle() const {                      //显示矩形区域范围
       setlinestyle(PS_SOLID);
       setlinecolor(RED);
       rectangle(a_x - a_w/2,a_y - a_h/2,a_x + a_w/2,a_y + a_h/2);
   }

    void Draw_Area_Circle() const {                        //显示圆形区域范围，宽度为圆的直径
       setlinestyle(PS_SOLID,1);
       setlinecolor(BLUE);//显示圆形区域
       circle(a_x,a_y,a_w/2);
   }

    void Draw_Area_Ellipse() const {                      //显示椭圆区域范围
       setlinestyle(PS_SOLID,1);
       setlinecolor(BLUE);//显示椭圆区域
       ellipse(a_x - a_w/2,a_y - a_h/2,a_x + a_w/2,a_y + a_h/2);
   }

    [[nodiscard]] bool Mouse_In_Area_Rectangle() const{                      //判断鼠标是否在矩形区域
       // 获取窗口在屏幕上的位置
       HWND hwnd = GetHWnd();
       RECT rect;
       GetWindowRect(hwnd, &rect);
        if (pt.x - rect.left >= a_x - a_w/2 && pt.x - rect.left <= a_x + a_w/2 && pt.y - rect.top - 24 >= a_y - a_h/2 && pt.y - rect.top - 24 <= a_y + a_h/2) return true;
        return false;
   }

    [[nodiscard]] bool Mouse_In_Area_Circle() const {                      //判断鼠标是否在圆形区域
       
       double dx = pt.x - a_x;
       double dy = pt.y - 24 - a_y;
       double aw = a_w;
       double rx = aw/2;
       if (dx*dx + dy*dy < rx*rx) return true;
        return false;
   }

    [[nodiscard]] bool Mouse_In_Area_Ellipse() const{                      //判断鼠标是否在椭圆区域

       double dx = pt.x - a_x;
       double dy = pt.y - 24 - a_y;
       double aw = a_w;
       double ah = a_h;
       double rx = aw/2;
       double ry = ah/2;
       if (dx*dx/(rx*rx) + dy*dy/(ry*ry) < 1) return true;
       return false;
   }

};

//图片类
class Sprite:public Area {

    //属性
protected:
    int s_x = 0,s_y = 0,s_w = 0,s_h = 0;     //以px,py为中心的图片的宽与高
    IMAGE photo;             // 图片名

    //方法
public:
    void Load_Sprite(const int w,const int h,const char* path) {     //设置图片宽与高并加载图片
        s_w = w;
        s_h = h;
        if (loadimage(&photo,path,s_w,s_h)) {           //检查错误
            cout << path<< " Error" << endl;

        }
    }

    void Draw_Sprite(const int x,const int y)  {                                      //绘制更新图片
        s_x = x + p_x;
        s_y = y + p_y;
        // putimage(p_x - s_w/2,p_y - s_h/2,&photo);
        drawImg(s_x - s_w/2,s_y - s_h/2,&photo);
    }


};


//动画类
class Animation:public Area {

public:
    //属性
    int An_x = 0,An_y = 0,An_w = 0,An_h = 0;      //设置图片宽与高
    int index = 0;                                //当前绘制图片的编号
    int gap = 0;                                  //与上一张绘制图片的间隔
    int times = -1;                               //非循环动画的播放次数
    vector<vector<IMAGE>> multiple_img;           //存储多组图片
    int turn = -1;                                //标记循环图片是否变化

    //方法
public:
    void Load_Animation(const string& paths,const int n,const int w,const int h) {     //加载图片
        An_w = w;
        An_h = h;
        string s1 = paths + "/%d.png";
        char s2[100];
        for (int i = 0; i <= s1.size(); i++) {
            s2[i] = s1[i];
        }
        char path[100];
        vector<IMAGE> temp1;
        for (int i = 0;i < n;i++) {
            sprintf(path,s2,i);
            IMAGE temp;
            loadimage(&temp,path,An_w,An_h);
            temp1.push_back(temp);
        }
        multiple_img.push_back(temp1);
    }


    void Play_Animation_Loop(const int x,const int y,const int gaps,const int number) {         //播放循环动画
        if (turn != number) {
            turn = number;
            index = 0;
            gap = 0;
        }
        An_x = x + p_x;
        An_y = y + p_y;
        // putimage(p_x - s_w/2,p_y - s_h/2,&photo);
        drawImg(An_x - An_w/2,An_y - An_h/2,&multiple_img[number][index]);
        gap++;
        if (gap >= gaps) {
            gap = 0;
            index++;
        }
        if (index >= multiple_img[number].size()) {
            index = 0;
        }
    }

    void Play_Animation_Loop_A(const int x,const int y,const int gaps,const int number,const int begin) {         //从选定位置播放循环动画
        if (turn != number) {
            turn = number;
            index = begin;
        }
        An_x = x + p_x;
        An_y = y + p_y;
        // putimage(p_x - s_w/2,p_y - s_h/2,&photo);
        drawImg(An_x - An_w/2,An_y - An_h/2,&multiple_img[number][index]);
        gap++;
        if (gap >= gaps) {
            gap = 0;
            index++;
        }
        if (index >= multiple_img[number].size()) {
            index = begin;
        }
    }

    void Play_Animation_Simple(const int x,const int y,const int gaps,const int cnt,const int number) {       //播放几次动画
        if (times == -1) {
            times = cnt;
            index = 0;
            gap = 0;
        }

        if (times > 0) {
            An_x = x + p_x;
            An_y = y + p_y;
            // putimage(p_x - s_w/2,p_y - s_h/2,&photo);
            drawImg(An_x - An_w/2,An_y - An_h/2,&multiple_img[number][index]);
            gap++;
            if (gap >= gaps) {
                gap = 0;
                index++;
            }
        }

        if (index >= multiple_img[number].size() && times != 0) {
            times--;
            index = 0;
        }

    }

    [[nodiscard]] bool Close_signal() const{       //动画播放完成信号
        if (times == 0) return true;
        return false;
    }

};

//音乐播放类
class Sound {
    //属性
    string Open = "open ";
    string Play = "play ";
    string Close = "close ";
    string Repeat;
    string Set;
    int times = -1;              //播放非循环音乐的次数
    char s1[100] = {};          //open
    char s2[100] = {};          //play
    char s3[100] = {};          //close
    char s4[100] = {};          //repeat
    char s5[100] = {};          //Set
    int flag = 0;

    //方法
public:
    void Play_Bgm()const {        //循环播放背景音乐
        mciSendString(s1,nullptr,0,nullptr);
        mciSendString(s4,nullptr,0,nullptr);
        if (flag != -1) {
            mciSendString(s5,nullptr,0,nullptr);
        }
    }

    void Load_Music(const string& paths,int v) {
        flag = v;
        Open = Open + paths;
        for (int i = 0;i <= Open.size();i++) {
            s1[i] = Open[i];
        }
        Repeat = Play + paths + " repeat";
        for (int i = 0;i <=Repeat.size();i++) {
            s4[i] = Repeat[i];
        }
        Play = Play + paths;
        for (int i = 0;i <= Play.size();i++) {
            s2[i] = Play[i];
        }
        Close = Close + paths;
        for (int i = 0;i <= Close.size();i++) {
            s3[i] = Close[i];
        }
        char s6[100] = {};
        sprintf(s6,"%d",v);
        Set = "setaudio " + paths + " volume to " + s6;
        for (int i = 0;i <= Set.size();i++) {
            s5[i] = Set[i];
        }
    }

    void Play_Music() const{       //播放非循环音乐
        mciSendString(s3,nullptr,0,nullptr);
        mciSendString(s1,nullptr,0,nullptr);
        mciSendString(s2,nullptr,0,nullptr);
        if (flag != -1) {
            mciSendString(s5,nullptr,0,nullptr);
        }

    }

    void Close_Bgm() const{       //关闭音乐
        mciSendString(s3,nullptr,0,nullptr);
    }

};


//分组类
class group {
public:
    //属性
    vector<Area> Group_Areas;

public:
    //方法
    void Enter_Group(const Area &a) {        //存入组
        Group_Areas.push_back(a);
    }

};

static group Ground;            //声明全局地板类


#endif //HK_GAME_FILE_H