#include "stdafx.h"
#include "..\Public\Effect_Rect.h"

#include "GameInstance.h"


CEffect_Rect::CEffect_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CEffect_Void(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = FALSE;
}

CEffect_Rect::CEffect_Rect(const CEffect_Rect & rhs)
	: CEffect_Void(rhs)
{
}

HRESULT CEffect_Rect::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CEffect_Rect::Initialize(void* pArg)
{	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	// void* pArg : �����Ͽ� ��ü�� �����Ҷ� �������� ä���� ���� �����͸� �߰������� �� �ʱ�ȭ���ֱ����ؼ�. 
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_Scaling(0.5f, 1.f, 1.f);


	return S_OK;
}

void CEffect_Rect::Priority_Tick(_float fTimeDelta)
{

	
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	if (m_tVoidDesc.bActive_Tool)
	{	
		m_tVoidDesc.fSequenceTime = m_tVoidDesc.fLifeTime + m_tVoidDesc.fRemainTime;

		if (m_tVoidDesc.bPlay)
		{
			m_tVoidDesc.fSequenceAcc += fTimeDelta;

			// �������� �����ð��� ������ ���� ����(�� ����Ʈ ����)
			if (m_tVoidDesc.fWaitingAcc <= m_tVoidDesc.fWaitingTime)
			{
				m_tVoidDesc.fWaitingAcc += fTimeDelta;

				if (m_tVoidDesc.fWaitingAcc >= m_tVoidDesc.fWaitingTime)
				{
					m_tVoidDesc.bRender = TRUE;
				}
				else
					return;
			}

			// ������Ÿ�� ���� ����
			m_tVoidDesc.fTimeAcc += fTimeDelta;
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
							(m_tSpriteDesc.vUV_CurTileIndex.y)++;											// ���� �ε��� ����
							m_tSpriteDesc.vUV_CurTileIndex.x = m_tSpriteDesc.vUV_MinTileCount.x;			// ���� �ε��� �ʱ�ȭ

							if (m_tSpriteDesc.vUV_CurTileIndex.y == m_tSpriteDesc.vUV_MaxTileCount.y)		// ���� �ε����� ������ �Դٸ�
							{
								m_tSpriteDesc.bSpriteFinish = TRUE;											// ��������Ʈ �ִ� ��	
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
				// ������ ��


				// �� ����Ʈ�� Ÿ�Ӷ��� ����
				if (m_tVoidDesc.fRemainAcc >= m_tVoidDesc.fRemainTime)
				{
					m_tVoidDesc.fDissolveAmount = 1.f;
					m_tVoidDesc.bRender = TRUE;
					return;
				}
			}
		}
	}

};

void CEffect_Rect::Late_Tick(_float fTimeDelta)
{

	if (m_tVoidDesc.bActive_Tool)
	{
		if (m_tVoidDesc.bRender)
		{
			__super::Update_PivotMat();

			Compute_CamDistance();

			//FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_EFFECT, this));
			FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tVoidDesc.iRenderGroup, this), );
		}
	}

}

HRESULT CEffect_Rect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* �� ���̴��� 0��° �н��� �׸��ž�. */
	m_pShaderCom->Begin(m_tVoidDesc.iShaderPassIndex);

	/* ���� �׸������ϴ� ����, �ε������۸� ��ġ�� ���ε���. */
	m_pVIBufferCom->Bind_VIBuffers();

	/* ���ε��� ����, �ε����� �׷�. */
	m_pVIBufferCom->Render();

	return S_OK;
}

_bool CEffect_Rect::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);


	/* Sprite Desc */
	Out_Json["bLoop"] = m_tSpriteDesc.bLoop;
	Out_Json["fSequenceTerm"] = m_tSpriteDesc.fSequenceTerm;

	CJson_Utility::Write_Float2(Out_Json["vTextureSize"], m_tSpriteDesc.vTextureSize);
	CJson_Utility::Write_Float2(Out_Json["vTileSize"], m_tSpriteDesc.vTileSize);

	CJson_Utility::Write_Float2(Out_Json["vUV_MinTileCount"], m_tSpriteDesc.vUV_MinTileCount);
	CJson_Utility::Write_Float2(Out_Json["vUV_MaxTileCount"], m_tSpriteDesc.vUV_MaxTileCount);


	/* Distortion */
	Out_Json["fSequenceTerm"] = m_tDistortionDesc.fSequenceTerm;

	CJson_Utility::Write_Float3(Out_Json["vScrollSpeeds"], m_tDistortionDesc.vScrollSpeeds);
	CJson_Utility::Write_Float3(Out_Json["vScales"], m_tDistortionDesc.vScales);
	CJson_Utility::Write_Float2(Out_Json["vDistortion1"], m_tDistortionDesc.vDistortion1);
	CJson_Utility::Write_Float2(Out_Json["vDistortion2"], m_tDistortionDesc.vDistortion2);
	CJson_Utility::Write_Float2(Out_Json["vDistortion3"], m_tDistortionDesc.vDistortion3);

	Out_Json["fDistortionScale"] = m_tDistortionDesc.fDistortionScale;
	Out_Json["fDistortionBias"] = m_tDistortionDesc.fDistortionBias;

	return true;
}

