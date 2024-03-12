#include "stdafx.h"
#include "UI_Video.h"
#include "GameInstance.h"
#include "Json_Utility.h"

#include "Data_Manager.h"
// #include <avformat.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>

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

	//! �� ���̴��� 0��° �н��� �׸���.
	m_pShaderCom->Begin(m_iShaderNum); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Video::Ready_Components()
{
	//! For.Com_Texture1 // ��� ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_health_bar_damagel"),
		TEXT("Com_Texture_WhiteBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_WHITE]))))
		return E_FAIL;

	//D3D11_TEXTURE2D_DESC textureDesc;
	//textureDesc.Width = g_iWinsizeX;
	//textureDesc.Height = g_iWinsizeY;
	//textureDesc.MipLevels = 1;
	//textureDesc.ArraySize = 1;
	//textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//textureDesc.SampleDesc.Count = 1;
	//textureDesc.SampleDesc.Quality = 0;
	//textureDesc.Usage = D3D11_USAGE_DEFAULT;
	//textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	//textureDesc.CPUAccessFlags = 0;
	//textureDesc.MiscFlags = 0;

	//m_pDevice->CreateTexture2D(&textureDesc, NULL, &m_pTextureVideo);

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

	//AVFormatContext* format_context;
	//const char* input_file;

	//// FFmpeg ���̺귯�� �ʱ�ȭ
	//avformat_open_input(&format_context, input_file, NULL, NULL);
	//avformat_find_stream_info(format_context, NULL);

	//// ���� ��Ʈ�� ���� ���
	//AVStream* video_stream = format_context->streams[0];
	//AVCodecContext* video_codec_context = video_stream->codecpar;

	//// SWScaler ���ؽ�Ʈ ����
	//SwsContext* sws_context = sws_getContext(
	//	video_codec_context->width, video_codec_context->height, video_codec_context->pix_fmt,
	//	video_codec_context->width, video_codec_context->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

	//// ������ �� ��ȯ�� ������
	//AVFrame* frame = av_frame_alloc();
	//AVFrame* converted_frame = av_frame_alloc();

	//// RGB ���� ����
	//uint8_t* rgb_buffer = (uint8_t*)malloc(avpicture_get_size(AV_PIX_FMT_RGB24, video_codec_context->width, video_codec_context->height));

	//// ���� ���ڵ� �� ��ȯ ����
	//while (av_read_frame(format_context, &packet) >= 0) {
	//	if (packet.stream_index == video_stream->index) {
	//		// ������ ���ڵ�
	//		avcodec_decode_video2(video_codec_context, frame, &got_frame, &packet);

	//		if (got_frame) {
	//			// ������ ��ȯ
	//			sws_scale(sws_context, (const uint8_t**)frame->data, frame->linesize, 0,
	//				video_codec_context->height, converted_frame->data, converted_frame->linesize);

	//			// RGB ���ۿ� ����
	//			avpicture_layout(converted_frame, AV_PIX_FMT_RGB24, video_codec_context->width, video_codec_context->height, rgb_buffer, NULL);
	//		}
	//	}

	//	// ��Ŷ ���� ����
	//	av_packet_unref(&packet);
	//}


	if (FAILED(m_pTextureCom[HPBAR_WHITE]->Bind_ShaderResource(m_pShaderCom, "g_HpBarWhite_Texture")))	// Hp White
		return E_FAIL;

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
