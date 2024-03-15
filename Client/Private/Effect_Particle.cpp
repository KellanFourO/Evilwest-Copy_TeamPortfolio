#include "stdafx.h"
#include "..\Public\Effect_Particle.h"

#include "GameInstance.h"

#include "Effect.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CEffect_Void(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = TRUE;
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
	//m_tVoidDesc = *static_cast<EFFECTVOID_DESC*>(pArg);
	//*static_cast<EFFECTVOID_DESC*>(&m_tParticleDesc) = *static_cast<EFFECTVOID_DESC*>(pArg);

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
#ifdef _DEBUG
	//if (LEVEL_TOOL == static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel()))
	{
		if (m_tVoidDesc.bActive_Tool)
		{
#endif // _DEBUG
			m_tVoidDesc.fSequenceTime = m_tVoidDesc.fLifeTime + m_tVoidDesc.fRemainTime;

			if (m_tVoidDesc.bPlay)
			{
				m_tVoidDesc.fSequenceAcc += fTimeDelta;

				// �������� �����ð��� ������ ���� ����(��ƼŬ ����)
				if (m_tVoidDesc.fWaitingAcc <= m_tVoidDesc.fWaitingTime)
				{
					m_tVoidDesc.fWaitingAcc += fTimeDelta;

					if (m_tVoidDesc.fWaitingAcc >= m_tVoidDesc.fWaitingTime)
					{
						m_tVoidDesc.bRender = TRUE;
						//pDesc->bPlay	= TRUE;
					}
					else
						return;
				}

				// �ð� ���� ����
				m_tVoidDesc.fTimeAcc += fTimeDelta;
				m_tVoidDesc.fSpriteTimeAcc += fTimeDelta;
				m_tVoidDesc.fLifeTimeRatio = min(1.0f, m_tVoidDesc.fTimeAcc / m_tVoidDesc.fLifeTime);


				/* ======================= ������ Ÿ�� ���� ���� ======================= */
				if (m_tVoidDesc.bUseSpriteAnim)
				{
					if (!m_tSpriteDesc.bSpriteFinish)
					{
						if (m_tVoidDesc.fSpriteTimeAcc > m_tSpriteDesc.fSequenceTerm)
						{
							(m_tSpriteDesc.vUV_CurTileIndex.x)++;	// ���� �ε��� ����

							if (m_tSpriteDesc.vUV_CurTileIndex.x == m_tSpriteDesc.vUV_MaxTileCount.x)
							{
								(m_tSpriteDesc.vUV_CurTileIndex.y)++;									// ���� �ε��� ����
								m_tSpriteDesc.vUV_CurTileIndex.x = m_tSpriteDesc.vUV_MinTileCount.x;	// ���� �ε��� �ʱ�ȭ

								if (m_tSpriteDesc.vUV_CurTileIndex.y == m_tSpriteDesc.vUV_MaxTileCount.y)	// ���� �ε����� ������ �Դٸ�
								{
									m_tSpriteDesc.bSpriteFinish = TRUE;										// ��������Ʈ �ִ� ��	
								}
							}
							m_tVoidDesc.fSpriteTimeAcc = 0.f;	// �ð� �ʱ�ȭ
						}
					}

					if (m_tSpriteDesc.bSpriteFinish)
					{
						// ��������Ʈ ����� ������,
						if (m_tSpriteDesc.bLoop)	// ��������Ʈ�� ������ true�̸�
						{	
							// ��������Ʈ �ʱ�ȭ
							m_tSpriteDesc.Reset_Sprite();
						}
						else
						{
							// �ƴϸ� ���� ����
							m_tVoidDesc.bRender = FALSE;
							//m_tSpriteDesc.Reset_Sprite(); // �ʱ�ȭ
						}				
					}
				}

				/* ======================= ������ Ÿ�� ���� ��  ======================= */

				if (m_tVoidDesc.fTimeAcc >= m_tVoidDesc.fLifeTime)
				{
					// ���� ���ð� ���� ����
					m_tVoidDesc.fRemainAcc += fTimeDelta;

					// ������ ����
					m_tVoidDesc.bDissolve = TRUE;
					if (m_tVoidDesc.bDissolve)
					{
						m_tVoidDesc.fDissolveAmount = Easing::LerpToType(0.f, 1.f, m_tVoidDesc.fRemainAcc, m_tVoidDesc.fRemainTime, m_tVoidDesc.eType_Easing);
					}

					if (m_tVoidDesc.fRemainAcc >= m_tVoidDesc.fRemainTime)
					{
						m_tVoidDesc.fDissolveAmount = 1.f;
						m_tVoidDesc.bRender = TRUE;
						return;
					}
				}

				if (m_tVoidDesc.bRender)
				{
					m_pVIBufferCom->Update(fTimeDelta);
				}
			}
#ifdef _DEBUG
		}
	}
