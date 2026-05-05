
#include "file.h"
#include "script.h"
#include "boss_script.h"
#include "UI.h"
using namespace std;

//初始化
bool lose = false;

//hero
int hero_dirct = 0;          //默然方向
int hero_hp = 1;
int x_speed = 0;
int y_speed = 0;
bool jump_flag = false;      //跳跃状态标记
bool dash_flag = false;      //冲刺状态标记
bool attack_flag = false;    //攻击状态标记
bool double_flag = false;    //二次攻击标记
bool cnt_flag = false;
bool Attack_hero_flag = false;
bool LEFT = false;
int wait_attack_hero = 0;   //收到攻击倒计时
int cnt = 0 ;
int wait_dash = 0;      //冲刺CD倒计时
int wait_attack = 0;    //攻击CD倒计时
int cont = 0;           //进程占用
bool Attack_boss;
int death_time = 0;

//boss
int boss_dirct = 1;          //默然方向
int boss_hp = 1;
int boos_wait_dash = 0;         //冲刺CD倒计时
int boos_wait_jump = 0;         //跳跃CD倒计时
int boos_wait_jump_dash = 0;         //冲刺跳跃CD倒计时
int boos_wait_jump_final = 0;         //finalCD倒计时
int boss_wait_damage = 0;
int boss_damage_cd = 0;
bool boos_dash_flag = false;    //冲刺状态
bool boos_jump_flag = false;    //跳跃状态
int boss_cont = 0;              //进程占用
int boss_x_speed = 0;
int boss_y_speed = 0;
bool boos_walk_flag = false;
bool boss_jump_dash_flag = false;
bool boss_land_flag = false;
bool boss_jump_final_flag = false;
bool boss_effect = false;
bool Attack_hero = false;              //hero收到攻击FLAG
bool boss_damage_flag;

