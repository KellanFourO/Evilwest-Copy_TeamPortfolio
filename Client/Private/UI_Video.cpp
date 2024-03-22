#include "stdafx.h"
#include "UI_Video.h"
#include "GameInstance.h"
#include "Json_Utility.h"

#include "Data_Manager.h"
//#include <mfapi.h>
//#include <mfidl.h>
//#include <mfreadwrite.h>
//#include <mferror.h>

// #include <avformat.h>
//#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libswresample/swresample.h>
//#include <libswscale/swscale.h>
//#include <libavutil/avutil.h>

CUI_Video::CUI_Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Video::CUI_Video(const CUI_Video& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Video::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Video::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	LoadVideo(L"../Bin/Resources/Textures/UI/Video/MainMenu/ew_mainmenu_intro.avi");

	return S_OK;
}

void CUI_Video::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Video::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CUI_Video::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Video::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(ReadVideoFrame()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸���.
	m_pShaderCom->Begin(m_iShaderNum); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();
	
	return S_OK;
}

void CUI_Video::UI_Ready(_float fTimeDelta)
{
}

void CUI_Video::UI_Enter(_float fTimeDelta)
{
}

void CUI_Video::UI_Loop(_float fTimeDelta)
{
}

void CUI_Video::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_Video::Ready_Components()
{
	//! For.Com_Texture1 // ��� ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_health_bar_damagel"),
		TEXT("Com_Texture_WhiteBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_WHITE]))))
		return E_FAIL;

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* ȿ���� �ʿ��� �༮�� Map�ؽ��ĵ� �߰����ֱ� */
	return S_OK;
}

HRESULT CUI_Video::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	
	//// ���� �ؽ�ó ���ε�
	//if (FAILED(m_pTextureVideo->Bind_ShaderResource(m_pShaderCom, "g_VideoTexture")))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom[HPBAR_WHITE]->Bind_ShaderResource(m_pShaderCom, "g_HpBarWhite_Texture")))	// Hp White
		return E_FAIL;

	return S_OK;
}

// ���� �ε� �Լ� : �����ϴ� Ȯ���� (mp4, avi, wmv, mov, mpeg, flv)
HRESULT CUI_Video::LoadVideo(const wchar_t* filePath)
{
	//HRESULT hr = S_OK;
	//IMFSourceReader* m_pSourceReader = nullptr;
	//// Media Foundation �ʱ�ȭ �Լ�
	//hr = MFStartup(MF_VERSION);
	//if (FAILED(hr)) return hr;

	//// ���� ���� ���� : m_pSourceReader�� ���� ������ �дµ� ���Ǵ� �ҽ����� ��ü�� ������. (���� ���� ���� ����)
	//hr = MFCreateSourceReaderFromURL(filePath, nullptr, &m_pSourceReader);
	//if (FAILED(hr)) 
	//{
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//// ���� ���� �������� : ù ��° ���� ��Ʈ���� ���� �̵�� ���� ������ m_pMediaType�� �����´�.
	//hr = m_pSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &m_pMediaType);
	//if (FAILED(hr)) 
	//{
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//// ���� �������� �ʺ�� ���� ��������
	//UINT32 width = 0, height = 0;
	//hr = MFGetAttributeSize(m_pMediaType, MF_MT_FRAME_SIZE, &width, &height);
	//if (FAILED(hr)) 
	//{
	//	m_pMediaType->Release();
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//// ���� ������ �Ӽ� ����
	//IMFMediaType* pOutputType = nullptr;
	//hr = MFCreateMediaType(&pOutputType);
	//if (FAILED(hr)) 
	//{
	//	m_pMediaType->Release();
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//hr = pOutputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	//if (FAILED(hr)) 
	//{
	//	pOutputType->Release();
	//	m_pMediaType->Release();
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//hr = pOutputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
	//if (FAILED(hr)) 
	//{
	//	pOutputType->Release();
	//	m_pMediaType->Release();
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//hr = m_pSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pOutputType);
	//if (FAILED(hr)) 
	//{
	//	pOutputType->Release();
	//	m_pMediaType->Release();
	//	m_pSourceReader->Release();
	//	MFShutdown(); // Media Foundation ����
	//	return hr;
	//}

	//pOutputType->Release();

	//// ������ �б�
	//DWORD dwStreamIndex, dwFlags;
	//LONGLONG llTimestamp;
	//IMFSample* pSample = nullptr;
	//IMFMediaBuffer* pBuffer = nullptr;

	//// ù ��° �������� ������ �б�
	//// ù ��° �����󿡼� �������� �о� 4���� �Ű������� �������� ����ش�. (pSample�� �ش� ������ �����͸� ����ְ�, �� �����͸� ���� �� �ؽ�ó�� ��ȯ�� �� ���� ���Ѽ� �������� �����ش�.)
	//hr = m_pSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &dwStreamIndex, &dwFlags, &llTimestamp, &pSample);
	//if (FAILED(hr)) return hr;

	//// ������ �� �˻�
	//if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) 
	//{
	//	// �� �̻� �������� ����
	//	pSample->Release();
	//	return S_FALSE;
	//}

	//if (pSample == nullptr) 
	//{
	//	// �������� ����
	//	return S_FALSE;
	//}

	//hr = pSample->ConvertToContiguousBuffer(&pBuffer);
	//if (FAILED(hr)) 
	//{
	//	pSample->Release();
	//	return hr;
	//}

	//// �������� �̹��� ������ ��������
	//BYTE* pFrameData = nullptr;
	//DWORD cbMaxLength, cbCurrentLength;
	//hr = pBuffer->Lock(&pFrameData, &cbMaxLength, &cbCurrentLength);
	//if (FAILED(hr)) 
	//{
	//	pBuffer->Release();
	//	pSample->Release();
	//	return hr;
	//}

	//// �������� �ʺ�� ���� ��������
	//UINT32 width, height;
	//hr = MFGetAttributeSize(m_pMediaType, MF_MT_FRAME_SIZE, &width, &height);
	//if (FAILED(hr)) 
	//{
	//	pBuffer->Unlock();
	//	pBuffer->Release();
	//	pSample->Release();
	//	return hr;
	//}


	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;

	//// ShaderResourceView�� ������ ������ ����
	//hr = CUI_Video::CreateShaderResourceViewFromVideoFrame(
	//														m_pDevice,
	//														m_pContext,
	//														pFrameData,
	//														width,
	//														height,
	//														&pShaderResourceView);

	//// ���̴��� ��������
	//m_pShaderCom->Bind_SRV("g_Video_Texture", pShaderResourceView);

	////// �ؽ�ó�� ������ ������ ����
	////hr = CopyVideoFrameToTexture(m_pContext, pFrameData, width, height, m_pTextureVideo);
	////if (FAILED(hr)) 
	////{
	////	pBuffer->Unlock();
	////	pBuffer->Release();
	////	pSample->Release();
	////	return hr;
	////}

	//pBuffer->Unlock();
	//pBuffer->Release();
	//pSample->Release();

	return S_OK;
}

