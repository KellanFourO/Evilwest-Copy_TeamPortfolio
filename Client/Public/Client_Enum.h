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

	enum class ANIM_SHADER {ANIM_SHADER_END };

	enum class UI_SHADER {UI_SHADER_END};

	enum class INSTANCING_SHADER {INSTANCING_SHADER_END};


}

#endif // Client_Enum_h__