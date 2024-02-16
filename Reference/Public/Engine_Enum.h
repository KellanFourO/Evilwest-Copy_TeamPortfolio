#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	/* Input Device */
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	/* Transform */
	static const wchar_t* g_pTransformTag = TEXT("Com_Transform");

	/* Shader */
	enum class DEFERRED_SHADER { 
		DEBUG, 
		DIRECTIONAL_LIGHT, 
		POINT_LIGHT, 
		SPOT_LIGHT, 
		DEFERRED, 
		SHADER_END };

	enum class SSAO_SHADER { 
		SSAO, 
		GODRAY , 
		SHADER_END};

	enum class BLUR_SHADER { 
		BLUR_DOWN, 
		BLUR_UP,
		BLUR_HORIZON,
		BLUR_VERTICAL,
		BLUR_END 
		};
}

#endif // Engine_Enum_h__
