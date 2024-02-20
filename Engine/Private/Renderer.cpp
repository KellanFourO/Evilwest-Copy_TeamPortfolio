#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_SSAO.h"
#include "AlphaObject.h"
#include "Shader.h"
#include "SMath.h"
#include "RenderTarget.h"

_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	/* ���̴� ��ü ���� */
	FAILED_CHECK(Create_Shader());

	/* ����Ÿ�� ���� + MRT �׷� ���� */
	FAILED_CHECK(Create_RenderTarget());

	/* ���� ��ü ���� */
	FAILED_CHECK(Create_Buffer());

	/* ID3D11DepthStencilView �Ļ� */
	FAILED_CHECK(Create_DepthStencil());

#ifdef _DEBUG
	/* �ռ� ���� MRT�� ����׷����� �׸��¿뵵 */
	FAILED_CHECK(Ready_DebugRender()); 
#endif

	/* ssao - PostProcessing */
	FAILED_CHECK(Ready_SSAO());

	return S_OK;
}

#pragma region Renderer_Initialize

HRESULT CRenderer::Create_Buffer()
{
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pSSAO_VIBuffer = CVIBuffer_SSAO::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pSSAO_VIBuffer, E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Create_Shader()
{
	m_pShader[SHADER_TYPE::SHADER_DEFERRED] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	NULL_CHECK_RETURN(m_pShader[SHADER_TYPE::SHADER_DEFERRED], E_FAIL);

	m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_PostProcess.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements);
	NULL_CHECK_RETURN(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING], E_FAIL);

	m_pShader[SHADER_TYPE::SHADER_BLUR] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Blur.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	NULL_CHECK_RETURN(m_pShader[SHADER_TYPE::SHADER_BLUR], E_FAIL);
	
	m_pShader[SHADER_TYPE::SHADER_OUTLINE] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Outline.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	NULL_CHECK_RETURN(m_pShader[SHADER_TYPE::SHADER_OUTLINE], E_FAIL);

	//m_pShader[SHADER_TYPE::SHADER_FINAL] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Final.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	//NULL_CHECK_RETURN(m_pShader[SHADER_TYPE::SHADER_FINAL], E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Create_RenderTarget()
{
	D3D11_VIEWPORT		Viewport;
	_uint				iNumViewport = { 1 };

	m_pContext->RSGetViewports(&iNumViewport, &Viewport);

	/* RenderTarget */
	{
		/* MRT_GameObject */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f)));
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f)));
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))); /* ���̹��� �� ���� */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_ORM"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_ViewNormal"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))); 
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))); /* ������ �˾Ƽ� ���ǵ��� ó���� */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_OutLine"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 0.f)));

		/* MRT_Shadow */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

		/* MRT_LightAcc */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightDepth"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f)));

		/* MRT_SSAO	*/
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_SSAO"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f)));
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_SSAO_Blur"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f)));
		{
			FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_DownSampling"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
			FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Horizontal"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
			FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Vertical"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
			FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_UpSampling"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
		}

		/* MRT_HBAO+ */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_HBAO"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f)));

		/* MRT_Bloom_Blur*/
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_Blur"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));

		/* MRT_GodRay */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_GodRay"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	
		/* MRT_RadialBlur */
		FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_RadialBlur"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	}

	/* MRT*/
	{
		/* MRT_GameObject */
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_ORM")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_ViewNormal")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Bloom"))); /* ������Ʈ �ܰ��� */

		/* MRT_OutLine */
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Outline"), TEXT("Target_OutLine")));

		/* MRT_LightAcc - Q. Ambient �߰��ϴ°� ? */
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular")));

		/* MRT_Shadow */
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_LightDepth")));

		/* MRT_SSAO	*/
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_SSAO"), TEXT("Target_SSAO")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_SSAO_Blur"), TEXT("Target_SSAO_Blur")));

		/* Blur -  ���������� ����ϴ� ���� �ϳ��� �����ϱ����� Target_Blur_DownSampling�� g_�� ���� �Ѱܼ� �ϳ��� ����ϱ����� ������*/
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_DownSampling"), TEXT("Target_Blur_DownSampling")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Horizontal"), TEXT("Target_Blur_Horizontal")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Vertical"), TEXT("Target_Blur_Vertical")));
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_UpSampling"), TEXT("Target_Blur_UpSampling")));

		/* MRT_Bloom_Blur*/
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_Blur"), TEXT("Target_Bloom_Blur")));

		/* MRT_GodRay */
		//FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_GodRay"), TEXT("Target_GodRay")));

		/* MRT_RadialBlur*/
		FAILED_CHECK(m_pGameInstance->Add_MRT(TEXT("MRT_RaidalBlur"), TEXT("Target_RadialBlur")));

	}

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(Viewport.Width, Viewport.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));
	//XMStoreFloat4x4(&m_MinimapProj, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1600.f / 900.f, 0.2f, 1000.0f));
	
	return S_OK;
}

