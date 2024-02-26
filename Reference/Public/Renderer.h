#pragma once

#include "Base.h"

/* ȭ�鿡 �׷������� ��ü���� �׷����� ������� ��Ƴ��´�. */
/* ��Ƴ��� ������� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP 
	{ 
		/* Priority */
		RENDER_PRIORITY,RENDER_SHADOW, RENDER_NONLIGHT, 
		/* Post Processing  */
		RENDER_SSAO, RENDER_GODRAY, RENDER_OUTLINE,
		/* Blend */
		RENDER_NONBLEND, RENDER_BLEND, 
		/* UI */
		RENDER_UI,
		RENDER_NONBLEND_UI, /*RENDER_UI_MINIMAP, RENDER_UI_MINIMAP_ICON,*/
		RENDER_UI_EFFECT_NONBLEND, RENDER_UI_EFFECT_BLEND,
		RENDER_CURSOR,
		
		RENDER_END };

	enum SHADER_TYPE { SHADER_DEFERRED, SHADER_POSTPROCESSING, SHADER_BLUR, SHADER_OUTLINE, SHADER_FXAA, SHADER_FINAL, SHADER_DEFERRED_UI, SHADER_END };
	


private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eGroupID, class CGameObject* pGameObject);
	HRESULT Add_DebugRender(class CComponent* pDebugCom);
	HRESULT Pre_Setting(); /* Ŭ�󿡼� �ɼǰ� �ٲٴ¿뵵 */
	HRESULT Draw_RenderGroup();
	
	/* Ready */
	HRESULT Create_Buffer();
	HRESULT	Create_Shader();  
	HRESULT Create_RenderTarget();
	HRESULT Create_DepthStencil();
	HRESULT Ready_DebugRender();

	
#ifdef _DEBUG
public:
	void	Set_RenderDebug(_bool _bRenderDebug) {	m_bRenderDebug = _bRenderDebug;	}
	HRESULT	Control_HotKey();
#endif

private:
	_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();

	/* Pre-Post Processing */
	HRESULT Render_OutLine_PostProcessing();
	HRESULT Render_HBAO_PLUS();
	HRESULT Render_Bloom();
	HRESULT Render_Deferred();

	/* Post Processing */
	HRESULT Render_PostProcess();
	HRESULT Render_RadialBlur();
	HRESULT Render_GodRay();
	HRESULT Render_FXAA();
	HRESULT Render_HDR();

	/* ���� �� �׸��°� */
	HRESULT Render_Final();

	/* Common*/
	HRESULT Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _int eHorizontalPass, _int eVerticalPass, _int eBlendType, _bool bClear);

	HRESULT Render_Blend();

	HRESULT Render_LightAcc();
	HRESULT Render_OutLineGroup(); /* RenderGroup*/
#pragma region
	/* MRT */
	HRESULT Add_MRT_UI();

	/* Group */
	HRESULT Render_UI();
	HRESULT Render_Text();

	HRESULT Render_NonBlend_UI();
	HRESULT Render_Lights_UI();

	/* OutLine */
	HRESULT Render_OutLine_UI();

	HRESULT Render_Deferred_UI();
	HRESULT Render_UI_Minimap();
	HRESULT Render_UI_Minimap_Icon();
	HRESULT Render_UIEffectNonBlend();
	HRESULT Render_UIEffectBlend();

	HRESULT Render_Screen_Effect();
	HRESULT Render_UI_Final();
	HRESULT Render_Cursor();
#pragma endregion

	/* perlin�� �̿��� �ٴٹ���, ������, �� �� �ڿ������� ���������ϻ��� */

private: /* !UI */
	HRESULT Ready_UI_Target(D3D11_VIEWPORT Viewport);

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

	/* Ȱ�� ���� */
private:
	_bool						m_bInit					= { true }; /* ������ ���� �ǵ��������� */
	_bool						m_bSSAO_Active			= { false };
	_bool						m_bBloom_Active			= { false };
	_bool						m_bOutline_Active		= { false };
	_bool						m_bPBR_Active			= { false };
	_bool						m_bFXAA_Active			= { false };
	_bool						m_bHDR_Active			= { false };
	_bool						m_bFog_Active			= { false };
	_bool						m_bRim					= { false };

private:
	HBAO_PLUS_DESC				m_tHBAO_Option			= {};
	FOG_DESC					m_tFog_Option			= {};
	HDR_DESC					m_tHDR_Option			= {};
	SCREEN_DESC					m_tScreen_Option		= {};

	/* BLUR */
	HRESULT						Render_Blur_DownSample(const wstring& strStartTargetTag);
	HRESULT						Render_Blur_Horizontal(_int eHorizontalPass);
	HRESULT						Render_Blur_Vertical(_int eVerticalPass);
	HRESULT						Render_Blur_UpSample(const wstring& strFinalMrtTag, _bool bClear, _int eBlendType);
	void						Calc_Blur_GaussianWeights(_int sigma, _int iSize, _Out_ void* Weights);

	/* HDR */
	_float						Final_Brightness = 1.f;
	_float						Final_Saturation = 1.f;

	/* Radial Blur */
	_float4						m_fRadialBlurQuality = {};
	_float4						m_fRadialBlurPower = {};

	/* OutLine */
	_float4						m_vLineColor		= _float4(1.f, 0.f, 0.f, 1.f );

	/* HDR */
	_float						m_max_white = { 0.3f };

	/* Fog */ 
	FOG_DESC					m_CurrFog = {};