#endif // _DEBUG

}

void CEffect_Particle::Late_Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	//if (LEVEL_TOOL == static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel()))
	{
		if (m_tVoidDesc.bActive_Tool)
		{
#endif // _DEBUG
			if (m_tVoidDesc.bRender)
			{
				__super::Update_PivotMat();

				if (m_bSortZ)
				{
					//m_pVIBufferCom->Sort_Z(m_pVIBufferCom->Get_NumInstance());
				}
				Compute_CamDistance();

				// CRenderer::RENDER_BLEND
				FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tVoidDesc.iRenderGroup, this), );
			}
#ifdef _DEBUG
		}
	}
#endif // _DEBUG
}

HRESULT CEffect_Particle::Render()
{
#ifdef _DEBUG
	//if (LEVEL_TOOL == static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel()))
	{
		if (m_tVoidDesc.bActive_Tool)
		{
#endif // _DEBUG
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			/* �� ���̴��� 0��° �н��� �׸��ž�. */
			m_pShaderCom->Begin(m_tVoidDesc.iShaderPassIndex);

			/* ���� �׸������ϴ� ����, �ε������۸� ��ġ�� ���ε���. */
			m_pVIBufferCom->Bind_VIBuffers();

			/* ���ε��� ����, �ε����� �׷�. */
			m_pVIBufferCom->Render();
#ifdef _DEBUG
		}
	}
#endif // _DEBUG


	return S_OK;
}

void CEffect_Particle::ReSet_Effect()
{
	__super::ReSet_Effect();

	m_tVoidDesc.fDissolveAmount = 0.f;
	m_tVoidDesc.bDissolve = FALSE;
	m_tVoidDesc.bRender = FALSE;


	if (m_tVoidDesc.bUseSpriteAnim)
	{
		m_tSpriteDesc.bSpriteFinish = FALSE;
		m_tSpriteDesc.vUV_CurTileIndex.y = m_tSpriteDesc.vUV_MinTileCount.y;
		m_tSpriteDesc.vUV_CurTileIndex.x = m_tSpriteDesc.vUV_MinTileCount.x;
	}

	if (!m_pVIBufferCom->Get_Desc()->bRecycle)
	{
		// ��ƼŬ ���۰� ������ false�϶��� Reset�ϱ�
		//if (CVIBuffer_Particle::RISE != m_pVIBufferCom->Get_Desc()->eType_Action)
		{
			m_pVIBufferCom->ReSet();
		}

	}

}

void CEffect_Particle::End_Effect()
{
	__super::End_Effect();

	if (m_tVoidDesc.bLoop)
	{
		if(CVIBuffer_Particle::RISE != m_pVIBufferCom->Get_Desc()->eType_Action)
			ReSet_Effect();
	}

}

_bool CEffect_Particle::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	//Write_VoidDesc(Out_Json, &m_tParticleDesc);

	/* m_tParticleDesc */


	/* Sprite Desc */
	Out_Json["bLoop"] = m_tSpriteDesc.bLoop;
	Out_Json["fSequenceTerm"] = m_tSpriteDesc.fSequenceTerm;

	CJson_Utility::Write_Float2(Out_Json["vTextureSize"], m_tSpriteDesc.vTextureSize);
	CJson_Utility::Write_Float2(Out_Json["vTileSize"], m_tSpriteDesc.vTileSize);

	CJson_Utility::Write_Float2(Out_Json["vUV_MinTileCount"], m_tSpriteDesc.vUV_MinTileCount);
	CJson_Utility::Write_Float2(Out_Json["vUV_MaxTileCount"], m_tSpriteDesc.vUV_MaxTileCount);


	return true;
}

void CEffect_Particle::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);


	//*&m_tParticleDesc = *static_cast<PARTICLE_DESC*>(Load_VoidDesc(In_Json));
	//*static_cast<EFFECTVOID_DESC*>(&m_tParticleDesc) = *static_cast<EFFECTVOID_DESC*>(Load_VoidDesc(In_Json));
	//static_cast<EFFECTVOID_DESC>(m_tParticleDesc) = Load_VoidDesc(In_Json); // ��???????????????????????????


	/* m_tParticleDesc */


	/* Sprite Desc */
	m_tSpriteDesc.bLoop = In_Json["bLoop"]; 
	m_tSpriteDesc.fSequenceTerm = In_Json["fSequenceTerm"];

	CJson_Utility::Load_Float2(In_Json["vTextureSize"], m_tSpriteDesc.vTextureSize);
	CJson_Utility::Load_Float2(In_Json["vTileSize"], m_tSpriteDesc.vTileSize);

	CJson_Utility::Load_Float2(In_Json["vUV_MinTileCount"], m_tSpriteDesc.vUV_MinTileCount);
	CJson_Utility::Load_Float2(In_Json["vUV_MaxTileCount"], m_tSpriteDesc.vUV_MaxTileCount);

}