HRESULT CRenderer::Create_DepthStencil()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture));

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	FAILED_CHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pLightDepthDSV));

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Ready_DebugRender()
{
	_float fSizeX = 150.f;
	_float fSizeY = 100.f;

	/* MRT_GameObject - �»�� Default */
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Diffuse"),		(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 1.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Normal"),		(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 3.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Depth"),		(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 5.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_ORM"),			(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 7.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Bloom"),		(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 9.f),  fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Bloom_Blur"),	(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 11.f),  fSizeX, fSizeY));
	//FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_OutLine"),		(fSizeX / 2.f * 1.f), (fSizeY / 2.f * 11.f),  fSizeX, fSizeY));

	/* MRT_LightAcc */
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_LightDepth"),	(fSizeX / 2.f * 3.f), (fSizeY / 2.f * 1.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Shade"),		(fSizeX / 2.f * 3.f), (fSizeY / 2.f * 3.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Specular"),		(fSizeX / 2.f * 3.f), (fSizeY / 2.f * 5.f), fSizeX, fSizeY));

	/* MRT_Pro */ 
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_ViewNormal"),	(fSizeX / 2.f * 5.f), (fSizeY / 2.f * 1.f), fSizeX, fSizeY));
	FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_SSAO"),			(fSizeX / 2.f * 5.f), (fSizeY / 2.f * 3.f), fSizeX, fSizeY));
	//FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_SSAO_Blur"),	(fSizeX / 2.f * 5.f), (fSizeY / 2.f * 5.f), fSizeX, fSizeY));

	//FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_GodRay"),		(fSizeX / 2.f * 7.f), (fSizeY / 2.f * 1.f), fSizeX, fSizeY));

	/* MRT_Shadow - ���� Default */
	//FAILED_CHECK(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_LightDepth"),	(g_iWinsizeX - fSizeX * 0.5f), (fSizeY / 2.f * 1.f), fSizeX, fSizeY));

	return S_OK;
}

#endif // _DEBUG

#pragma endregion

HRESULT CRenderer::Draw_RenderGroup()
{
	FAILED_CHECK(Render_Priority());	/* SkyBox */
	FAILED_CHECK(Render_Shadow());		/* MRT_Shadow */
	FAILED_CHECK(Render_NonLight()); 
	FAILED_CHECK(Render_NonBlend());	/* MRT_GameObjects*/
	FAILED_CHECK(Render_LightAcc());	/* MRT_LightAcc */

	{ /* PostProcessing */
		if (true == m_bSSAO_Active)
		{
			FAILED_CHECK(Render_SSAO());
		}
		else
			FAILED_CHECK(Render_HBAO_Plus());

		if (true == m_bBloom_Active)
		{
			FAILED_CHECK(Render_Bloom());
		}

		if (true == m_bOutline_Active)
		{
			FAILED_CHECK(Render_OutLine_PostProcessing());
		}
	}

	FAILED_CHECK(Render_Deferred());
	FAILED_CHECK(Render_OutLineGroup());	/* Render_Group */
	FAILED_CHECK(Render_Blend());
	FAILED_CHECK(Render_UI());

#ifdef _DEBUG
	if(true == m_bRenderDebug)
		FAILED_CHECK(Render_Debug());
#endif

	return S_OK;
}

#pragma region RenderCall