public:
	/* Ȱ��ȭ */
	void Set_SSAO(_bool _ssao_active) { m_bSSAO_Active = _ssao_active; } 
	void Set_Bloom(_bool _bloom_active) { m_bBloom_Active = _bloom_active; }
	void Set_OutLine(_bool _Outline_active) { m_bOutline_Active = _Outline_active; }
	void Set_HDR(_bool _HDR_active) { m_bHDR_Active = _HDR_active; }
	void Set_FXAA(_bool _FXAA_active) { m_bFXAA_Active = _FXAA_active; }
	void Set_RimLight(_bool _RimLight) { m_bRim = _RimLight; }
	void Set_Fog(_bool _Fog) { m_bFog_Active = _Fog; }

	/* �ɼ����� */
	void Set_HBAO_Option(HBAO_PLUS_DESC desc) {	m_tHBAO_Option = desc; }
	void Set_Fog_Option(FOG_DESC desc) { m_tFog_Option = desc; }
	void Set_HDR_Option(HDR_DESC desc) { m_tHDR_Option = desc; }
	void Set_Screen_Option(SCREEN_DESC desc) { m_tScreen_Option = desc; }

private:
	class CShader*					m_pShader[SHADER_TYPE::SHADER_END]	= { nullptr };
	class CTexture*					m_pPerlinNoiseTextureCom			= { nullptr };
	class CGameInstance*			m_pGameInstance						= { nullptr };
	class CVIBuffer_Rect*			m_pVIBuffer							= { nullptr };

	ID3D11Device*					m_pDevice							= { nullptr };
	ID3D11DeviceContext*			m_pContext							= { nullptr };
	ID3D11DepthStencilView*			m_pLightDepthDSV					= { nullptr };
	list<class CGameObject*>		m_RenderObjects[RENDER_END];
	D3D11_VIEWPORT					m_Viewport = {};

#ifdef _DEBUG
	list<class CComponent*>		m_DebugComponent;
	_bool						m_bRenderDebug = { true };
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

/*
* Blur : ��ü / ������ �帮�ų� �ѿ��� ������۾� = ���̰� �ø��°�����ü�� ���ϳ��� ��ȿ���� �� ���� ����� ����. 
	ȭ�鿡 �ؽ��ĸ� �׸��� 
	-> �ؽ��ĸ� ���� �Ǵ� �� ���Ϸ� �ٿ���ø��Ѵ� (�ΰ��� �ﰢ������ �̷���� 2D & 256 Ȥ�� ȭ���� ����  
	-> ���õ� �ؽ��Ŀ� ������� �����Ѵ� : ������ �ȼ����� ��������� ���ϴ°��̴�. 
	-> �������� �����Ѵ� 
	-> ���� ȭ�� ������� ���ø��Ѵ� 
	-> ȭ�鿡 �ؽ��ĸ� �׸���. 
	
	(+) ����ġ ������ ���� ������ ������ �� �ִ�. 
		- ����ġ�� ������ �ֺ��ȼ��� �⿩�� �������� ���� ȿ���� ��������. 
		- ����ġ�� ���ٸ� �ֺ��ȼ��� �⿩�� ������ �� ���� ��ȿ���� ��Ÿ����. 
*/

/*
*/

/* SSAO 

	struct QuadVertex // ssao
	{
		_float3 pos;
		_float3 normal;
		_float2 tex;
	};

	typedef struct tagXMCOLOR
	{
		union {
			struct {
				uint8_t b;
				uint8_t g;
				uint8_t r;
				uint8_t a;
			};
			uint32_t c;
		};
	}XMCOLOR;

HRESULT RenderScreenQuad();
class CTexture*				m_pRandomVectorTexture = { nullptr };
	class CVIBuffer_SSAO*			m_pSSAO_VIBuffer = { nullptr };
ID3D11Buffer*				m_ScreenQuadVB = { nullptr };
ID3D11Buffer*				m_ScreenQuadIB = { nullptr };
ID3D11ShaderResourceView*	m_RandomVectorSRV;
//SSAO_Data					m_tSSAO_Data;
const _matrix				m_mTexture = {	XMMatrixSet(0.5f, 0.0f, 0.0f, 0.0f,	0.0f, -0.5f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,	0.5f, 0.5f, 0.0f, 1.0f) };
_float4						m_vFrustumFarCorner[4];
_float4						m_vOffsets[14];
_float						m_OffsetsFloat[56];
_int						m_iQuadVerCount;
_int						m_iQuadIndexCount;
HRESULT Ready_SSAO();

	HRESULT Render_SSAO();
	HRESULT Render_SSAO_Blur();
HRESULT						SSAO_OnSize();
HRESULT						BuildFullScreenQuad();
void						BuildOffsetVectors();
void						BuildRandomVectorTexture();
*/