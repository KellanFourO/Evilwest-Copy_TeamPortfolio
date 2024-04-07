#ifndef Client_Enum_h__
#define Client_Enum_h__

namespace Engine
{
	enum class MODEL_SHADER {
		MODEL_ORIGIN,				// 0 - ���� �� �׷��� Render
		MODEL_NODEOPTH,				// 1 - ���̱��x -> SkyBox ������ �뵵 
		MODEL_SHADOW,				// 2 - �׸��� �׸��� 
		MODEL_WHITEBLINK,			// 3 - ������� �����Ÿ� 
		MODEL_OUTLINE,				// 4
		MODEL_NONECULL_NONEDSS,		// 5
		MODEL_INTROBOSS_BLOODPOOL,  // 6 - Intro������ �ٴڷ�����
		MODEL_NORMAL_MAPPING,		// 7 - �븻���� �Ѱ� 
		MODEL_DIFFUSE,				// 8 - Diffuse �÷��� �ܺο��� �����¿뵵 
		MODEL_ALPHA,				// 9 - AlphaTexture ���ε� 
		MODEL_ICICLE,				// 10 - ��帧 ���� �𵨵� ������
		MODEL_FOLIAGECLIP,			// 11 - 
		MODEL_SHADER_END
	};

	enum class ANIM_SHADER {
		ANIM_ORIGIN,				// 0 - �⺻����
		ANIM_WIREFRAME,				// 1 - �޽÷� ���� 
		ANIM_SHADOW,				// 2 - �׸���
		ANIM_INTRO_BOSS,			// 3 - INTRO STAGE ���� 
		ANIM_EXAMPLE,				// 4 - Bloom , Rim ����ϴ� ����
		ANIM_SHADER_END };

	enum class MONSTER_SHADER {
		COMMON_ORIGIN,				// 0 - �⺻ �״�� �׸��� 
		COMMON_WIREFRAME,			// 1 
		COMMON_SHADOW,				// 2 - �׸��� �׸��� 
		INFECTED_PUNCH,				// 3 - Infected - Attack���� �ָԺ��ϱ� 
		COMMON_DISSOLVE,			// 4 - ���� : ������� �������ȿ�� 
		COMMON_MESHCHECK,			// 5 - ���� : ������ �÷��� �ش��ϴ� �޽��� ���� �ٲ㼭 �޽ð� ����ε������� üũ�ϴ¿뵵 
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
		PUNCH,					//				��ų 1��
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


	//enum class Player_Skill 
	//{
	//
	//};

}
#endif // Client_Enum_h__