#pragma region RenderGroup

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), m_pLightDepthDSV));

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)g_iSizeX;
	ViewPortDesc.Height = (_float)g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	for (auto& pGameObject : m_RenderObjects[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_SHADOW].clear();

	FAILED_CHECK(m_pGameInstance->End_MRT());

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = g_iWinsizeX;
	ViewPortDesc.Height = g_iWinsizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	/* ������ ���õǾ��ִ� ����۸� ������ Diffuse�� Normal�� ��ġ�� ���ε��Ѵ�. */
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects")));

	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	/* ����۸� ���� ��ġ�� �ٽ� ��ġ�� ���ε��Ѵ�. */
	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	m_RenderObjects[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return ((CAlphaObject*)pSour)->Get_CamDistance() > ((CAlphaObject*)pDest)->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

#pragma endregion

HRESULT CRenderer::Render_LightAcc()
{
	/* Shade */
	/* ������ ���� ���� ����� ������ �ش�. */
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc")));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformFloat4x4Inverse(CPipeLine::D3DTS_VIEW)));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformFloat4x4Inverse(CPipeLine::D3DTS_PROJ)));

	_float		CamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_CamFar", &CamFar, sizeof(_float)));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4)));

	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Diffuse"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_DiffuseTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Normal"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_NormalTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_DepthTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_ORM"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_ORMTexture"));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_PBR_Active", &m_bPBR_Active, sizeof(_bool)));

	m_pGameInstance->Render_Lights(m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	
	/* 0��°�� ����۷���Ÿ���� �ö󰬴�. */
	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	/* ���۵忡 ���� ������� */
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));
	
	_float			CamFar = m_pGameInstance->Get_CamFar();
	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(-20.f, 20.f, -20.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), g_iWinsizeX / g_iWinsizeY, 0.1f, CamFar));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_LightViewMatrix", &ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_LightProjMatrix", &ProjMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_CamFar", &CamFar, sizeof(_float)));

	/* MRT_GameObject */
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Diffuse"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_DiffuseTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Normal"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_NormalTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_ViewNormal"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_NormalDepthTarget"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_DepthTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Shade"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_ShadeTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Specular"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_SpecularTexture"));
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_LightDepth"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_LightDepthTexture"));
	//FAILED_CHECK(m_pGameInstance->Add_RenderTarget(TEXT("Target_ORM"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	/* Post Processing */
	{
		if (true == m_bPBR_Active) /* �ӽ÷� �Ѵ� ���ϵ��� ���¿뵵���� */
		{
			FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bSSAO_Active", &m_bSSAO_Active, sizeof(_bool)));
			if (true == m_bSSAO_Active)
			{
				//FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_SSAO"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_SSAOTexture"));
				FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_SSAO_Blur"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_SSAOTexture")); /* ssao �߰� */
			}
			else
			{
				FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_HBAO"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_SSAOTexture"));
			}
		}

		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bBloom_Active", &m_bBloom_Active, sizeof(_bool)));
		if (true == m_bBloom_Active)
		{
			FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Bloom_Blur"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_BloomTarget"));
		}

		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_Outline_Active", &m_bOutline_Active, sizeof(_bool)));
		if (true == m_bOutline_Active)
		{
			m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Outline"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_OutlineTarget");
		}
	}

	m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Begin(ECast(DEFERRED_SHADER::DEFERRED));

	m_pVIBuffer->Bind_VIBuffers();

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_OutLine_PostProcessing()
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_Outline"))); 

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	{
		FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Normal"), m_pShader[SHADER_TYPE::SHADER_OUTLINE], "g_NormalTarget"));

		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_OUTLINE]->Bind_RawValue("g_vLineColor", &m_vLineColor, sizeof(_float4)));
	}

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_OUTLINE]->Begin(0));

	//FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	/* SSAO : crysis ������ ���� crytek���� ������ �ֺ� ����� �ٻ�ȭ�ϴ� �������. 
		 * ����� �� �ȼ��� ���� ���� �� ���������� �����ϴ� ������� ����� �׸���. 
		 * �� ������ ���ø��Ͽ� �� �ȼ��� ������� ����� ���۵� ���̴�ȿ������ ��������� ���ø��Ͽ� �����꿡�� �ֺ��׸��� ������ �� �ֵ����Ѵ�. */

	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_SSAO"))); /* Target SSAO �ܵ� -> Blur �����ؾ��� */

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	/* ���� �ø��� */
	{
		/* matViewToTEXsPACE*/
		_matrix P = m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE::D3DTS_PROJ);
		_matrix PT = XMMatrixMultiply(P, m_mTexture);
		_float4x4 ViewToTexSpcace = {};
		XMStoreFloat4x4(&ViewToTexSpcace, PT);
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_Matrix("ViewToTexSpcace", &ViewToTexSpcace));

		/* Offset */
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_RawValue("g_OffsetVector", &m_vOffsets, sizeof(_float4) * 14));
		//FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_
		
		/* Frustum*/
		SSAO_OnSize();
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_RawValue("FrustumCorner", &m_vFrustumFarCorner, sizeof(_float4) * 4));
		
		FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_ViewNormal"), m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING], "g_NormalDepthTarget"));
		
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Bind_Texture("g_RandomVectorTexture", m_pRandomSRV));

		//FAILED_CHECK(m_pRandomVectorTexture->Bind_ShaderResource(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING], "g_RandomVectorTexture"));
		
		
	}

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_POSTPROCESSING]->Begin(ECast(SSAO_SHADER::SSAO)));
	
	FAILED_CHECK(RenderScreenQuad());

	FAILED_CHECK(m_pSSAO_VIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	Render_Blur(L"Target_SSAO", L"MRT_SSAO_Blur", true, ECast(BLUR_SHADER::BLUR_HORIZON_QUARTER), ECast(BLUR_SHADER::BLUR_VERTICAL_QUARTER), ECast(BLUR_SHADER::BLUR_UP_ADD));

	return S_OK;
}

