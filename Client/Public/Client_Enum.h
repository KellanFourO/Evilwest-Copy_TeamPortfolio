#ifndef Client_Enum_h__
#define Client_Enum_h__

namespace Client
{
	enum class MODEL_SHADER { 
		MODEL_ORIGIN,				// 0 - ���� �� �׷��� Render
		MODEL_NODEOPTH,				// 1 - ���̱��x -> SkyBox ������ �뵵 
		MODEL_SHADOW,				// 2 - �׸��� �׸��� 
		MODEL_WHITEBLINK,			// 3 - ������� �����Ÿ� 
		MODEL_OUTLINE,				// 4 - �ܰ��� ��� : �β��� Ŭ�󿡼� ���� 
		MODEL_NONECULL_NONEDSS,		// 5 - �¿��߰� 
		MODEL_SHADER_END
	};

	enum class ANIM_SHADER {
		ANIM_ORIGIN,				// 0 - �⺻����
		ANIM_WIREFRAME,				// 1 - WireFrame 
		ANIM_SHADOW,				// 2 - �׸���
		ANIM_DISSOLVE,				// 3 - ������
		ANIM_DISTORTION,			// 4 - �ְ�
		ANIM_GRAYSCALE,				// 5 - ȸ���� : �װų� ������
		ANIM_MASKING,				// 6 - ����ŷ�ؽ��ķ� �ڸ��� 
		ANIM_BLOOM,					// 7 - BLOOM + BLUR 
		ANIM_OUTLINE,				// 8 - �ܰ��� ���̱� : �β��� Ŭ�󿡼� ���� 
		ANIM_SHADER_END };

	enum class UI_SHADER {UI_SHADER_END};

	enum class INSTANCING_SHADER {INSTANCING_SHADER_END};

	enum class INSTANCING_ANIM_SHADER { 
		INSTANCING_ANIM_ORIGIN,					// - �⺻ ���� 
		INSTANCING_ANIM_SHADER_END};			

#pragma region UI
	enum class UISTATE
	{
		READY,
		APPEAR,
		TICK,
		DISAPPEAR,
		STATE_END
	};
#pragma endregion End
}
#endif // Client_Enum_h__
