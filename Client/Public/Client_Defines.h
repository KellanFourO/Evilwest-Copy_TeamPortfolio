#pragma once

#include "Engine_Defines.h"

namespace Client
{
	/* 1600 x 900 ���������� ���� */
	static const unsigned int	g_iWinSizeY = 720;
	static const unsigned int	g_iWinSizeX = 1280;
	//static const unsigned int	g_iWinSizeX = 1600;
	//static const unsigned int	g_iWinSizeY = 900;
	//static const unsigned int	g_iWinSizeX = 1920;
	//static const unsigned int	g_iWinSizeY = 1080;
	//static const unsigned int	g_iWinSizeX = 2560;
	//static const unsigned int	g_iWinSizeY = 1440;

	// ! Close Window
	_bool g_CloseWindow = false;
	_bool g_UIMouseDownLB = false;
	_bool g_UIMousePressingLB = false;
	_bool g_UIMouseDownRB = false;
	_bool g_UIMousePressingRB = false;

	enum LEVEL { 
		LEVEL_STATIC, 
		LEVEL_LOGO, 

		LEVEL_INTRO,
		LEVEL_INTRO_BOSS,
		LEVEL_SNOWMOUNTAIN,
		LEVEL_SNOWMOUNTAINBOSS,
		LEVEL_LAVA,
	
		LEVEL_TOOL,
		LEVEL_LOADING,

		LEVEL_GAMEPLAY,

		LEVEL_END };

	enum class OBJECT_LAYER 
	{
		BACKGROUND,
		BUILDING,
		CAMERA,
		EFFECT,
		MONSTER,
		PLAYER,
		UI_MONSTER,
		UI_PLAYER,
		PARTICLE,
		OBJECT_LAYER_END
	};

	#define LAYER_BACKGROUND				TEXT("Layer_BackGround")
	#define LAYER_BUILDING					TEXT("Layer_Building")
	#define LAYER_CAMERA					TEXT("Layer_Camera")
	#define LAYER_EFFECT					TEXT("Layer_Effect")
	#define LAYER_MONSTER					TEXT("Layer_Monster")
	#define LAYER_BOSS						TEXT("Layer_Boss")
	#define LAYER_PLAYER					TEXT("Layer_Player")
	#define LAYER_UI_MONSTER				TEXT("Layer_UI_Monster")
	#define LAYER_UI_PLAYER					TEXT("Layer_UI_Player")
	#define LAYER_PARTICLE					TEXT("Layer_Particle")

	#define LAYER_PLAYER_BULLET				TEXT("Layer_Player_Bullet")
	#define LAYER_MONSTER_BULLET			TEXT("Layer_Monster_Bullet")


	//Player_Weapon
	#define PLAYER_WEAPON_PUNCH_L			TEXT("Weapon_Punch_L")
	#define PLAYER_WEAPON_PUNCH_R			TEXT("Weapon_Punch_R")
	#define PLAYER_WEAPON_WINCHESTER		TEXT("Weapon_Winchester")
	#define PLAYER_WEAPON_KICK				TEXT("Weapon_Kick")
	#define PLAYER_WEAPON_ZAPPER			TEXT("Weapon_Zapper")
	#define PLAYER_WEAPON_REVOLVER			TEXT("Weapon_Revolver")
	#define PLAYER_WEAPON_SHOTGUN			TEXT("Weapon_Shotgun")
	#define PLAYER_WEAPON_DYNAMITE			TEXT("Weapon_Dynamite")
	#define PLAYER_WEAPON_WHIP				TEXT("Weapon_Whip")



	#define TANK_WEAPON_PUNCH_L				TEXT("Weapon_Punch_L")
	#define TANK_WEAPON_PUNCH_R				TEXT("Weapon_Punch_R")

	#define BANDIT_HEAVY_WEAPON				TEXT("Weapon_Axe")

	#define NPC_WEAPON_EDGAR				TEXT("Weapon_Edgar")

	