HRESULT CRenderer::Render_HBAO_Plus()
{
	GFSDK_SSAO_InputData_D3D11 Input;
	Input.DepthData.DepthTextureType = GFSDK_SSAO_HARDWARE_DEPTHS;
	Input.DepthData.pFullResDepthTextureSRV = m_pGameInstance->Get_DepthSRV();

	_matrix ProjMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	Input.DepthData.ProjectionMatrix.Data = GFSDK_SSAO_Float4x4((const GFSDK_SSAO_FLOAT*)&ProjMatrix);
	Input.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.DepthData.MetersToViewSpaceUnits = 1.f;

	GFSDK_SSAO_Parameters Params;
	Params.Radius = 2.f;
	Params.Bias = 0.1f;
	Params.PowerExponent = 2.f;
	Params.Blur.Enable = true;
	Params.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;
	Params.Blur.Sharpness = 16.f;

	ID3D11RenderTargetView* pView = {};
	pView = m_pGameInstance->Find_RenderTarget(TEXT("Target_HBAO"))->Get_RTV();
	NULL_CHECK_RETURN(pView, E_FAIL);

	GFSDK_SSAO_Output_D3D11 Output;
	Output.pRenderTargetView = pView;
	
	Output.Blend.Mode = GFSDK_SSAO_OVERWRITE_RGB;

	GFSDK_SSAO_Status status;
	status = m_pGameInstance->Get_AOContext()->RenderAO(m_pContext, Input, Params, Output);
	assert(status == GFSDK_SSAO_OK);

	return S_OK;
}

HRESULT CRenderer::Render_Bloom()
{
	FAILED_CHECK(Render_Blur(L"Target_Bloom", L"MRT_Bloom_Blur", true, ECast(BLUR_SHADER::BLUR_HORIZON_HIGH), ECast(BLUR_SHADER::BLUR_VERTICAL_HIGH), ECast(BLUR_SHADER::BLUR_UP_ADD)));
	
	return S_OK;
}

HRESULT CRenderer::Render_RadialBlur()
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_RaidalBlur"))); 

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	/* ���� �ø��� */
	{
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_fQuality", &m_fRadialBlurQuality, sizeof(_float4)));
		FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_fRadialPower", &m_fRadialBlurPower, sizeof(_float4)));
	}

	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Blend"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], "g_BlendTarget"));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Begin(ECast(DEFERRED_SHADER::SHADER_END)));

	FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	//FAILED_CHECK(Render_AlphaBlendTargetMix(L"Target_RadialBlur", L"MRT_Blend", true))) 
	//-> mix ��Ű�� deferred���̴��� �� �ٸ����� ������ render -> clear�� 

	return S_OK;
}