HRESULT CUI_Video::CopyVideoFrameToTexture(ID3D11DeviceContext* pContext, BYTE* pFrameData, UINT32 width, UINT32 height, ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	// �ؽ�ó ���� �� ������ ������ ����
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return hr;

	BYTE* pTextureData = reinterpret_cast<BYTE*>(mappedResource.pData);
	UINT32 rowPitch = width * 4; // RGBA �����̹Ƿ� �ʺ� * 4

	for (UINT32 y = 0; y < height; ++y) 
	{
		memcpy(pTextureData, pFrameData, rowPitch);
		pTextureData += mappedResource.RowPitch; // ���� ������ �̵�
		pFrameData += rowPitch; // ���� ������ �̵�
	}

	pContext->Unmap(pTexture, 0);

	return S_OK;
}

HRESULT CUI_Video::CreateShaderResourceViewFromVideoFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BYTE* pFrameData, UINT32 width, UINT32 height, ID3D11ShaderResourceView** ppShaderResourceView)
{
	//HRESULT hr = S_OK;

	//// �ؽ�ó ����
	//ID3D11Texture2D* pTexture = nullptr;
	//D3D11_TEXTURE2D_DESC desc;
	//desc.Width = width;
	//desc.Height = height;
	//desc.MipLevels = 1;
	//desc.ArraySize = 1;
	//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA ����
	//desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality = 0;
	//desc.Usage = D3D11_USAGE_DEFAULT;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = 0;
	//desc.MiscFlags = 0;

	//hr = pDevice->CreateTexture2D(&desc, nullptr, &pTexture);
	//if (FAILED(hr))
	//	return hr;

	//// �ؽ�ó ���� �� ������ ������ ����
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//hr = pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(hr))
	//{
	//	pTexture->Release();
	//	return hr;
	//}

	//BYTE* pTextureData = reinterpret_cast<BYTE*>(mappedResource.pData);
	//UINT32 rowPitch = width * 4; // RGBA �����̹Ƿ� �ʺ� * 4

	//for (UINT32 y = 0; y < height; ++y)
	//{
	//	memcpy(pTextureData, pFrameData, rowPitch);
	//	pTextureData += mappedResource.RowPitch; // ���� ������ �̵�
	//	pFrameData += rowPitch; // ���� ������ �̵�
	//}

	//pContext->Unmap(pTexture, 0);

	//// SRV ����
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//ZeroMemory(&srvDesc, sizeof(srvDesc));
	//srvDesc.Format = desc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = desc.MipLevels;

	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	//hr = pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pShaderResourceView);
	//if (FAILED(hr))
	//{
	//	pTexture->Release();
	//	return hr;
	//}

	//pTexture->Release(); // SRV ���� �� �ؽ�ó�� �� �̻� �ʿ����� �����Ƿ� ����

	//*ppShaderResourceView = pShaderResourceView;

	return S_OK;
}

