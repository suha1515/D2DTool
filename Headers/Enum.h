#pragma once

#ifndef __ENUM_H__

enum Layer {LAYER_GROUND, LAYER_0, LAYER_1, LAYER_2, LAYER_3, LAYER_4,LAYER_5,LAYER_END };

enum ANIMATION_TYPE{ANIMATION_LOOP,ANIMATION_ONCE};

enum COLLIDE_TYPE{NORMAL,LEFT_TOP,LEFT_BOTTOM,RIGHT_TOP,RIGHT_BOTTOM};
 

enum DIR {
	UP, RIGHT_UP_45, RIGHT, RIGHT_DOWN_45, DOWN,
	LEFT_UP_45, LEFT, LEFT_DOWN_45
};

enum ENEMY_TYPE { MOUSE, BOT, TURRET ,BOSS};
#define __ENUM_H__
#endif