HRESULT CEffect_Particle::Change_TextureCom(wstring strProtoTextureTag)	// �� ��
{
	_uint iCurLevel = m_pGameInstance->Get_CurrentLevel();

	wstring strDiffuse	= TEXT("Diffuse");
	wstring strNormal	= TEXT("Normal");
	wstring strMask		= TEXT("Mask");
	wstring strNoise	= TEXT("Noise");
	wstring strSprite	= TEXT("Sprite");


	if (strProtoTextureTag.find(strDiffuse) != string::npos)	// ���ڿ� ã��
	{
		// ��ǻ�� �ؽ�ó ������Ʈ ���� �� ���ο� �ؽ�ó�� �ٽ� ���� (���� : �Ϲ� ��ǻ������ -> �� ��ǻ�������� �����ϰ���� ��)
		if (nullptr != m_pTextureCom[TEXTURE_DIFFUSE])
		{
			Remove_Component(TEXT("Com_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]));
			FAILED_CHECK(__super::Add_Component(iCurLevel, strProtoTextureTag, TEXT("Com_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE])));
		}
	}
	else if (strProtoTextureTag.find(strNormal) != string::npos)
	{
		// �븻 �ؽ�ó ������Ʈ ���� �� ���ο� �ؽ�ó�� �ٽ� ���� (���� : �Ϲ� �븻���� -> �� �븻������ �����ϰ���� ��)
		if (nullptr != m_pTextureCom[TEXTURE_NORAML])
		{
			Remove_Component(TEXT("Com_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NORAML]));
			FAILED_CHECK(__super::Add_Component(iCurLevel, strProtoTextureTag, TEXT("Com_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NORAML])));
		}
	}
	else if (strProtoTextureTag.find(strMask) != string::npos)
	{
		// ����ũ �ؽ�ó ������Ʈ ���� �� ���ο� �ؽ�ó�� �ٽ� ���� (���� : �Ϲ� ����ũ���� -> ���� ����ũ������ �����ϰ���� ��)
		if (nullptr != m_pTextureCom[TEXTURE_MASK])
		{
			Remove_Component(TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]));
			FAILED_CHECK(__super::Add_Component(iCurLevel, strProtoTextureTag, TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK])));
		}
	}
	else if (strProtoTextureTag.find(strNoise) != string::npos)
	{
		// ������ �ؽ�ó ������Ʈ ���� �� ���ο� �ؽ�ó�� �ٽ� ���� (���� : �Ϲ� ���������� -> �� ������������ �����ϰ���� ��)
		if (nullptr != m_pTextureCom[TEXTURE_NOISE])
		{
			Remove_Component(TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE]));
			FAILED_CHECK(__super::Add_Component(iCurLevel, strProtoTextureTag, TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE])));
		}
	}
	else if (strProtoTextureTag.find(strSprite) != string::npos)
	{
		// ��������Ʈ �ؽ�ó ������Ʈ ���� �� ���ο� �ؽ�ó�� �ٽ� ���� (���� : �Ϲ� ��������Ʈ���� -> ���� ��������Ʈ������ �����ϰ���� ��)
		if (nullptr != m_pTextureCom[TEXTURE_SPRITE])
		{
			Remove_Component(TEXT("Com_Sprite"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_SPRITE]));
			FAILED_CHECK(__super::Add_Component(iCurLevel, strProtoTextureTag, TEXT("Com_Sprite"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_SPRITE])));
		}
	}


	return S_OK;
}

HRESULT CEffect_Particle::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	{
		FAILED_CHECK(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_Particle_Point"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom)));
	}


	/* For.Com_VIBuffer */
	{
		CVIBuffer_Particle::PARTICLE_BUFFER_DESC tBufferDesc = {};
		FAILED_CHECK(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tBufferDesc));
	}


	/* For.Com_Texture */
	{
		// Diffuse
		FAILED_CHECK(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_DIFFUSE], TEXT("Com_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE])));


		// Normal
		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_NORAML])
			FAILED_CHECK(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_NORAML], TEXT("Com_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NORAML])));

		// Mask
		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_MASK])
			FAILED_CHECK(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_MASK], TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK])));

		// Noise
		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_NOISE])
			FAILED_CHECK(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_NOISE], TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE])));

		// Sprite
		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_SPRITE])
			FAILED_CHECK(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_SPRITE], TEXT("Com_Sprite"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_SPRITE])));
	}


	return S_OK;
}