// �������� ������ ������ ��ȯ�Ͽ� �����ش�.
HRESULT CUI_Video::ReadVideoFrame()
{
	//if (m_pSourceReader == nullptr)
	//	return E_FAIL;

	//HRESULT hr = S_OK;

	//	// ������ �б�
	//DWORD dwStreamIndex, dwFlags;
	//LONGLONG llTimestamp;
	//IMFSample* pSample = nullptr;
	//IMFMediaBuffer* pBuffer = nullptr;

	//// ù ��° �������� ������ �б�
	//// ù ��° �����󿡼� �������� �о� 4���� �Ű������� �������� ����ش�. (pSample�� �ش� ������ �����͸� ����ְ�, �� �����͸� ���� �� �ؽ�ó�� ��ȯ�� �� ���� ���Ѽ� �������� �����ش�.)
	//if(m_pSourceReader != nullptr)
	//	hr = m_pSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &dwStreamIndex, &dwFlags, &llTimestamp, &pSample);

	//if (FAILED(hr)) return hr;

	//// ������ �� �˻�
	//if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) 
	//{
	//	// �� �̻� �������� ����
	//	pSample->Release();
	//	return S_FALSE;
	//}

	//if (pSample == nullptr) 
	//{
	//	// �������� ����
	//	return S_FALSE;
	//}

	//hr = pSample->ConvertToContiguousBuffer(&pBuffer);
	//if (FAILED(hr)) 
	//{
	//	pSample->Release();
	//	return hr;
	//}

	//// �������� �̹��� ������ ��������
	//BYTE* pFrameData = nullptr;
	//DWORD cbMaxLength, cbCurrentLength;
	//hr = pBuffer->Lock(&pFrameData, &cbMaxLength, &cbCurrentLength);
	//if (FAILED(hr)) 
	//{
	//	pBuffer->Release();
	//	pSample->Release();
	//	return hr;
	//}

	//// �������� �ʺ�� ���� ��������
	//UINT32 width, height;
	//hr = MFGetAttributeSize(m_pMediaType, MF_MT_FRAME_SIZE, &width, &height);
	//if (FAILED(hr)) 
	//{
	//	pBuffer->Unlock();
	//	pBuffer->Release();
	//	pSample->Release();
	//	return hr;
	//}


	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;

	//// ShaderResourceView�� ������ ������ ����
	//hr = CUI_Video::CreateShaderResourceViewFromVideoFrame(
	//														m_pDevice,
	//														m_pContext,
	//														pFrameData,
	//														width,
	//														height,
	//														&pShaderResourceView);

	//// ���̴��� ��������
	//m_pShaderCom->Bind_SRV("g_Video_Texture", pShaderResourceView);

	////// �ؽ�ó�� ������ ������ ����
	////hr = CopyVideoFrameToTexture(m_pContext, pFrameData, width, height, m_pTextureVideo);
	////if (FAILED(hr)) 
	////{
	////	pBuffer->Unlock();
	////	pBuffer->Release();
	////	pSample->Release();
	////	return hr;
	////}

	//pBuffer->Unlock();
	//pBuffer->Release();
	//pSample->Release();

	return S_OK;
}

HRESULT CUI_Video::CreateVideoTexture(ID3D11Device* pDevice, UINT32 width, UINT32 height, ID3D11Texture2D** ppTexture, ID3D11ShaderResourceView** ppTextureView)
{
	HRESULT hr = S_OK;

	// �ؽ�ó ����
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.SampleDesc.Count = 1;

	hr = pDevice->CreateTexture2D(&desc, nullptr, ppTexture);
	if (FAILED(hr)) return hr;

	// �ؽ�ó ���ҽ� �� ����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = pDevice->CreateShaderResourceView(*ppTexture, &srvDesc, ppTextureView);
	if (FAILED(hr)) {
		(*ppTexture)->Release();
		return hr;
	}

	return S_OK;
}

HRESULT CUI_Video::ConvertVideoFrameToTexture(ID3D11DeviceContext* pContext, BYTE* pFrameData, UINT32 width, UINT32 height, ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	// �ؽ�ó�� ������ ������ ����
	hr = CopyVideoFrameToTexture(pContext, pFrameData, width, height, pTexture);
	if (FAILED(hr)) return hr;

	return S_OK;
}

json CUI_Video::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_Video::Load_Desc()
{

}

CUI_Video* CUI_Video::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Video* pInstance = new CUI_Video(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Video");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Video::Clone(void* pArg)
{
	CUI_Video* pInstance = new CUI_Video(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Video");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Video::Pool()
{
	return new CUI_Video(*this);
}

void CUI_Video::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
