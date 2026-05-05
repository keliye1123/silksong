#include "file.h"
#include "function.h"

#define Y_SPEED 30   //跳的初始速度
#define X_SPEED 6    //左右移动速度
#define  X_DASH 18   //左右冲刺速度
#define DOWN_SPEED 20 //匀速下落的速度
#define G 1          //重力加速度
#define DASH_CD 60        //冲刺CD:60帧
#define ATTACK_CD 30     //攻击CD
#define CONTINUE 30  //冲刺持续播放动画：30帧
#define CONTINUE_ATTACK 20  //攻击动画持续时间：20帧
#define DAMAGE_TIME     36  //受到攻击持续时间
#define DAMAGE_TIME_CD     60  //受到攻击CD
#define ATTACK_GAP      80  //两次攻击间隔小于多少时，触发攻击二段
#define DEATH_TIME      60  //死亡动画持续时间



extern int hero_dirct;          //默然方向
extern int x_speed;
extern int y_speed ;
extern bool jump_flag;
extern bool dash_flag;
extern bool attack_flag;
extern bool double_flag;
extern bool cnt_flag;
extern bool Attack_hero_flag;
extern bool LEFT;
extern int wait_attack_hero;
extern int cnt;
extern int wait_dash;
extern int wait_attack;
extern int cont;
extern bool Attack_hero;
extern bool Attack_boss;
extern int hero_hp;
extern int death_time;
extern bool lose;
extern bool boss_damage_flag;


