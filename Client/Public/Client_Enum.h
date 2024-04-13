#ifndef Client_Enum_h__
#define Client_Enum_h__

namespace Engine
{
	enum class MODEL_SHADER {
		MODEL_ORIGIN,				// 0 - ���� �� �׷��� Render
		MODEL_NODEOPTH,				// 1 - ���̱��x -> SkyBox ������ �뵵 
		MODEL_SHADOW,				// 2 - �׸��� �׸��� 
		MODEL_CASCADE,				// 3 - CSM 
		MODEL_WHITEBLINK,			// 4 - ������� �����Ÿ� 
		MODEL_NONECULL_NONEDSS,		// 5
		MODEL_INTROBOSS_BLOODPOOL,  // 6 - Intro������ �ٴڷ�����
		MODEL_NORMAL_MAPPING,		// 7 - �븻���� �Ѱ� 
		MODEL_DIFFUSE,				// 8 - Diffuse �÷��� �ܺο��� �����¿뵵 
		MODEL_ALPHA,				// 9 - AlphaTexture ���ε� 
		MODEL_ICICLE,				// 10 - ��帧 ���� �𵨵� ������
		MODEL_FOLIAGECLIP,			// 11 - 
		MODEL_OUTLINE_KEEP,			// 12 - OutLine �״�� ���� 
		MODEL_OUTLINE_BLINK,		// 13 - OutLine �����Ÿ�
		MODEL_EX_EMISSIVE,			// 14 - Emissive �������� �ȳ���
		MODEL_MESH_BLOOM ,			// 15 - GLOWȿ�� �׷��ϱ� �ش�޽���ü�� ����� �ִ°� 
		MODEL_SHADER_END
	};

	enum class ANIM_SHADER {
		ANIM_ORIGIN,				// 0 - �⺻����
		ANIM_WIREFRAME,				// 1 - �޽÷� ���� 
		ANIM_SHADOW,				// 2 - �׸���
		ANIM_CASCADE,				// 3 - CSM
		ANIM_INTRO_BOSS,			// 4 - INTRO STAGE ���� 
		ANIM_EXAMPLE,				// 5 - Bloom , Rim ����ϴ� ����
		ANIM_SHADER_END };

	enum class MONSTER_SHADER {
		COMMON_ORIGIN,				// 0 - �⺻ �״�� �׸��� 
		COMMON_WIREFRAME,			// 1 - ����� 
		COMMON_SHADOW,				// 2 - �׸��� �׸��� 
		COMMON_CASCADE,				// 3 - CSM
		COMMON_DISSOLVE,			// 4 - ���� : ������� �������ȿ�� 
		COMMON_MESHCHECK,			// 5 - ���� : ������ �÷��� �ش��ϴ� �޽��� ���� �ٲ㼭 �޽ð� ����ε������� üũ�ϴ¿뵵 
		COMMON_OUTLINE_BLINK,		// 6 - �ܰ��� �����Ÿ� 
		COMMON_OUTLINE_KEEP,		// 7 - �ܰ��� ��� ���� 
		INFECTED_PUNCH,				// 8 - Infected - Attack���� �ָԺ��ϱ� 
		COMMON_EXCEPT_EMISSIVE,		// 9 - EMISSIVE�ؽ��� ���ع��� 
		MONSTER_SHADER_END };

	enum class UI_SHADER {UI_SHADER_END};

	enum class INSTANCING_SHADER {INSTANCING_SHADER_END};


}

namespace Client
{
#pragma region UI
	enum class UISTATE
	{
		READY,
		ENTER,
		LOOP,
		EXIT,
		PLAYER_HUD,
		TUTORIAL_BOX,
		DEIDSCREEN,
		STATE_END
	};

	enum class UITYPE
	{
		NONE,
		LEFT_HUD,
		RIGHT_HUD,
		PLAYER_HUD,
		LEVEL_UP,
		REWARD,
		QUESTBOX,
		TUTORIALBOX,
		CROSSHAIR,
		WEAKNESS,
		TYPE_END
	};
	enum class UI_BUTTON
	{
		NONE,

		// OPTION WINDOW 
			TITLE_PLAYER,
			TITLE_GAME,
			TITLE_INDEX_UP,
			TITLE_INDEX_DOWN,
			CLOSE_OPTION_WINDOW,
		// OPTION_WINDOW_END
		
		// GAME_OPTION
			/* SHADER */
			HBAO_PLUS,	 // ���� ���� �Ҽ���
			FOG,		
			RADIAL_BLUR, 
			DOF,		
			HDR,
			FXAA,
			HSV,		 // ���� ���� �Ҽ���
			SHADOW,
			LIGHT_SHAFT, // ���� - ������
			PBR,		 // ���� - ����ä�� ����� 
			SSR,		 // ���� - ����ä�� ����� 
			/* SHADER_END */
			/* SOUND */
			MASTER_SOUND,
			MUSIC_SOUND,
			EFFECT_SOUND,
			/* SOUND_END */
		// GAME_OPTION_END

		// PLAYER_EQUIPMENT
			WEAPON_GAUNTLET,
			WEAPON_REVOLVER,
			WEAPON_RIFLE,
			WEAPON_FLAMETHROWER,
			WEAPON_DYNAMITE,
		// PLAYER_EQUIPMENT_END

		BUTTON_END
	};

	enum class TUTORIAL_TEXT // Ʃ�丮�� �ؽ�Ʈ ��ȣ
	{
		START,					//				����
		CRANE,					//				ũ����
		ZOMBIE,					//				����
		JENU,					//				�������� ����
		ZOMBIE_OVER,			//				���񹫸�
		SNIPER,					//				��������
		SNOW_START,				//				�������
		TANK,					//				��ũ ����
		REWARD,					//				����
		TUTORIALTEXT_END		//				End
	};

	enum class SKILLSTATE // ��ų ����
	{
		LOCK,					//				
		COOLDOWN,				//
		ACTIVE,					//				
		SKILL_END				//				
	};
#pragma endregion End


	enum class MODE_COLOR // �÷� �ռ����
	{ 
		MUL,		// ���ϱ�		0
		SCREEN,		// ��ũ��		1
		OVERLAY,	// ��������		2
		ADD,		// ���ϱ�		3 
		BURN,		// ��(Burn)		4

		VIVID_RIGHT,	// ���� ����Ʈ 5
		SOFT_RIGHT,		// ����Ʈ ����Ʈ 6
		HARD_RIGHT,		// �ϵ� ����Ʈ 7

		COLOR_DODGE,	// �÷� ���� 8
		MIX_BURN,		// ȥ�� �� 9

		
		MODE_COLOR_END	//			10
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

		REVOLVER_UPGRADE,
		RIFLE_UPGRADE,
		SHOTGUN_UPGRADE,

		Additional_Weapon_End
	};

}
#endif // Client_Enum_h__
