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
	enum class DEFERRED_SHADER { DEBUG, 
								 DIRECTIONAL_LIGHT, 
								 POINT_LIGHT, 
								 SPOT_LIGHT, 
								 DEFERRED, 
								 SHADER_END };

	enum class SSAO_SHADER { SSAO, GODRAY , SHADER_END};

	
	enum MINMAX { MIN, MAX, MINMAX_END };
	enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };

	enum EASING_TYPE
	{
		LINEAR

		, QUAD_IN, QUAD_OUT, QUAD_INOUT
		, CUBIC_IN, CUBIC_OUT, CUBIC_INOUT
		, QUART_IN, QUART_OUT, QUART_INOUT
		, QUINT_IN, QUINT_OUT, QUINT_INOUT
		, SINE_IN, SINE_OUT, SINE_INOUT
		, EXPO_IN, EXPO_OUT, EXPO_INOUT
		, CIRC_IN, CIRC_OUT, CIRC_INOUT
		, ELASTIC_IN, ELASTIC_OUT, ELASTIC_INOUT
		, BOUNCE_IN, BOUNCE_OUT
		, OUT_BACK, INOUT_BACK

		, EASING_TYPE_END
	};

}

#endif // Engine_Enum_h__