HRESULT CRenderer::Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _int eHorizontalPass, _int eVerticalPass, _int eBlendType, _bool bClear)
{
	FAILED_CHECK(Render_Blur_DownSample(strStartTargetTag));

	FAILED_CHECK(Render_Blur_Horizontal(eHorizontalPass));

	FAILED_CHECK(Render_Blur_Vertical(eVerticalPass));

	FAILED_CHECK(Render_Blur_UpSample(strFinalTragetTag, bClear, eBlendType));

	return S_OK;
}

HRESULT CRenderer::Render_GodRay()
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_GodRay")));

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_DebugOnOff()
{
	if (m_pGameInstance->Key_Down(DIK_GRAVE))
	{
		if (true == m_bPBR_Active)
		{
			if (true == m_bSSAO_Active)
				cout << "SSAO : true " << endl;
			else
				cout << "HBAO+ : false " << endl;
		}
		else
		{
			cout << " NO SSAO HOB+ " << endl;

		}
		if (true == m_bBloom_Active)
			cout << "BloomBlur : true " << endl;
		else
			cout << "BloomBlur : false " << endl;
		/* --------------------------------- */
		cout << "OutLine : Test�� " << endl;

		cout << " ----------------------------- " << endl;
	}

	return S_OK;
}

HRESULT CRenderer::Render_OutLineGroup()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_OUTLINE])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_OutLine();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_OUTLINE].clear();

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader[SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	for (auto& pDebugCom : m_DebugComponent)
	{
		pDebugCom->Render();
		//AnimTool �۾��߿� �� ������ �����ÿ� �������� 
		Safe_Release(pDebugCom);
	}
	m_DebugComponent.clear();

	m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_GameObjects"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_LightAcc"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_Shadow"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_SSAO"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	//m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_SSAO_Blur"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	//m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_Outline"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);
	m_pGameInstance->Render_Debug_RTVs(TEXT("MRT_Bloom_Blur"), m_pShader[SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);

	return S_OK;
}
#endif

#pragma endregion

#pragma region ssao_initialize

HRESULT CRenderer::Ready_SSAO()
{
	/* ssao ��ü ���� 
		: SSAO ��ü�� ������� Direct3D ��ġ, DeviceContext, ȭ�� ũ��, ī�޶� fov �� ī�޶� ���Ÿ� ��� �Ÿ��� �����ؾ��Ѵ�. */
	FAILED_CHECK(BuildFullScreenQuad());

	FAILED_CHECK(SSAO_OnSize());

	BuildOffsetVectors();

	//BuildRandomVectorTexture();

	m_pRandomVectorTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Shader/RandomNormalTexture.jpg"), 1);
	NULL_CHECK_RETURN(m_pRandomVectorTexture, E_FAIL);

	m_pRandomSRV = m_pRandomVectorTexture->Get_SRV();
	NULL_CHECK_RETURN(m_pRandomSRV, E_FAIL);

	Safe_AddRef(m_pRandomSRV);
	Safe_Release(m_pRandomVectorTexture);

	return S_OK;
}

HRESULT CRenderer::SSAO_OnSize()
{
	/* + BuildFrustumFarCorners */
	_float4 CamSetting = m_pGameInstance->Get_CamSetting();
	// { Cam_near, Cam_far, Cam_fovY, Cam_aspectRatio };
	/* ī�޶��� ����������Ŀ��� ���Ǵ°�. 
	 * ���������� ���� �� Ŭ���� ����� ��Ÿ����. */

	_float fNear = CamSetting.x;
	_float fFar = CamSetting.y;
	_float fFovY = CamSetting.z;
	_float fAspect = g_iWinsizeX / g_iWinsizeY;

	_float fHalfHeight = fFar * tanf(0.5f * fFovY);
	_float fHalfWidth = fAspect * fHalfHeight;

	m_vFrustumFarCorner[0] = _float4(-fHalfWidth, -fHalfHeight, fFar, 0.0f);
	m_vFrustumFarCorner[1] = _float4(-fHalfWidth, +fHalfHeight, fFar, 0.0f);
	m_vFrustumFarCorner[2] = _float4(+fHalfWidth, +fHalfHeight, fFar, 0.0f);
	m_vFrustumFarCorner[3] = _float4(+fHalfWidth, -fHalfHeight, fFar, 0.0f);

	return S_OK;
}

HRESULT CRenderer::BuildFullScreenQuad()
{
	m_iQuadVerCount = 4;
	m_iQuadIndexCount = 6;

	/* Vertex */
	QuadVertex* pVertices = new QuadVertex[m_iQuadVerCount];
	NULL_CHECK_RETURN(pVertices, E_FAIL);

	/* Index */
	_ulong* pIndices = new _ulong[m_iQuadIndexCount];
	NULL_CHECK_RETURN(pIndices, E_FAIL);

	pVertices[0].pos = _float3(-0.5f, -0.5f, 0.0f);
	pVertices[1].pos = _float3(-0.5f, +0.5f, 0.0f);
	pVertices[2].pos = _float3(+0.5f, +0.5f, 0.0f);
	pVertices[3].pos = _float3(+0.5f, -0.5f, 0.0f);

	pVertices[0].ToFarPlaneIndex = _float3(0.0f, 0.0f, 0.0f);
	pVertices[1].ToFarPlaneIndex = _float3(1.0f, 0.0f, 0.0f);
	pVertices[2].ToFarPlaneIndex = _float3(2.0f, 0.0f, 0.0f);
	pVertices[3].ToFarPlaneIndex = _float3(3.0f, 0.0f, 0.0f);

	pVertices[0].tex = _float2(0.0f, 1.0f);
	pVertices[1].tex = _float2(0.0f, 0.0f);
	pVertices[2].tex = _float2(1.0f, 0.0f);
	pVertices[3].tex = _float2(1.0f, 1.0f);

	// 
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(QuadVertex) * m_iQuadVerCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_ScreenQuadVB)))
		return E_FAIL;

	D3D11_BUFFER_DESC  indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_ulong) * m_iQuadIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_ScreenQuadIB)))
		return E_FAIL;

	Safe_Delete_Array<QuadVertex*>(pVertices);
	Safe_Delete_Array<_ulong*>(pIndices);

	return S_OK;
}

