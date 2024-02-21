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
		RENDER_UI_FRONT, RENDER_UI, RENDER_UI_BACK,	RENDER_END };

	enum SHADER_TYPE { SHADER_DEFERRED, SHADER_POSTPROCESSING, SHADER_BLUR, SHADER_OUTLINE, SHADER_FINAL, SHADER_END };
	
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
	void Set_RenderDebug(_bool _bRenderDebug) {	m_bRenderDebug = _bRenderDebug;	}
#endif

private:
	_float4x4					m_WorldMatrix;
	_float4x4					m_ViewMatrix, m_ProjMatrix;

private:
	
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_OutLine_PostProcessing();
	HRESULT Render_OutLineGroup();
	HRESULT Render_HBO_Plus();
	HRESULT Render_Bloom();

	HRESULT Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _int eHorizontalPass, _int eVerticalPass, _int eBlendType, _bool bClear);
	HRESULT Render_RadialBlur();
	HRESULT Render_GodRay();

	/* perlin�� �̿��� �ٴٹ���, ������, �� �� �ڿ������� ���������ϻ��� */

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

	/* Ȱ�� ���� */
private:
	_bool						m_bSSAO_Active			= { true };
	_bool						m_bBloom_Active			= { true };
	_bool						m_bOutline_Active		= { true };

public:
	void Set_SSAO(_bool _ssao_active)		{ m_bSSAO_Active = _ssao_active; } /* �ܰ��� �ɼ����� */
	void Set_Bloom(_bool _bloom_active)		{ m_bBloom_Active = _bloom_active; }
	void Set_OutLine(_bool _Outline_active) { m_bOutline_Active = _Outline_active; }

private:
	/* BLUR */
	HRESULT						Render_Blur_DownSample(const wstring& strStartTargetTag);
	HRESULT						Render_Blur_Horizontal(_int eHorizontalPass);
	HRESULT						Render_Blur_Vertical(_int eVerticalPass);
	HRESULT						Render_Blur_UpSample(const wstring& strFinalMrtTag, _bool bClear, _int eBlendType);
	void						Calc_Blur_GaussianWeights(_int sigma, _int iSize, _Out_ void* Weights);

	/* Radial Blur */
	_float4						m_fRadialBlurQuality = {};
	_float4						m_fRadialBlurPower = {};

	/* OutLine */
	_float4						m_vLineColor		= _float4(1.f, 0.f, 0.f, 1.f );

private:
	class CShader*					m_pShader[SHADER_TYPE::SHADER_END] = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };
	class CVIBuffer_Rect*			m_pVIBuffer = { nullptr };

	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	ID3D11DepthStencilView*			m_pLightDepthDSV = { nullptr };
	list<class CGameObject*>		m_RenderObjects[RENDER_END];

#ifdef _DEBUG
	list<class CComponent*>		m_DebugComponent;
	_bool						m_bRenderDebug = { false };
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