void control(Animation &hero,Area &ground,Animation &Effect ,Animation &Effect_Attack,Animation &boss,Animation &Damage_Effect,Sound &damage_music,Sound &sword,Sound & sword_hit) {
    //备注
    /*
    * //hero.number
    //0 向右待机
    //1 向左待机
    //2 向右走路
    //3 向左走路
    //4 右跳开始
    //5 右跳循环
    //6 右跳降落
    //7 左跳开始
    //8 左跳循环
    //9 左跳降落
    //10 右冲
    //11 左冲
    //12 右攻击 1段
    //13 左攻击 1段
    //14 右攻击 2段
    //15 左攻击 2段
    //16 右跳跃攻击
    //17 左跳跃攻击
    //18 右下落攻击
    //19 左下落攻击
    //20 右受到攻击
    //21 左受到攻击
    //22 死亡


    //effect.number
    //0 右冲特效
    //1 左冲特效

    //Effect_Attack.number
    //0 右攻击 1段特效
    //1 左攻击 1段特效
    //2 右攻击 2段特效
    //3 左攻击 2段特效

     */

    //输入控制
    //控制左右方向与walk速度
    if (GetAsyncKeyState('D')) {
            x_speed = X_SPEED;
        hero_dirct = 0;
    }
    else if (GetAsyncKeyState('A') ) {
            x_speed = -X_SPEED;
        hero_dirct = 1;
    }
    else {
        x_speed = 0;
    }

    //检测开始冲刺
    if (GetAsyncKeyState('X') && wait_dash == 0 && cont == 0) {
        dash_flag = true;
        wait_dash = DASH_CD;
        cont = CONTINUE;
        hero.index = 0;
        Effect.index = 0;       //特效回到最初帧
    }

    //检测攻击
    if (GetAsyncKeyState('J') ) {

        if (wait_attack == 0 && cont == 0 &&cnt_flag == false) {
            if (hero_dirct == 0) {
                Effect_Attack.Set_Area(WIDTH/2.0, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            }
            else {
               Effect_Attack.Set_Area(WIDTH/2.7, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            }
            if (In_Area(boss,Effect_Attack)) {
                sword_hit.Play_Music();
            }
            else{
                sword.Play_Music();
            }
            attack_flag = true;
            cnt_flag = true;
            cnt = ATTACK_GAP;
            wait_attack = ATTACK_CD;
            cont = CONTINUE_ATTACK;
            hero.index = 0;
            Effect_Attack.index = 0;
        }
        else if (wait_attack == 0 && cont == 0 && cnt_flag == true) {
            if (hero_dirct == 0) {
                Effect_Attack.Set_Area(WIDTH/2.0, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            }
            else {
                Effect_Attack.Set_Area(WIDTH/2.7, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            }
            if (In_Area(boss,Effect_Attack)) {
                sword_hit.Play_Music();
            }
            else{
                sword.Play_Music();
            }
            double_flag = true;
            wait_attack = ATTACK_CD;
            cont = CONTINUE_ATTACK;
            hero.index = 0;
            Effect_Attack.index = 0;
        }
    }

    //触发攻击二段间隔倒计时
    if (cnt_flag == true) {
        cnt--;
        if (cnt == 0) {
            cnt_flag = false;
        }
    }

    //冲刺CD
    if (wait_dash > 0) {
        wait_dash--;
    }
    //攻击CD
    if (wait_attack > 0) {
        wait_attack--;
    }
    //攻击CD
    if (wait_attack_hero > 0) {
        wait_attack_hero--;
    }
    
    //冲刺控制速度
    if ( dash_flag == true) {
        if (hero_dirct == 0) {
            x_speed = X_DASH;
        }
        else {
            x_speed = -X_DASH;
        }
    }

    //控制进程
    if (cont > 0) {
        cont--;
        if (cont == 0) {
            dash_flag = false;
            attack_flag = false;
            double_flag = false;
            Attack_hero_flag = false;
        }
    }
    //检测开始跳跃
    if (GetAsyncKeyState(VK_SPACE) && jump_flag == false) {//监测到跳跃
        jump_flag = true;
        y_speed =  Y_SPEED;
    }

    //模拟跳跃过程
    if (y_speed > -DOWN_SPEED && jump_flag == true) {
        y_speed -= G;
    }
    else if (ground.a_y - hero.a_y <= (ground.a_h + hero.a_h)/2){
        hero.p_y = 0;   //着陆恢复初始高度
        Effect.p_y = 0;
        Effect_Attack.p_y = 0;
        Damage_Effect.p_y = 0;
        y_speed = 0;
        jump_flag = false;
    }


    //受到攻击所有进程结束
    if (Attack_hero == true && Attack_hero_flag == false && wait_attack_hero == 0) {
        hero_hp--;
        Attack_hero_flag = true;
        cont = DAMAGE_TIME;
        wait_attack_hero = DAMAGE_TIME_CD;
        if (hero.An_x < boss.An_x) {
            LEFT = true;
        }
        else {
            LEFT = false;
        }
        damage_music.Play_Music();
    }

    if (Attack_hero_flag == true) {
        x_speed = 0;
        y_speed = 0;
        jump_flag = false;
        dash_flag = false;
        attack_flag = false;
        double_flag = false;
        cnt_flag = false;
        cnt = 0 ;
        wait_dash = 0;
        wait_attack = 0;
        if (cont > 24) {
            if (LEFT) {
                x_speed = -5;
            }
            else {
                x_speed = 5;
            }
            y_speed = 10;
        }
        else if (cont <= 24 && cont > 12) {
            if (LEFT) {
                x_speed = -5;
            }
            else {
                x_speed = 5;
            }
            y_speed = -10;

        }
        else if (cont < 20 && cont > 0) {
            x_speed = 0;
            y_speed = -DOWN_SPEED;
        }
    }

    if (hero_hp == 0 && death_time == 0) {
        death_time = DEATH_TIME;
    }
    if (death_time > 0) {
        y_speed = 0;
        x_speed = 0;
        death_time--;
        if (death_time == 0) {
            lose = true;
            return;
        }
    }

    //左墙壁
    if (hero.An_x - hero.An_w/2<= 0 && hero_dirct == 1) {
        x_speed = 0;
    }

    if (hero.An_x + hero.An_w/2 >= WIDTH && hero_dirct == 0) {
        x_speed = 0;
    }

    //更新坐标
    hero.p_y -= y_speed;
    hero.p_x += x_speed;
    Effect.p_y -= y_speed;
    Effect.p_x += x_speed;
    Effect_Attack.p_y -= y_speed;
    Effect_Attack.p_x += x_speed;
    Damage_Effect.p_y -= y_speed;
    Damage_Effect.p_x += x_speed;

    Attack_boss = false;
    //绘制图片
    //收到攻击
    if (death_time > 0) {

        hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,22);
    }
    else if (Attack_hero_flag == true) {

        if (LEFT) {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,21);
            Damage_Effect.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,12,0);
        }
        else {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,20);
            Damage_Effect.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,12,0);
        }
    }
    //攻击二段时刻
    else if (cont > 0 && double_flag == true) {
        if (hero_dirct == 0) {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,hero_dirct+14);
            Effect_Attack.Play_Animation_Loop(WIDTH/2.0, HEIGHT*0.86,15,hero_dirct+2);
            Effect_Attack.Set_Area(WIDTH/2.0, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            if (In_Area(boss,Effect_Attack)) {
                Attack_boss = true;
            }
        }
        else {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,hero_dirct+14);
            Effect_Attack.Play_Animation_Loop(WIDTH/2.7, HEIGHT*0.86,15,hero_dirct+2);
            Effect_Attack.Set_Area(WIDTH/2.7, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            if (In_Area(boss,Effect_Attack)) {
                Attack_boss = true;
            }
        }
    }
    //攻击一段时刻
    else if (cont > 0 && attack_flag == true) {
        if (hero_dirct == 0) {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,hero_dirct+12);
            Effect_Attack.Play_Animation_Loop(WIDTH/2.0, HEIGHT*0.86,15,hero_dirct);
            Effect_Attack.Set_Area(WIDTH/2.0, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            if (In_Area(boss,Effect_Attack)) {
                Attack_boss = true;
            }
        }
        else {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,6,hero_dirct+12);
            Effect_Attack.Play_Animation_Loop(WIDTH/2.7, HEIGHT*0.86,15,hero_dirct);
            Effect_Attack.Set_Area(WIDTH/2.7, HEIGHT*0.86,WIDTH/10,HEIGHT/10);
            if (In_Area(boss,Effect_Attack)) {
                Attack_boss = true;
            }
        }
    }
    //冲刺时刻
    else if (cont > 0 && dash_flag == true) {
        if (hero_dirct == 0) {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+10);
            Effect.Play_Animation_Loop(WIDTH/2.8, HEIGHT*0.86,7,hero_dirct);
        }
        else {
            hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+10);
            Effect.Play_Animation_Loop(WIDTH/1.9, HEIGHT*0.86,7,hero_dirct);
        }
    }
    //跳跃时刻
    else if (jump_flag == true) {
        if (y_speed >= 0 && hero_dirct == 0) {
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+4);
        }
        else if (y_speed <= 0 && hero_dirct == 0){
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+5);
        }
        else if (y_speed >= 0 && hero_dirct == 1) {
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+6);
        }
        else if (y_speed <= 0 && hero_dirct == 1){
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+7);
        }
    }
    //非跳跃时刻
    else {
        if (hero_dirct == 0 && GetAsyncKeyState('D')) {
                hero.Play_Animation_Loop_A(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+2,3);
        }
        else if (hero_dirct == 1 && GetAsyncKeyState('A')) {
                hero.Play_Animation_Loop_A(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct+2,3);
        }
        else if (hero_dirct == 0) {
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct);
        }
        else if (hero_dirct == 1) {
                hero.Play_Animation_Loop(WIDTH/2.25, HEIGHT*0.86,8,hero_dirct);
        }
    }


}