void CRenderer::BuildOffsetVectors()
{
	// 14���� �����ϰ� ������ ���ͷ� ����.
	// ������ü�� 8���� �𼭸��� ����, �� ���� ���� 6�� �߽����� �����Ѵ�.
	// �׻� �ٸ� �� ���� �������� �� ���� ������ ����Ѵ�. -> ������ü �ݴ��ʵ� �յ��ϰ� ���� 
	// 14�� �̸��� ���ø� ����Ʈ�� ������ ������ ���͸� �յ��ϰ� �л��ų �� �ִ�.

	//{
	//	// 8���� ť�� �ڳ� ����
	//	m_vOffsets[0] = _float4(+1.0f, +1.0f, +1.0f, 0.0f);
	//	m_vOffsets[1] = _float4(-1.0f, -1.0f, -1.0f, 0.0f);
	//					
	//	m_vOffsets[2] = _float4(-1.0f, +1.0f, +1.0f, 0.0f);
	//	m_vOffsets[3] = _float4(+1.0f, -1.0f, -1.0f, 0.0f);
	//					
	//	m_vOffsets[4] = _float4(+1.0f, +1.0f, -1.0f, 0.0f);
	//	m_vOffsets[5] = _float4(-1.0f, -1.0f, +1.0f, 0.0f);
	//					
	//	m_vOffsets[6] = _float4(-1.0f, +1.0f, -1.0f, 0.0f);
	//	m_vOffsets[7] = _float4(+1.0f, -1.0f, +1.0f, 0.0f);
	//}

	//{
	//	// 6���� ǥ�� �߽��� ����
	//	m_vOffsets[8]	= _float4(-1.0f, 0.0f, 0.0f, 0.0f);
	//	m_vOffsets[9]	= _float4(+1.0f, 0.0f, 0.0f, 0.0f);
	//					  
	//	m_vOffsets[10]	= _float4(0.0f, -1.0f, 0.0f, 0.0f);
	//	m_vOffsets[11]	= _float4(0.0f, +1.0f, 0.0f, 0.0f);
	//					  
	//	m_vOffsets[12]	= _float4(0.0f, 0.0f, -1.0f, 0.0f);
	//	m_vOffsets[13]	= _float4(0.0f, 0.0f, +1.0f, 0.0f);
	//}					  
	//					  
	//{					  
	//	m_vOffsets[14]	= _float4(-1.0f, 1.0f, 0.0f, 0.0f);
	//	m_vOffsets[15]	= _float4(1.0f, 1.0f, 0.0f, 0.0f);
	//	m_vOffsets[16]	= _float4(0.0f, 1.0f, -1.0f, 0.0f);
	//	m_vOffsets[17]	= _float4(0.0f, 1.0f, 1.0f, 0.0f);
	//}

	//{
	//	m_vOffsets[18] = _float4(1.0f, 0.0f, 1.0f, 0.0f);
	//	m_vOffsets[19] = _float4(-1.0f, 0.0f, 1.0f, 0.0f);
	//	m_vOffsets[20] = _float4(-1.0f, 0.0f, -1.0f, 0.0f);
	//	m_vOffsets[21] = _float4(1.0f, 0.0f, -1.0f, 0.0f);
	//}

	//{
	//	m_vOffsets[22] = _float4(-1.0f, -1.0f, 0.0f, 0.0f);
	//	m_vOffsets[23] = _float4(1.0f, -1.0f, 0.0f, 0.0f);
	//	m_vOffsets[24] = _float4(0.0f, -1.0f, -1.0f, 0.0f);
	//	m_vOffsets[25] = _float4(0.0f, -1.0f, 1.0f, 0.0f);
	//}

	m_vOffsets[0]  = _float4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[1]  = _float4(-1.0f, -1.0f, -1.0f, 0.0f);
				   	 
	m_vOffsets[2]  = _float4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[3]  = _float4(+1.0f, -1.0f, -1.0f, 0.0f);
				   	 
	m_vOffsets[4]  = _float4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[5]  = _float4(-1.0f, -1.0f, +1.0f, 0.0f);
				   	 
	m_vOffsets[6]  = _float4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[7]  = _float4(+1.0f, -1.0f, +1.0f, 0.0f);
				   	 
	// 6 centers   offfsets
	m_vOffsets[8]  = _float4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_vOffsets[9]  = _float4(+1.0f, 0.0f, 0.0f, 0.0f);
					 
	m_vOffsets[10] = _float4(0.0f, -1.0f, 0.0f, 0.0f);
	m_vOffsets[11] = _float4(0.0f, +1.0f, 0.0f, 0.0f);
					 
	m_vOffsets[12] = _float4(0.0f, 0.0f, -1.0f, 0.0f);
	m_vOffsets[13] = _float4(0.0f, 0.0f, +1.0f, 0.0f);

	//// ���� ������ �ʱ�ȭ
	//mt19937 randEngine;
	//randEngine.seed(std::random_device()());
	//uniform_real_distribution<_float> randF(0.25f, 1.0f);
	for (_uint i = 0; i < 26; ++i)
	{
		// [0.25, 1.0] ������ ������ ���͸� �����.
		_float fRandom = SMath::fRandom(0.25f, 1.0f);

		_vector v = fRandom * XMVector4Normalize(XMLoadFloat4(&m_vOffsets[i]));
		
		m_vOffsets[i] = v;
	}
}