	enum class COLLISION_LAYER
	{
		NONE,
		PLAYER_PARRYING,
		MONSTER_SHIELD,
		WEAKNESS,
		EXPLOSION_ATTACK,
		PLAYER_ATTACK,
		MONSTER_ATTACK,
		PLAYER,
		MONSTER,
		DESTRUCTABLE_PROPS,
		OBSTACLE,

		INTERACT,
		PICKING_INSTANCE,
		
		//EDIT_NAVIMESH,
		//DYNAMIC_PROP,
		//BOSS_DYNAMIC_PROP,
		//TRIGGER,
		//LADDER_UP,
		//LADDER_DOWN,
		//DOOR,
		//ELEVATOR,
		//CHECKPOINT,
		//PLAYER_BATCOL,
		//ITEM,
		//ONLY_VIEW,
		//CHECK_DIR,
		//INTERIOR,
		//PICKING_MESH,
		LAYER_END
	};

	enum class PHYSX_COLLISION_LAYER
	{
		NONE,
		PLAYER,
		PLAYER_WEAPON,
		MONSTER,
		TERRAIN,
		STATIC_PROP,
		DYNAMIC_PROP,
		DYNAMIC_PEICE,
		GROUND,
		CAMERA,
		INTERIOR,
		LAYER_END
	};

	enum class GAME_STATE 
	{
		GAMEPLAY,
		UI,
		MAINMENU,
		OPTION,
		SKILLWINDOW,
		CUTSCENE,
		DEAD,
		TALK,
		GAME_STATE_END
	};

	enum class Additional_Skill
	{
		/* Active */
		KICK,			// ������
		UPPER_CUT,		// ������
		HERO_PUNCH,		// ������ġ (������ ������)
		HEAL,			// ȸ�� ��ų
		ELECTRIC_DASH,	// ������
		ELECTROCUTE,	// ��� �� ���� (�ռ�)
		ELECTRIC_WHIP,	// ���� �������
		QUAKE_PUNCH,	// ������� 1
		QUAKE_PUNCH2,	// ������� 2
		QUAKE_PUNCH3,	// ������� 3
		HIT_EIGHT,		// E-�޺� (���� ��ġ) [�������¿��� ��밡��]
		SUPER_CHARGE,	// �������� ���
		TELEPORT_PUNCH,	// �ڷ���Ʈ ��ġ

		/* Passive */
		MAX_HP,						// �ִ� ü�� ���� (10%)
		REGEN_HP,					// ü�� ��� (ü���� ������ ȸ���Ǵ� ȿ��)
		ELECTRIC_DAMAGE_INCREASE,	// ���⽺ų ����� ����
		ELECTRIC_COOLTIME_DECREASE,	// ���⽺ų ��Ÿ�� ����
		MELEE_DAMAGE_INCREASE,		// ������ų ����� ����
		INCREASE_EXP,				// ����ġ ȹ�淮 ����
		//MELEE_COOLTIME_DECREASE,	// ������ų ��Ÿ�� ���� => INCREASE_EXP !����ġ ȹ�淮 ������ ����
		HASTE,						// �̵��ӵ� ����

		/* Weapon */
		RIFLE,						// ������
		REVOLVER,					// ������
		SHOTGUN,					// ����

		/* Weapon Passive */
		RIFLE_SKILL1,				// ������ ��ų1
		REVOLVER_SKILL1,			// ������ ��ų1
		SHOTGUN_SKILL1,				// ���� ��ų1


		Additional_Skill_End
	};

	enum class Additional_Weapon
	{
		REVOLVER,
		RIFLE,
		SHOTGUN,
		Additional_Weapon_End
	};

/* Yoojeong_Add */
#define EFFECT_MANAGER CEffect_Manager::GetInstance()
/* Yoojeong_End */
	
}

using namespace Client;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

#include "Client_Enum.h"
#include "Client_Preset.h"
