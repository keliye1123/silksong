#include "file.h"

#define DAMAGE_HP_CONTINUE 30   //摧毁动画持续时间
#define DAMAGE_HP_CD 60   //摧毁动画CD
#define TEMP_CONTINUE  30   //待机动画播放时间
#define TEMP_CD  120   //待机动画播放CD

extern int hero_hp;
extern int Attack_hero;

bool Damage_flag = false;
bool tmep_flag = false;
int damage_hp_cont = 0;
int damage_hp_wait = 0;
int temp_cont = 0;
int temp_wait = 0;


float a[5] = {6,4.7,3.85,3.25,2.85};

void UI(Sprite &background, Sprite &back_hp, Sprite &empty,Animation &HP,Animation &HP_Damage) {

    //背景绘制
    background.Draw_Sprite(WIDTH/2,HEIGHT/2);

    //血条框绘制
    back_hp.Draw_Sprite(WIDTH/10,HEIGHT/10);
    for (int i = 0;i < hero_hp;i++) {
        empty.Draw_Sprite(WIDTH/a[i], HEIGHT/10);
        HP.Play_Animation_Loop(WIDTH/a[i], HEIGHT/10,30,0);
    }

    //摧毁动画过程
    if (Attack_hero == true && Damage_flag == false && damage_hp_wait == 0) {
        HP_Damage.index = 0;
        Damage_flag = true;
        damage_hp_cont = DAMAGE_HP_CONTINUE;
        damage_hp_wait = DAMAGE_HP_CD;
    }
    if (Damage_flag == true) {
        HP_Damage.Play_Animation_Loop(WIDTH/a[hero_hp], HEIGHT/10,5,0);
        damage_hp_cont--;
        if ( damage_hp_cont == 0) {
            Damage_flag = false;
        }
    }
    if ( damage_hp_wait  != 0) {
        damage_hp_wait--;
    }




}