HRESULT CRenderer::RenderScreenQuad()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	_uint stride = sizeof(QuadVertex);
	_uint offset = 0;

	m_pContext->IASetVertexBuffers(0, 1, &m_ScreenQuadVB, &stride, &offset);

	m_pContext->IASetIndexBuffer(m_ScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pContext->DrawIndexed(m_iQuadIndexCount, 0, 0);

	return S_OK;
}
#pragma endregion

#pragma region Blur

HRESULT CRenderer::Render_Blur_DownSample(const wstring& strStartTargetTag)
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_DownSampling")));

	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(strStartTargetTag, m_pShader[SHADER_TYPE::SHADER_BLUR], "g_BlurTarget"));

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 0.5f;
	WorldMatrix._22 = m_WorldMatrix._22 * 0.5f;

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Begin(ECast(BLUR_SHADER::BLUR_DOWN)));

	FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Horizontal(_int eHorizontalPass)
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Horizontal")));

	/* �ٿ���ø��� ������ �����ϹǷ� */
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_DownSampling"), m_pShader[SHADER_TYPE::SHADER_BLUR], "g_BlurTarget"));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Begin(eHorizontalPass));

	FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Vertical(_int eVerticalPass)
{
	FAILED_CHECK(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Vertical")));

	/* ������ø��� ������ ���� */
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_Horizontal"), m_pShader[SHADER_TYPE::SHADER_BLUR], "g_BlurTarget"));


	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Begin(eVerticalPass));

	FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}