void CEffect_Rect::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);


	/* Sprite Desc */
	m_tSpriteDesc.bLoop = In_Json["bLoop"];
	m_tSpriteDesc.fSequenceTerm = In_Json["fSequenceTerm"];

	CJson_Utility::Load_Float2(In_Json["vTextureSize"], m_tSpriteDesc.vTextureSize);
	CJson_Utility::Load_Float2(In_Json["vTileSize"], m_tSpriteDesc.vTileSize);

	CJson_Utility::Load_Float2(In_Json["vUV_MinTileCount"], m_tSpriteDesc.vUV_MinTileCount);
	CJson_Utility::Load_Float2(In_Json["vUV_MaxTileCount"], m_tSpriteDesc.vUV_MaxTileCount);


	/* Distortion */
	m_tDistortionDesc.fSequenceTerm = In_Json["fSequenceTerm"];

	CJson_Utility::Load_Float3(In_Json["vScrollSpeeds"], m_tDistortionDesc.vScrollSpeeds);
	CJson_Utility::Load_Float3(In_Json["vScales"], m_tDistortionDesc.vScales);
	CJson_Utility::Load_Float2(In_Json["vDistortion1"], m_tDistortionDesc.vDistortion1);
	CJson_Utility::Load_Float2(In_Json["vDistortion2"], m_tDistortionDesc.vDistortion2);
	CJson_Utility::Load_Float2(In_Json["vDistortion3"], m_tDistortionDesc.vDistortion3);

	m_tDistortionDesc.fDistortionScale = In_Json["fDistortionScale"];
	m_tDistortionDesc.fDistortionBias = In_Json["fDistortionBias"];

}

void CEffect_Rect::ReSet_Effect()
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

}

void CEffect_Rect::End_Effect()
{
	__super::End_Effect();

	if (m_tVoidDesc.bLoop)
	{
		ReSet_Effect();
	}
}

HRESULT CEffect_Rect::Change_TextureCom(wstring strProtoTextureTag)
{
	_uint iCurLevel = m_pGameInstance->Get_CurrentLevel();

	wstring strDiffuse = TEXT("Diffuse");
	wstring strNormal = TEXT("Normal");
	wstring strMask = TEXT("Mask");
	wstring strNoise = TEXT("Noise");
	wstring strSprite = TEXT("Sprite");


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


HRESULT CEffect_Rect::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	FAILED_CHECK(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_EffectTex"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom)));


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;



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

HRESULT CEffect_Rect::Bind_ShaderResources()
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
		if (nullptr != m_pTextureCom[TEXTURE_SPRITE])	// ��������Ʈ �ؽ�ó ������ ���ε�
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


	/* Camera ============================================================================================ */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4)));
	_float3 vCamDirectionFloat3 = m_pGameInstance->Get_CamDirection();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamDirection", &vCamDirectionFloat3, sizeof(_float3)));

	_float fCamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fCamFar", &fCamFar, sizeof(_float)));



	/* Sprite =========================================================================================*/
	if (m_tVoidDesc.bUseSpriteAnim)
	{
		m_tVoidDesc.vUV_Offset = { (_float)(m_tSpriteDesc.vUV_CurTileIndex.x * m_tSpriteDesc.vTileSize.x) / m_tSpriteDesc.vTextureSize.x
									, (_float)(m_tSpriteDesc.vUV_CurTileIndex.y * m_tSpriteDesc.vTileSize.y) / m_tSpriteDesc.vTextureSize.y };

		m_tVoidDesc.vUV_Scale = { (_float)m_tSpriteDesc.vTileSize.x / m_tSpriteDesc.vTextureSize.x
								, (_float)m_tSpriteDesc.vTileSize.y / m_tSpriteDesc.vTextureSize.y };
	}


	/* UV ============================================================================================ */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_UVOffset", &m_tVoidDesc.vUV_Offset, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_UVScale", &m_tVoidDesc.vUV_Scale, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fDegree", &m_tVoidDesc.fUV_RotDegree, sizeof(_float)));

	/* Distortion */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fFrameTime", &m_tVoidDesc.fTimeAcc, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vScrollSpeeds", &m_tDistortionDesc.vScrollSpeeds, sizeof(_float3)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vScales", &m_tDistortionDesc.vScales, sizeof(_float3)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vDistortion1", &m_tDistortionDesc.vDistortion1, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vDistortion2", &m_tDistortionDesc.vDistortion2, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vDistortion3", &m_tDistortionDesc.vDistortion3, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fDistortionScale", &m_tDistortionDesc.fDistortionScale, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fDistortionBias", &m_tDistortionDesc.fDistortionBias, sizeof(_float)));


	/* Rim & Bloom ====================================================================================== */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_tVoidDesc.vBloomPower, sizeof(_float3)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vRimColor", &m_tVoidDesc.vRimColor, sizeof(_float4)));


	/* ETC ============================================================================================ */
	//FAILED_CHECK(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture"));

	return S_OK;
}

CEffect_Rect * CEffect_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CEffect_Rect*		pInstance = new CEffect_Rect(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Rect::Clone(void* pArg)
{
	CEffect_Rect*		pInstance = new CEffect_Rect(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Rect::Pool()
{
	return new CEffect_Rect(*this);
}

void CEffect_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
}

