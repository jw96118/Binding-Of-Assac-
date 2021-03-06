#pragma once

#ifndef __ENUM_H__

enum OBJECT_TYPE
{
	PLAYER,
	MONSTER,
	PLAYER_BULLET,
	MONSTER_BULLET,
	MONSTRO_BULLET,
	BONY_BULLET,
	SHIELD,
	MAP,
	GRID,
	ITEM_BOTTOM,
	P_ITEM,
	UI,
	ROCK,
	EXPLOSION,
	BOMB,
	OBJECT_END
};

enum RENDER_LAYER
{
	LAYER_BACK,
	LAYER_OBJECT,
	LAYER_FLY_OBJECT,
	LAYER_ITEM,
	LAYER_EFFECT,
	LAYER_UI,
	LAYER_END
};
enum PLAYER_STATE
{
	IDLE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
};		
enum PLAYER_INVIL_STATE
{
	NOT_INVINCIBILITY,
	INVINCIBILITY
};
enum PLAYER_ATTACK_STATE
{
	NO,
	ATTACK_LEFT,
	ATTACK_RIGHT,
	ATTACK_UP,
	ATTACK_DOWN
};
enum MONSTER_TYPE
{
	BLANK = -1,
	BONEY = 0,
	CHARGER,
	MASKANDHEART,
	LEECH,
	MULLIGAN,
	MONSTRO
};
enum MONSTER_STATE
{
	MONSTER_IDLE,
	MONSTER_MOVE_LEFT,
	MONSTER_MOVE_RIGHT,
	MONSTER_MOVE_TOP,
	MONSTER_MOVE_BOTTOM,
	MONSTER_ATTACK_LEFT,
	MONSTER_ATTACK_RIGHT,
	MONSTER_ATTACK_TOP,
	MONSTER_ATTACK_BOTTOM,
	MONSTER_DEAD
};
enum MONSTRO_PATTERN
{
	BATTLE_START,
	SHORT_JUMP,
	LONG_JUMP,
	SHOT_TEAR
};
enum PLAYER_MOVE_STATE
{
	NO_MOVE,
	MOVE_ROOM,
};

enum P_ITEM_LIST
{
	NON = -1,
	SPOONBENDER,
	CRICKETSHEAD,
	HALOOFFIES,
	THEPACT,
	THEMULIGAN,
	FATE,
	LOSTCONTACT,
	TWOTWO
};
enum GAME_STATE
{
	TITLE,
	SAVE_SELECT,
	GAME_SELECT,
	MAIN_GAME,
	EVENT_SCENE,
	OPEN_OPTION
};
enum GAME_SELECT_STATE
{
	NEW_RUN,
	CONTINUE,
	CHALLENGES,
	STATS,
	OPTIONS,
	GAME_SELECT_STATE_END
};
enum ROOM_TYPE
{
	START_ROOM,
	GOLD_ROOM,
	NOMAL_ROOM,
	MONSTER_ROOM,
	BOSS_ROOM
};
enum ROOM_STATE
{
	NO_BATTLE,
	NOW_BATTLE
};
enum ROOM_DIRECTION
{
	NOT=-1,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};
namespace SOUNDMGR
{
	enum CHANNEL_ID { BGM, PLAYER, MONSTER, MONSTER_DEAD , ROCK_BROKEN, EFFECT, SCENE, MAX_CHANNEL };

}
#define __ENUM_H__
#endif
