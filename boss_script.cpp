#include "file.h"
#include "function.h"

#define G 1          //重力加速度
#define BOSS_CD_DASH 200   //冲刺技能CD
#define BOSS_CD_JUMP 300   //普通跳跃CD
#define BOSS_CD_JUMP_DASH 300   //冲刺跳跃CD
#define BOSS_CD_JUMP_FINAL 300   //finalCD
#define BOSS_CONTINUE_DASH 148   //蓄力与冲刺持续时间 11*8 + 1*60
#define BOSS_CONTINUE_JUMP 116   //普通跳跃时间 48 + 68
#define BOSS_CONTINUE_JUMP_DASH 84   //冲刺跳跃时间 40 + 44
#define BOSS_CONTINUE_JUMP_FINAL 151  // 75 + 56 + 20
#define  BOSS_X_DASH 36   //左右冲刺速度
#define BOSS_X_SPEED 6    //左右移动速度
#define BOSS_Y_SPEED 40   //跳的初始速度
#define DOWN_SPEED 20     //匀速下落的速度
#define LAND_TIME 24      //落地缓冲时间
#define BOSS_DAMAGE_TIME 20 //
#define BOSS_DAMAGE_CD 40

extern int boss_dirct;          //默然方向
extern int boos_wait_dash;
extern int boos_wait_jump;
extern int boos_wait_jump_dash;
extern int boos_wait_jump_final;
extern int boss_wait_damage;
extern int boss_damage_cd;
extern bool boos_dash_flag;
extern bool boos_jump_flag;
extern int boss_cont;
extern int boss_x_speed;
extern int boss_y_speed;
extern bool boos_walk_flag;
extern bool boss_jump_dash_flag;
extern bool boss_land_flag;
extern bool boss_jump_final_flag;
extern bool boss_effect;
extern bool Attack_hero ;
extern bool Attack_boss;
extern bool boss_damage_flag;
extern int boss_hp;
extern bool lose;