HRESULT CRenderer::Render_Blur_UpSample(const wstring& strFinalMrtTag, _bool bClear, _int eBlendType)
{
	/* ����������� �ٽ� �ǵ��� */
	FAILED_CHECK(m_pGameInstance->Begin_MRT(strFinalMrtTag, nullptr, bClear));

	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_Vertical"), m_pShader[SHADER_TYPE::SHADER_BLUR], "g_BlurTarget"));

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 2.f;
	WorldMatrix._22 = m_WorldMatrix._22 * 2.f;

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &WorldMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix));
	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix));

	FAILED_CHECK(m_pShader[SHADER_TYPE::SHADER_BLUR]->Begin(eBlendType));

	FAILED_CHECK(m_pVIBuffer->Render());

	FAILED_CHECK(m_pGameInstance->End_MRT());

	return S_OK;
}
void CRenderer::Calc_Blur_GaussianWeights(_int sigma, _int iSize , _Out_ void* Weights)
{
	/* Calc_Blur_GaussianWeights(3, g_fWeight_middle[7]) �̷������� ���ÿ��� */
	/* sigma �� �������� ����ġ�� ������ ����. Ŭ���� õõ�� �����ؼ� �ε巯�� ���� ��Ÿ����.  */
	/* */

	/*example : sigma = 1, iSize = 3*/
	_float coeff = 1.0f / (2.0f * sigma * sigma);

	// ����ġ�� ����ϰ� ����
	_float* Weight = static_cast<float*>(Weights);
	_int halfSize = iSize / 2; /* �߾��� �ε��� */

	_float sum = 0.0f;

	for (_int i = -halfSize; i <= halfSize; ++i)
	{
		_int x = i + halfSize;
		Weight[x] = exp(-static_cast<float>(i * i) * coeff);
	}

	// ����ġ ����ȭ
	for (_int i = 0; i < iSize; ++i)
	{
		sum += Weight[i];
	}

	for (_int i = 0; i < iSize; ++i)
	{
		Weight[i] /= sum;
	}
}

#pragma endregion

#pragma region Add RenderGroup / Debug_Render

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroupID, CGameObject* pGameObject)
{
	if (nullptr == pGameObject ||
		eGroupID >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eGroupID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugRender(CComponent* pDebugCom)
{
#ifdef _DEBUG
	m_DebugComponent.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);
#endif

	return S_OK;
}

HRESULT CRenderer::Pre_Setting()
{
#pragma region HOTKEY

	if (m_pGameInstance->Key_Down(DIK_1)) /* �ӽ÷� SSAO �� HBAO �Ѵ� ���¿뵵 */
		m_bPBR_Active = !m_bPBR_Active;
	if (m_pGameInstance->Key_Down(DIK_2))
		m_bSSAO_Active = !m_bSSAO_Active;
	if (m_pGameInstance->Key_Down(DIK_3))
		m_bBloom_Active = !m_bBloom_Active;
	if (m_pGameInstance->Key_Down(DIK_4))
		m_bOutline_Active = !m_bOutline_Active;
	
	Render_DebugOnOff();
#pragma endregion



	return S_OK;
}

#pragma endregion

#pragma region Create / Free

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderer::Free()
{
	for (auto& ObjectList : m_RenderObjects)
	{
		for (auto& pGameObject : ObjectList)
			Safe_Release(pGameObject);
		ObjectList.clear();
	}

#ifdef _DEBUG

	for (auto pDebugCom : m_DebugComponent)
		Safe_Release(pDebugCom);
	m_DebugComponent.clear();

#endif

	for (_int i = 0; i < ECast(SHADER_TYPE::SHADER_END); ++i)
		Safe_Release(m_pShader[i]);

	/* ssao ���� */
	Safe_Release(m_pRandomVectorTexture);
	Safe_Release(m_pRandomSRV);
	Safe_Release(m_ScreenQuadVB);
	Safe_Release(m_ScreenQuadIB);

	Safe_Release(m_pLightDepthDSV);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

#pragma endregion
