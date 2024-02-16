#pragma once

#include "Base.h"

/* 화면에 그려져야할 객체들을 그려야할 순서대로 모아놓는다. */
/* 모아놓은 순서대로 객체들의 렌더함수를 호출한다.(렌더콜) */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SHADOW, RENDER_NONLIGHT, 
					   
						/* Shader */
					  RENDER_SSAO, RENDER_GODRAY, RENDER_OUTLINE,
					   
					   RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_END };

	enum SHADER_TYPE { SHADER_DEFERRED, SHADER_POSTPROCESSING, SHADER_BLUR, SHADER_FINAL, SHADER_END };
	
	struct QuadVertex // ssao 
	{
		_float3 pos;
		_float3 normal;
		_float2 tex;
	};
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eGroupID, class CGameObject* pGameObject);
	HRESULT Add_DebugRender(class CComponent* pDebugCom);
	HRESULT Draw_RenderGroup();
	
	/* Ready */
	HRESULT Create_Buffer();
	HRESULT	Create_Shader();
	HRESULT Create_RenderTarget();
	HRESULT Create_DepthStencil();
	HRESULT Ready_DebugRender();
	HRESULT Ready_SSAO();

	/* SSAO */
	HRESULT SSAO_OnSize();
	HRESULT BuildFullScreenQuad();
	void	BuildOffsetVectors();
	void	BuildRandomVectorTexture();

	HRESULT RenderScreenQuad();

#ifdef _DEBUG
public:
	void Set_RenderDebug(_bool _bRenderDebug) {	m_bRenderDebug = _bRenderDebug;	}
#endif

private:
	class CShader*				m_pShader[SHADER_TYPE::SHADER_END] = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CVIBuffer_Rect*		m_pVIBuffer = { nullptr };

	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	ID3D11DepthStencilView*		m_pLightDepthDSV = { nullptr };
	list<class CGameObject*>	m_RenderObjects[RENDER_END];

#ifdef _DEBUG
	list<class CComponent*>		m_DebugComponent;
	_bool						m_bRenderDebug = { true };
#endif

private:
	_float4x4					m_WorldMatrix;
	_float4x4					m_ViewMatrix, m_ProjMatrix;
	_float4						m_vLineColor = { 1.f, 1.f, 1.f, 1.f };

private: 
	/* SSAO */
	class CTexture*				m_pRandomVectorTexture = { nullptr };
	ID3D11Buffer*				m_ScreenQuadVB = { nullptr };
	ID3D11Buffer*				m_ScreenQuadIB = { nullptr };
	ID3D11ShaderResourceView*	m_RandomVectorSRV;
	_float4						m_vFrustumFarCorner[4];
	_float4						m_vOffsets[14];
	_float						m_OffsetsFloat[56];
	_int						m_iQuadVerCount;
	_int						m_iQuadIndexCount;
	_float						RandF() {	return (float)(rand()) / (float)RAND_MAX; }
	_float						RandF(float a, float b) { return a + RandF() * (b - a); };

	/* ssao_blur */
	HRESULT						Render_Blur_DownSample(const wstring& strStartTargetTag);
	HRESULT						Render_Blur_Horizontal(_int eHorizontalPass);
	HRESULT						Render_Blur_Vertical(_int eVerticalPass);
	HRESULT						Render_Blur_UpSample(const wstring& strFinalMrtTag, _bool bClear, _int eBlendType);

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();

	HRESULT Render_OutLine();
	HRESULT Render_SSAO();
	HRESULT Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _int eHorizontalPass, _int eVerticalPass, _int eBlendType, _bool bClear);
	HRESULT Render_GodRay();


public:
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

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

/*
Blur : 전체 / 개별을 흐리거나 뿌옇게 만드는작업 = 줄이고 늘리는과정자체가 또하나의 블러효과라서 더 좋은 결과를 낸다. 
	화면에 텍스쳐를 그린다 
	-> 텍스쳐를 절반 또는 그 이하로 다운샘플링한다 (두개의 삼각형으로 이루어진 2D & 256 혹은 화면의 절반  
	-> 샘플된 텍스쳐에 수평블러를 수행한다 : 인접한 픽셀들의 가중평균을 구하는것이다. 
	-> 수직블러를 수행한다 
	-> 원래 화면 사이즈로 샘플링한다 
	-> 화면에 텍스쳐를 그린다. */