void boss_script(Animation &boss,Animation &hero,Animation &boss_Effect,Area &ground,Animation &boss_Attack_Effect,Animation &boss_Damage_Effect,Sound &sword_hit,Sound &aidito,Sound &xiao,Sound &ha,Sound &haha) {
    //hero.number
    //0 向右待机
    //1 向左待机
    //2 向右走
    //3 向左走
    //4 向右冲
    //5 向左冲
    //6 普通向右跳
    //7 普通向左跳
    //8 向右冲刺
    //9 向左冲刺
    //10 向右跳技能
    //11 向左跳技能

    if (boss_hp == 0) {
        lose = true;
        return;
    }

    //生成随机数
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,3);
    int choice = 0;

    //初始化速度
    boss_x_speed = 0;

    Attack_hero = false;

    //当进程为空时可以改变方向
    if (hero.An_x < boss.An_x && boss_dirct == 0 && boss_cont == 0) {
        boss_dirct = 1;
    }
    else if (hero.An_x > boss.An_x && boss_dirct == 1 && boss_cont == 0) {
        boss_dirct = 0;
    }

    if (boss_cont == 0) {
        choice = dist(gen);
    }
    //随机选择
    if (choice == 0) {
        //冲刺条件判断
        if (hero.p_y == 0 && boss.p_y == 0 && boos_wait_dash == 0 && boss_cont == 0) {

            if (abs(boss.An_x - hero.An_x) < 1000) {
                boos_wait_dash = BOSS_CD_DASH;
                boss_cont = BOSS_CONTINUE_DASH;
                boos_dash_flag = true;
            }
            else {
                boos_walk_flag = true;
            }
        }
    }
    else if (choice == 1) {
        //跳跃条件判断
        if (abs(boss.An_x - hero.An_x) < 300  && boos_wait_jump == 0 && boss_cont == 0) {
            boos_wait_jump = BOSS_CD_JUMP;
            boss_cont = BOSS_CONTINUE_JUMP;
            boos_jump_flag = true;
            boss_y_speed = BOSS_Y_SPEED;
            haha.Play_Music();
        }
    }
    else if (choice == 2) {
        //冲刺跳跃条件判断
        if (boos_wait_jump_dash == 0 && boss_cont == 0) {
            boss_jump_dash_flag = true;
            boos_wait_jump_dash = BOSS_CD_JUMP_DASH;
            boss_y_speed = BOSS_Y_SPEED;
            boss_cont = BOSS_CONTINUE_JUMP_DASH;
            xiao.Play_Music();
        }
    }
    else if (choice == 3) {
        //final条件判断
        if (boos_wait_jump_final == 0 && boss_cont == 0) {
            boss_jump_final_flag = true;
            boos_wait_jump_final = BOSS_CD_JUMP_FINAL;
            boss_y_speed = BOSS_Y_SPEED;
            boss_cont = BOSS_CONTINUE_JUMP_FINAL;
        }
    }

    //冲刺段音效控制
    if ( boos_dash_flag == true && boss_cont == 60) {
        aidito.Play_Music();
    }

    //冲刺CD倒计时
    if (boos_wait_dash) {
        boos_wait_dash--;
    }

    //跳跃CD倒计时
    if (boos_wait_jump) {
        boos_wait_jump--;
    }

    //冲刺跳跃CD倒计时
    if (boos_wait_jump_dash) {
        boos_wait_jump_dash--;
    }

    //finalCD倒计时
    if (boos_wait_jump_final) {
        boos_wait_jump_final--;
    }

    //冲刺跳跃阶段控制速度
    if ( boss_jump_dash_flag == true) {
       if (boss_cont > 44) {
           //x速度
           if (boss_dirct == 0) {
               boss_x_speed = BOSS_X_SPEED;
           }
           else {
               boss_x_speed = -BOSS_X_SPEED;
           }

           //y速度
           boss_y_speed -= G;
       }
        else if (boss_cont <= 44 && boss_cont > 0) {
            //x速度
            if (boss_dirct == 0) {
                boss_x_speed = BOSS_X_DASH;
            }
            else {
                boss_x_speed = -BOSS_X_DASH;
            }

            //y速度
            boss_y_speed = -BOSS_X_DASH;

            if (ground.a_y - boss.a_y <= (ground.a_h + boss.a_h)/2){
                boss.p_y = 0;   //着陆恢复初始高度
                boss_y_speed = 0;
                boss_jump_dash_flag = false;
                boss_land_flag = true;
                boss_cont = LAND_TIME;
            }
        }

    }

    //final阶段控制速度
    if (boss_jump_final_flag == true) {
        if (boss_cont > 76) {
            //x速度
            if (boss_dirct == 0) {
                boss_x_speed = BOSS_X_SPEED;
            }
            else {
                boss_x_speed = -BOSS_X_SPEED;
            }

            //y速度
            boss_y_speed -= G;
        }
        else if (boss_cont <= 76 && boss_cont > 20) {
            //x速度
            boss_x_speed = 0;
            //y速度
            boss_y_speed = 0;
            boss_effect = true;
        }
        else if (boss_cont <= 20 && boss_cont > 0) {
            boss_effect = false;
            //x速度
            boss_x_speed = 0;
            //y速度
            boss_y_speed = -DOWN_SPEED/2;
            if (boss_cont == 1){
                boss_jump_final_flag = false;
                boss.p_y = 0;   //着陆恢复初始高度
                boss_Attack_Effect.p_y = 0;
                boss_Damage_Effect.p_y = 0;
                boss_y_speed = 0;
                boss_jump_dash_flag = false;
                boss_land_flag = true;
                boss_cont = LAND_TIME;
            }
        }

    }

    //Final音效
    if (boss_cont == 76 && boss_jump_final_flag == true) {
        ha.Play_Music();
    }


    //跳跃阶段控制速度
    if ( boos_jump_flag == true) {
        //x速度
        if (boss_dirct == 0) {
            boss_x_speed = BOSS_X_SPEED;
        }
        else {
            boss_x_speed = -BOSS_X_SPEED;
        }

        //y速度
        //模拟跳跃过程
        if (boss_y_speed > -DOWN_SPEED && boos_jump_flag == true) {
            boss_y_speed -= G;
        }
        else if (ground.a_y - boss.a_y <= (ground.a_h + boss.a_h)/2){
            boss.p_y = 0;   //着陆恢复初始高度
            boss_Attack_Effect.p_y = 0;
            boss_Damage_Effect.p_y = 0;
            boss_y_speed = 0;
            boos_jump_flag = false;
            boss_land_flag = true;
            boss_cont = LAND_TIME;
        }

    }

    //进程倒计时
    if (boss_cont) {
        boss_cont--;
        if ( boss_cont == 0) {
            boos_dash_flag = false;
            boos_jump_flag = false;
            boss_land_flag = false;
            boss_jump_dash_flag = false;
        }
    }

    if (abs(boss.An_x - hero.An_x) >= 1000 && boos_walk_flag == true) {
        if (boss_dirct == 0) {
            boss_x_speed = BOSS_X_SPEED;
        }
        else {
            boss_x_speed = -BOSS_X_SPEED;
        }
        boss_cont = 1;
    }
    else if (abs(boss.An_x - hero.An_x) < 1000 && boos_walk_flag == true) {
        boss_cont = 0;
        boos_walk_flag = false;
    }

    //冲刺段速度控制
    if ( boos_dash_flag == true && boss_cont < 60 && boss_cont > 0) {
        if (boss_dirct == 0) {
            boss_x_speed = BOSS_X_DASH;
        }
        else {
            boss_x_speed = -BOSS_X_DASH;
        }
    }

    //墙壁
    if ((boss.An_x - boss.An_w/2<= 0 && boss_dirct == 1) || (boss.An_x + boss.An_w/2 >= WIDTH && boss_dirct == 0)) {
        boss_x_speed = 0;
    }

    if (Attack_boss == true && boss_damage_flag == false && boss_damage_cd == 0) {
        boss_hp--;
        boss_wait_damage = BOSS_DAMAGE_TIME;
        boss_damage_cd = BOSS_DAMAGE_CD;
        Attack_boss = false;
        boss_damage_flag = true;
    }

    if (boss_wait_damage > 0) {
        boss_wait_damage--;
        if (boss_wait_damage == 0) {
            boss_damage_flag = false;
        }
    }

    if (boss_damage_cd > 0) {
        boss_damage_cd--;
    }

    //更新坐标
    boss.p_y -= boss_y_speed;
    boss.p_x += boss_x_speed;
    boss_Effect.p_x += boss_x_speed;
    boss_Attack_Effect.p_x += boss_x_speed;
    boss_Attack_Effect.p_y -= boss_y_speed;
    boss_Damage_Effect.p_y -= boss_y_speed;
    boss_Damage_Effect.p_x += boss_x_speed;

    //绘图区

    //受到攻击动画
    if (boss_damage_flag == true) {

        if (boss_dirct) {
            boss_Damage_Effect.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct);
        }
        else {
            boss_Damage_Effect.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct);
        }

    }

    //技能动画
    if (boss_jump_final_flag == true) {

        if (boss_dirct) {
                boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+10);
        }
        else {
                boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+10);
        }
        if (boss_effect == true) {
            boss_Attack_Effect.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,0);
            boss_Attack_Effect.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/5,HEIGHT/3);
            if (In_Area(hero, boss_Attack_Effect)) {
                Attack_hero = true;
            }
        }
    }
    //跳跃冲刺动画
    else if (boss_jump_dash_flag == true) {
        if (boss_dirct) {
             if (boss_cont > 44) {
                 boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+8);
             }
            else {
                cout << 1 << endl;
                boss.Play_Animation_Loop_A(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+8,11);
                boss_Attack_Effect.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/10,HEIGHT/5);
                if (In_Area(hero, boss_Attack_Effect)) {
                    Attack_hero = true;
                }
            }
        }
        else {
            if (boss_cont > 44) {
                boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+8);
            }
            else {
                boss.Play_Animation_Loop_A(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+8,11);
                boss_Attack_Effect.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/10,HEIGHT/5);
                if (In_Area(hero, boss_Attack_Effect)) {
                    Attack_hero = true;
                }
            }

        }
    }
    //着路动画
    else if (boss_land_flag == true) {
        if (boss_dirct) {
            boss.Play_Animation_Loop_A(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+6,23);
        }
        else {
            boss.Play_Animation_Loop_A(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+6,23);
        }
    }
    //距离太近跳走
    else if (boos_jump_flag == true) {

        if (boss_dirct) {
            boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+6);
        }
        else {
            boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,4,boss_dirct+6);
        }
    }
    //地面上距离太远不在冲刺距离，走路
    else if ( boos_walk_flag == true) {
        if (boss_dirct) {
            boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+2);
        }
        else {
            boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+2);
        }
    }
    //冲刺攻击
    else if (boos_dash_flag == true) {
        //蓄力阶段
        if (boss_cont > 60) {
            boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+4);
        }
        //冲刺阶段
        else if(boss_cont <= 60 && boss_cont > 0) {
            boss.Play_Animation_Loop_A(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct+4,11);
            if (boss_dirct) {
                boss_Effect.Play_Animation_Loop(WIDTH/1.3, HEIGHT*0.83,15,boss_dirct);
                boss_Attack_Effect.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/6,HEIGHT/5);
                if (In_Area(hero, boss_Attack_Effect)) {
                    Attack_hero = true;
                }
            }
            else {
                boss_Effect.Play_Animation_Loop(WIDTH/1.7, HEIGHT*0.83,15,boss_dirct);
                boss_Attack_Effect.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/6,HEIGHT/5);
                if (In_Area(hero, boss_Attack_Effect)) {
                    Attack_hero = true;
                }
            }

        }
    }
    //原地待机
    else {
        boss.Play_Animation_Loop(WIDTH/1.5, HEIGHT*0.83,8,boss_dirct);
    }
}