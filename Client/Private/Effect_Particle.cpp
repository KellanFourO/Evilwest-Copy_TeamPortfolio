#include "stdafx.h"
#include "..\Public\Effect_Particle.h"

#include "GameInstance.h"

#include "Effect.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CEffect_Void(pDevice, pContext, strPrototypeTag)
{

}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& rhs)
	: CEffect_Void(rhs)
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* pArg)
{
	m_tParticleDesc = *(PARTICLE_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Particle::Priority_Tick(_float fTimeDelta)
{


}

void CEffect_Particle::Tick(_float fTimeDelta)
{

	CVIBuffer_Particle_Point::PARTICLE_POINT_DESC* pDesc = m_pVIBufferCom->Get_Desc();

	if (m_tParticleDesc.bActive_Tool)
	{
		m_fSequenceTime = m_fLifeTime + m_fRemainTime;

		pDesc->bActive_Tool = TRUE;
		pDesc->vMinMaxLifeTime.x = m_fWaitingTime;
		pDesc->vMinMaxLifeTime.y = m_fLifeTime;

		if (m_tParticleDesc.bPlay)
		{
			m_fSequenceAcc += fTimeDelta;

			// 시작지연 누적시간이 지나면 렌더 시작(파티클 시작)
			if (m_fWaitingAcc <= m_fWaitingTime)
			{
				m_fWaitingAcc += fTimeDelta;

				if (m_fWaitingAcc >= m_fWaitingTime)
				{
					m_tParticleDesc.bRender = TRUE;
					//pDesc->bPlay	= TRUE;
				}
				else
					return;
			}

			// 시간 누적 시작
			m_fTimeAcc += fTimeDelta;
			m_fLifeTimeRatio = min(1.0f, m_fTimeAcc / m_fLifeTime);


			/* ======================= 라이프 타임 동작 시작 ======================= */



			/* ======================= 라이프 타임 동작 끝  ======================= */

			if (m_fTimeAcc >= m_fLifeTime)
			{
				// 삭제 대기시간 누적 시작
				m_fRemainAcc += fTimeDelta;

				// 디졸브 시작
				m_tParticleDesc.bDissolve = TRUE;
				if (m_tParticleDesc.bDissolve)
				{
					m_tParticleDesc.fDissolveAmount = Easing::LerpToType(0.f, 1.f, m_fRemainAcc, m_fRemainTime, m_tParticleDesc.eType_Easing);
				}

				if (m_fRemainAcc >= m_fRemainTime)
				{
					m_tParticleDesc.fDissolveAmount = 1.f;
					m_tParticleDesc.bRender = FALSE;
					return;
				}
			}

			if (m_tParticleDesc.bRender)
			{
				m_pVIBufferCom->Update(fTimeDelta);
			}
		}
	}
	else
	{
		m_pVIBufferCom->Get_Desc()->bActive_Tool = FALSE;
	}
}

void CEffect_Particle::Late_Tick(_float fTimeDelta)
{
	if (m_tParticleDesc.bActive_Tool)
	{
		if (m_tParticleDesc.bRender)
		{
			Compute_CamDistance();

			// CRenderer::RENDER_BLEND
			if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDERGROUP(m_tParticleDesc.iRenderGroup), this)))
				return;
		}
	}
}

HRESULT CEffect_Particle::Render()
{
	if (m_tParticleDesc.bActive_Tool)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		/* 이 쉐이더에 0번째 패스로 그릴거야. */
		m_pShaderCom->Begin(m_tParticleDesc.iShaderPassIndex);

		/* 내가 그리려고하는 정점, 인덱스버퍼를 장치에 바인딩해. */
		m_pVIBufferCom->Bind_VIBuffers();

		/* 바인딩된 정점, 인덱스를 그려. */
		m_pVIBufferCom->Render();
	}


	return S_OK;
}