bool scene_1() {
    //初始化
    {
        lose = false;
        //hero
        hero_dirct = 0;          //默然方向
        hero_hp = 5;
        x_speed = 0;
        y_speed = 0;
        jump_flag = false;      //跳跃状态标记
        dash_flag = false;      //冲刺状态标记
        attack_flag = false;    //攻击状态标记
        double_flag = false;    //二次攻击标记
        cnt_flag = false;
        Attack_hero_flag = false;
        LEFT = false;
        wait_attack_hero = 0;   //收到攻击倒计时
        cnt = 0 ;
        wait_dash = 0;      //冲刺CD倒计时
        wait_attack = 0;    //攻击CD倒计时
        cont = 0;           //进程占用
        Attack_boss = false;
        death_time = 0;

        //boss
        boss_dirct = 1;          //默然方向
        boss_hp = 25;
        boos_wait_dash = 0;         //冲刺CD倒计时
        boos_wait_jump = 0;         //跳跃CD倒计时
        boos_wait_jump_dash = 0;         //冲刺跳跃CD倒计时
        boos_wait_jump_final = 0;         //finalCD倒计时
        boss_wait_damage = 0;
        boss_damage_cd = 0;
        boos_jump_flag = false;    //跳跃状态
        boss_cont = 0;              //进程占用
        boss_x_speed = 0;
        boss_y_speed = 0;
        boos_walk_flag = false;
        boss_jump_dash_flag = false;
        boss_land_flag = false;
        boss_jump_final_flag = false;
        boss_effect = false;
        Attack_hero = false;              //hero收到攻击FLAG
        boss_damage_flag = false;
    }
    //图片资源加载
    Sprite background,back_hp,empty;
    Animation HP,HP_Damage;
    Area ground;
    Animation hero,Effect,Effect_Attack;
    Animation boss,boss_Effect;
    Animation boss_Attack_Effect;
    Animation Damage_Effect;
    Animation boss_Damage_Effect;
    {
        background.Load_Sprite(WIDTH,HEIGHT,"img/hollow knight/background.png");
        back_hp.Load_Sprite(WIDTH/10,HEIGHT/10,"img/hollow knight/UI/Blood/00.png");
        empty.Load_Sprite(WIDTH/25,HEIGHT/20,"img/hollow knight/UI/Blood/empty.png");

        HP.Load_Animation("img/hollow knight/UI/Blood/tmp",6,WIDTH/25,HEIGHT/20);
        HP_Damage.Load_Animation("img/hollow knight/UI/Blood/Damage",6,WIDTH/25,HEIGHT/20);

        hero.Load_Animation("img/hollow knight/IdleR",9,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Idle",9,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/WalkR",8,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Walk",8,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/JumpR/Start",9,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/JumpR/Loop",3,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/JumpR/Land",3,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Jump/Start",9,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Jump/Loop",3,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Jump/Land",3,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/DashR",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Dash",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/AttackR/Attack/1",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Attack/Attack/1",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/AttackR/Attack/2",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Attack/Attack/2",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/AttackR/AttackUp",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Attack/AttackUp",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/AttackR/AttackDown",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Attack/AttackDown",5,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/DamageR",6,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Damage",6,WIDTH/20,HEIGHT/11);
        hero.Load_Animation("img/hollow knight/Death",10,WIDTH/20,HEIGHT/11);

        Effect.Load_Animation("img/hollow knight/DashEffectR",5,WIDTH/5,HEIGHT/5);
        Effect.Load_Animation("img/hollow knight/DashEffect",5,WIDTH/5,HEIGHT/5);

        Effect_Attack.Load_Animation("img/hollow knight/AttackR/Attack/effect1",2,WIDTH/10,HEIGHT/10);
        Effect_Attack.Load_Animation("img/hollow knight/Attack/Attack/effect1",2,WIDTH/10,HEIGHT/10);
        Effect_Attack.Load_Animation("img/hollow knight/AttackR/Attack/effect2",2,WIDTH/10,HEIGHT/10);
        Effect_Attack.Load_Animation("img/hollow knight/Attack/Attack/effect2",2,WIDTH/10,HEIGHT/10);

        boss.Load_Animation("img/hollow knight/Boss/IdleR",2,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/Idle",2,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/WalkR",11,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/Walk",11,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/DashR",12,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/Dash",12,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/JumpR",29,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/Jump",29,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/JumpDashR",12,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/JumpDash",12,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/JumpFinalR",46,WIDTH/10,HEIGHT/5);
        boss.Load_Animation("img/hollow knight/Boss/JumpFinal",46,WIDTH/10,HEIGHT/5);
        boss_Effect.Load_Animation("img/hollow knight/Boss/DashR/Dash_EffectR",4,WIDTH/10,HEIGHT/5);
        boss_Effect.Load_Animation("img/hollow knight/Boss/Dash/Dash_Effect",4,WIDTH/10,HEIGHT/5);

        boss_Attack_Effect.Load_Animation("img/hollow knight/Boss/JumpFinal/Effects",18,WIDTH/5,HEIGHT/3);

        Damage_Effect.Load_Animation("img/hollow knight/Damage_Effect",3,WIDTH/5,HEIGHT/3);

        boss_Damage_Effect.Load_Animation("img/hollow knight/AttackEffect/AttackR",5,WIDTH/5,HEIGHT/3);
        boss_Damage_Effect.Load_Animation("img/hollow knight/AttackEffect/Attack",5,WIDTH/5,HEIGHT/3);


    }

    //声明音乐变量
    Sound damage_music,bgm,sword,sword_hit,aidito,boss_open,xiao,ha,haha;
    bgm.Load_Music("music/Hornet.mp3",-1);
    damage_music.Load_Music("music/Player/player_damage.mp3",-1);
    sword.Load_Music("music/Player/sword_1.mp3",-1);
    sword_hit.Load_Music("music/Player/sword_hit.mp3",-1);
    aidito.Load_Music("music/Boss/aidito.mp3",-1);
    boss_open.Load_Music("music/Boss/open.mp3",-1);
    xiao.Load_Music("music/Boss/xiao.mp3",-1);
    ha.Load_Music("music/Boss/ha.mp3",-1);
    haha.Load_Music("music/Boss/haha.mp3",-1);

    bgm.Play_Bgm();
    boss_open.Play_Music();
    while (true) {
        clock_t StartTime = clock();

        ground.Set_Area(WIDTH/2,HEIGHT*13/14,WIDTH,HEIGHT/17);                 //地面区域更新
        hero.Set_Area(WIDTH/2.25, HEIGHT*0.86,WIDTH/20,HEIGHT/11);            //hero区域更新
        boss.Set_Area(WIDTH/1.5, HEIGHT*0.83,WIDTH/10,HEIGHT/5);
        //绘图区
        BeginBatchDraw();

        UI(background,back_hp,empty,HP,HP_Damage);

        //boss绘制与脚本
        boss_script(boss,hero,boss_Effect,ground,boss_Attack_Effect, boss_Damage_Effect,sword_hit,aidito,xiao,ha,haha);

        //主角绘制与脚本
        control(hero,ground,Effect,Effect_Attack,boss,Damage_Effect,damage_music,sword,sword_hit);

        char s[20];
        sprintf(s,"boss血量：%d",boss_hp);
        setbkmode(TRANSPARENT);
        settextcolor(RED);
        settextstyle(50,0,"微软雅黑");
        outtextxy(WIDTH/1.4,HEIGHT/10,s);

        EndBatchDraw();

        if (lose) {
            bgm.Close_Bgm();
            cleardevice();
            Sprite End;
            End.Load_Sprite(WIDTH*0.9,HEIGHT*0.9,"UI/playagain.png");
            Area Yes,No;
            Yes.Set_Area(WIDTH/2.45,HEIGHT/2,WIDTH/40,HEIGHT/20);
            No.Set_Area(WIDTH/1.68,HEIGHT/2,WIDTH/40,HEIGHT/20);
            End.Draw_Sprite(WIDTH/2,HEIGHT/2);
            while (1) {//点击开始游戏检测
                GetCursorPos(&pt);//鼠标坐标监测
                if (Yes.Mouse_In_Area_Rectangle() && GetAsyncKeyState(VK_LBUTTON)) {
                    return true;
                }
                else if (No.Mouse_In_Area_Rectangle() && GetAsyncKeyState(VK_LBUTTON)) {
                    return false;
                }
            }
        }

        //控制帧率

        clock_t EndTime = clock();
        if (EndTime - StartTime < FPS) Sleep(FPS - (EndTime - StartTime));
        // EndTime = clock();
        // cout << EndTime - StartTime << endl;
    }
}