HRESULT CEffect_Particle::Bind_ShaderResources()
{

	/* Matrix ============================================================================================ */
	if (m_tVoidDesc.bParentPivot)
	{
		FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_tVoidDesc.matCombined));
	}
	else
	{
		FAILED_CHECK(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	}

	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)));


	/* Texture ============================================================================================ */
	if (m_tVoidDesc.bUseSpriteAnim)
	{
		// ��������Ʈ ����̰�, 
		if(nullptr != m_pTextureCom[TEXTURE_SPRITE])	// ��������Ʈ �ؽ�ó ������ ���ε�
			FAILED_CHECK(m_pTextureCom[TEXTURE_SPRITE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_SPRITE]));
	}
	else
	{
		// �⺻�� ��ǻ� ���ε�
		FAILED_CHECK(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_DIFFUSE]));
	}

	if (nullptr != m_pTextureCom[TEXTURE_NORAML])	// �븻 �ؽ�ó ������ ���ε�
		FAILED_CHECK(m_pTextureCom[TEXTURE_NORAML]->Bind_ShaderResource(m_pShaderCom, "g_NormalTexture", m_tVoidDesc.iTextureIndex[TEXTURE_NORAML]));

	if (nullptr != m_pTextureCom[TEXTURE_MASK])		// ����ũ �ؽ�ó ������ ���ε�
		FAILED_CHECK(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_tVoidDesc.iTextureIndex[TEXTURE_MASK]));

	if (nullptr != m_pTextureCom[TEXTURE_NOISE])	// ������ �ؽ�ó ������ ���ε�
		FAILED_CHECK(m_pTextureCom[TEXTURE_NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_NOISE]));



	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_bBillBoard", &m_tVoidDesc.bBillBoard, sizeof(_bool)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fAlpha_Discard", &m_tVoidDesc.vColor_Clip.w, sizeof(_float)));

	_float3 vBlack_Discard = _float3(m_tVoidDesc.vColor_Clip.x, m_tVoidDesc.vColor_Clip.y, m_tVoidDesc.vColor_Clip.z);
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vBlack_Discard", &vBlack_Discard, sizeof(_float3)));

	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vColor_Mul", &m_tVoidDesc.vColor_Mul, sizeof(_float4)));

	/* UV ============================================================================================ */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fDegree", &m_tVoidDesc.fUV_RotDegree, sizeof(_float)));


	// ����Ʈ ����(��ƼŬ ���ۿ��� �������� ������)
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_EffectDesc", m_pVIBufferCom->Get_ParticleShaderInfoDescs().data(), _uint(sizeof(CVIBuffer_Particle::PARTICLE_SHADER_INFO_DESC) * m_pVIBufferCom->Get_ParticleShaderInfoDescs().size())));

	// ��������Ʈ
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_bSprite", &m_tVoidDesc.bUseSpriteAnim, sizeof(_bool)));
	if (m_tVoidDesc.bUseSpriteAnim)
	{
		m_tVoidDesc.vUV_Offset = { (_float)(m_tSpriteDesc.vUV_CurTileIndex.x * m_tSpriteDesc.vTileSize.x) / m_tSpriteDesc.vTextureSize.x
									, (_float)(m_tSpriteDesc.vUV_CurTileIndex.y * m_tSpriteDesc.vTileSize.y) / m_tSpriteDesc.vTextureSize.y };

		m_tVoidDesc.vUV_Scale = { (_float)m_tSpriteDesc.vTileSize.x / m_tSpriteDesc.vTextureSize.x
								, (_float)m_tSpriteDesc.vTileSize.y / m_tSpriteDesc.vTextureSize.y };


		FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_UVOffset", &m_tVoidDesc.vUV_Offset, sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_UVScale", &m_tVoidDesc.vUV_Scale, sizeof(_float2)));
	}


	/* Camera ============================================================================================ */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4)));
	_float3 vCamDirectionFloat3 = m_pGameInstance->Get_CamDirection();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamDirection", &vCamDirectionFloat3, sizeof(_float3)));

	_float fCamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fCamFar", &fCamFar, sizeof(_float)));


	/* ETC ============================================================================================ */
	FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture"));


	/* Rim Bloom */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_tVoidDesc.vBloomPower, sizeof(_float3)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vRimColor", &m_tVoidDesc.vRimColor, sizeof(_float4)));


	return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
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

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
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

	//Safe_Release(m_pModelCom);
	//Safe_Release(m_pVIBufferCom_Model);

	Safe_Release(m_pVIBufferCom);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
}