void CEffect_Particle::ReSet_Effect()
{
	__super::ReSet_Effect();

	m_tParticleDesc.fDissolveAmount = 0.f;
	m_tParticleDesc.bDissolve = FALSE;
	m_tParticleDesc.bRender = FALSE;

	m_pVIBufferCom->ReSet();

}

void CEffect_Particle::End_Effect()
{
	__super::End_Effect();

	if (m_tParticleDesc.bLoop)
	{
		ReSet_Effect();
	}

}

_bool CEffect_Particle::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		Out_Json["strTextureTag"][i] = m_pGameInstance->Convert_WString_To_String(m_tParticleDesc.strTextureTag[i]);

	Out_Json["strShaderTag"] = m_pGameInstance->Convert_WString_To_String(m_tParticleDesc.strShaderTag);

	return true;
}

void CEffect_Particle::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		m_pGameInstance->Convert_WString_To_String(m_tParticleDesc.strTextureTag[i]) = In_Json["strTextureTag"][i];

	m_pGameInstance->Convert_WString_To_String(m_tParticleDesc.strShaderTag) = In_Json["strShaderTag"];

}


HRESULT CEffect_Particle::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, m_tParticleDesc.strShaderTag,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	{
		CVIBuffer_Particle_Point::PARTICLE_POINT_DESC		tVIBufferDesc = {};
		tVIBufferDesc.eType_Action = { CVIBuffer_Particle_Point::TYPE_ACTION::SPHERE };
		tVIBufferDesc.eType_Fade = { CVIBuffer_Particle_Point::TYPE_FADE::FADE_OUT };

		tVIBufferDesc.bActive_Tool	= { TRUE };
		tVIBufferDesc.bBillBoard	= { TRUE };
		//tVIBufferDesc.bPlay			= { TRUE };
		tVIBufferDesc.bReverse		= { FALSE };
		tVIBufferDesc.bLoop			= { TRUE };

		tVIBufferDesc.vMinMaxLifeTime = _float2(0.5f, 3.0f);
		tVIBufferDesc.iCurNumInstance = { m_tParticleDesc.iCurInstanceCnt };

		tVIBufferDesc.vMinMaxRange = { 0.1f, 3.f };
		tVIBufferDesc.vCenterPosition = _float3(0.f, 0.f, 0.f);
		tVIBufferDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);

		tVIBufferDesc.vMinMaxLengthPosition = { 3.f, 3.f };

		tVIBufferDesc.vMinMaxSpeed = _float2(0.1f, 5.0f);
		tVIBufferDesc.fSpeedAcc = { 2.f };
		tVIBufferDesc.fAccPosition = { 0.1f };

		tVIBufferDesc.fGravityAcc = { -9.8 };
		tVIBufferDesc.vCurrentGravity = { 0.f, 0.f, 0.f };

		tVIBufferDesc.vMinMaxRotationOffsetX = { 0.0f, 360.f };
		tVIBufferDesc.vMinMaxRotationOffsetY = { 0.0f, 360.f };
		tVIBufferDesc.vMinMaxRotationOffsetZ = { 0.0f, 360.f };

		tVIBufferDesc.vCurrentRotation = { 0.f, 0.f, 0.f };
		tVIBufferDesc.vMinMaxRotationForce = { 0.f, 0.f, 0.f };

		tVIBufferDesc.vMinMaxScale = _float2(0.2f, 0.5f);
		tVIBufferDesc.vAddScale = { 0.f, 0.f };
		tVIBufferDesc.vMinMaxScaleForce = { 1.f, 1.f };

		tVIBufferDesc.vCurrentColor = _float4(1.f, 1.f, 1.f, 1.f);
		tVIBufferDesc.vColorSpeed = { 0.f, 0.f, 0.f, 0.f };
		tVIBufferDesc.vColorForce = { 0.f, 0.f, 0.f, 0.f };

		tVIBufferDesc.fMinMaxAlpha = { 1.f, 1.f };
		tVIBufferDesc.fAlphaForce = { 0.f };

		tVIBufferDesc.vSpriteUV = { 0.f, 0.f };
		tVIBufferDesc.vSpriteUVForce = { 0.f, 0.f };
		tVIBufferDesc.iSpriteFrameIndex = { 1 };

		if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_VIBuffer_Particle_Point"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tVIBufferDesc)))
			return E_FAIL;
	}


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(iNextLevel, m_tParticleDesc.strTextureTag[TEXTURE_DIFFUSE],
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]))))
		return E_FAIL;

	if (TEXT("") != m_tParticleDesc.strTextureTag[TEXTURE_MASK])
	{
		/* For.Com_Mask */
		if (FAILED(__super::Add_Component(iNextLevel, m_tParticleDesc.strTextureTag[TEXTURE_MASK],
			TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]))))
			return E_FAIL;
	}

	if (TEXT("") != m_tParticleDesc.strTextureTag[TEXTURE_NOISE])
	{
		/* For.Com_Noise */
		if (FAILED(__super::Add_Component(iNextLevel, m_tParticleDesc.strTextureTag[TEXTURE_NOISE],
			TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE]))))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CEffect_Particle::Bind_ShaderResources()
{
	//if (FAILED(__super::Bind_ShaderResources()))
	//	return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	//if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDegree", &m_tParticleDesc.fUV_RotDegree, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tParticleDesc.iTextureIndex[TEXTURE_DIFFUSE])))
		return E_FAIL;

	if (nullptr != m_pTextureCom[TEXTURE_MASK])
	{
		if (FAILED(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_tParticleDesc.iTextureIndex[TEXTURE_MASK])))
			return E_FAIL;
	}
	if (nullptr != m_pTextureCom[TEXTURE_NOISE])
	{
		if (FAILED(m_pTextureCom[TEXTURE_NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_tParticleDesc.iTextureIndex[TEXTURE_NOISE])))
			return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_DiscardValue", &m_tParticleDesc.vColor_Clip.w, sizeof(_float)));


	//if (SPRITE == m_tParticleDesc.eType)
	//{
	//	_vector vCamDirection = m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW).r[2];
	//	vCamDirection = XMVector4Normalize(vCamDirection);
	//	_float4 vCamDirectionFloat4 = {};
	//	XMStoreFloat4(&vCamDirectionFloat4, vCamDirection);

	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamDirection", &vCamDirectionFloat4, sizeof(_float4))))
	//		return E_FAIL;


	//	m_tParticleDesc.vUV_Offset = { (_float)(m_tSpriteDesc.vUV_CurTileIndex.x * m_tSpriteDesc.vTileSize.x) / m_tSpriteDesc.vTextureSize.x
	//					, (_float)(m_tSpriteDesc.vUV_CurTileIndex.y * m_tSpriteDesc.vTileSize.y) / m_tSpriteDesc.vTextureSize.y };

	//	m_tParticleDesc.vUV_Scale = { (_float)m_tSpriteDesc.vTileSize.x / m_tSpriteDesc.vTextureSize.x
	//							, (_float)m_tSpriteDesc.vTileSize.y / m_tSpriteDesc.vTextureSize.y };


	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_UVOffset", &m_tParticleDesc.vUV_Offset, sizeof(_float2))))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_UVScale", &m_tParticleDesc.vUV_Scale, sizeof(_float2))))
	//		return E_FAIL;
	//}




	//if (FAILED(m_pShaderCom->Bind_RawValue("FX_Variables", &m_tVariables, sizeof(tagFX_Variables))))
	//	return E_FAIL;


	return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext, strPrototypeTag);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Particle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* pArg)
{
	CEffect_Particle* pInstance = new CEffect_Particle(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Particle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Particle::Pool()
{
	return new CEffect_Particle(*this);
}

void CEffect_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
}

