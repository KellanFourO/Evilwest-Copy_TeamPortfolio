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
		COMMON_WIREFRAME,			// 1 - 
		COMMON_SHADOW,				// 2 - �׸��� �׸��� 
		INFECTED_PUNCH,				// 3 - Infected - Attack���� �ָԺ��ϱ� 
		SNIPER_WEAPON,				// 4 - �������� ���� 
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
		STATE_END
	};
	enum class UITYPE
	{
		NONE,
		LEFT_HUD,
		RIGHT_HUD,
		LEVEL_UP,
		REWARD,
		QUESTBOX,
		TUTORIALBOX,
		CROSSHAIR,
		WEAKNESS,
		TYPE_END
	};
#pragma endregion End
}
#endif // Client_Enum_h__
