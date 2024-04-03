#ifndef Client_Enum_h__
#define Client_Enum_h__

namespace Engine
{
	enum class MODEL_SHADER {
		MODEL_ORIGIN,				// 0 - 원래 모델 그래도 Render
		MODEL_NODEOPTH,				// 1 - 깊이기록x -> SkyBox 같은것 용도 
		MODEL_SHADOW,				// 2 - 그림자 그리기 
		MODEL_CASCADE,				// 3 - CSM 
		MODEL_WHITEBLINK,			// 4 - 흰색으로 깜빡거림 
		MODEL_NONECULL_NONEDSS,		// 5
		MODEL_INTROBOSS_BLOODPOOL,  // 6 - Intro보스맵 바닥렌더용
		MODEL_NORMAL_MAPPING,		// 7 - 노말맵핑 한거 
		MODEL_DIFFUSE,				// 8 - Diffuse 컬러만 외부에서 던지는용도 
		MODEL_ALPHA,				// 9 - AlphaTexture 바인딩 
		MODEL_ICICLE,				// 10 - 고드름 얼음 모델들 렌더용
		MODEL_FOLIAGECLIP,			// 11 - 
		MODEL_OUTLINE_KEEP,			// 12 - OutLine 그대로 유지 
		MODEL_OUTLINE_BLINK,		// 13 - OutLine 깜빡거림
		MODEL_SHADER_END
	};

	enum class ANIM_SHADER {
		ANIM_ORIGIN,				// 0 - 기본형태
		ANIM_WIREFRAME,				// 1 - 메시로 보기 
		ANIM_SHADOW,				// 2 - 그림자
		ANIM_CASCADE,				// 3 - CSM
		ANIM_INTRO_BOSS,			// 4 - INTRO STAGE 보스 
		ANIM_EXAMPLE,				// 5 - Bloom , Rim 사용하는 예시
		ANIM_SHADER_END };

	enum class MONSTER_SHADER {
		COMMON_ORIGIN,				// 0 - 기본 그대로 그리기 
		COMMON_WIREFRAME,			// 1 - 빈깡통 
		COMMON_SHADOW,				// 2 - 그림자 그리기 
		COMMON_CASCADE,				// 3 - CSM
		COMMON_DISSOLVE,			// 4 - 공통 : 디졸브로 사라지는효과 
		COMMON_MESHCHECK,			// 5 - 공통 : 던지는 컬러로 해당하는 메시의 색을 바꿔서 메시가 몇번인덱스인지 체크하는용도 
		COMMON_OUTLINE_BLINK,		// 6 - 외곽선 깜빡거림 
		COMMON_OUTLINE_KEEP,		// 7 - 외곽선 계속 유지 
		INFECTED_PUNCH,				// 8 - Infected - Attack에서 주먹변하기 
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
			HBAO_PLUS,	 // 세기 조절 할수도
			FOG,		
			RADIAL_BLUR, 
			DOF,		
			HDR,
			FXAA,
			HSV,		 // 세기 조절 할수도
			SHADOW,
			LIGHT_SHAFT, // 예정 - 빼야함
			PBR,		 // 예정 - 없는채로 만들기 
			SSR,		 // 예정 - 없는채로 만들기 
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
#pragma endregion End


	enum class MODE_COLOR // 컬러 합성모드
	{ 
		MUL,		// 곱하기		0
		SCREEN,		// 스크린		1
		OVERLAY,	// 오버레이		2
		ADD,		// 더하기		3 
		BURN,		// 번(Burn)		4
		
		MODE_COLOR_END	//			5
	};



}
#endif // Client_Enum_h__
