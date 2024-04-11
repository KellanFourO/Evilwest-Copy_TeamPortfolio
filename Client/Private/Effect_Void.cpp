#include "stdafx.h"
#include "..\Public\Effect_Void.h"

#include "GameInstance.h"

#include "Effect.h"

CEffect_Void::CEffect_Void(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CAlphaObject(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = FALSE;
}

CEffect_Void::CEffect_Void(const CEffect_Void & rhs)
	: CAlphaObject(rhs)
{
}

HRESULT CEffect_Void::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEffect_Void::Initialize(void* pArg)
{	
	XMStoreFloat4x4(&m_tVoidDesc.matPivot, XMMatrixIdentity());
	XMStoreFloat4x4(&m_tVoidDesc.matCombined, XMMatrixIdentity());
	
	m_tVoidDesc = *static_cast<EFFECTVOID_DESC*>(pArg);

		

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}

void CEffect_Void::Priority_Tick(_float fTimeDelta)
{

	
}

void CEffect_Void::Tick(_float fTimeDelta)
{
	/* ���� Ʋ ��� */
	//if (m_tEffectDesc.bActive_Tool)
	//{
	//	m_fSequenceTime = m_fLifeTime + m_fRemainTime;

	//	if (m_tEffectDesc.bPlay)
	//	{
	//		// �������� �����ð��� ������ ���� ����(�� ����Ʈ ����)
	//		if (m_fWaitingAcc <= m_fWaitingTime)
	//		{
	//			m_fWaitingAcc += fTimeDelta;

	//			if (m_fWaitingAcc >= m_fWaitingTime)
	//			{
	//				m_tEffectDesc.bRender = TRUE;
	//			}
	//			else
	//				return;
	//		}

	//		// ������Ÿ�� ���� ����
	//		m_fTimeAcc += fTimeDelta;
	//		m_fLifeTimeRatio = min(1.0f, m_fTimeAcc / m_fLifeTime);

	//		/* ======================= ������ Ÿ�� ���� ���� ======================= */




	//		/* ======================= ������ Ÿ�� ���� ��  ======================= */

	//		if (m_fTimeAcc >= m_fLifeTime)
	//		{
	//			// ���� ���ð� ���� ����
	//			m_fRemainAcc += fTimeDelta;

	//			// ������ ����
	//			m_tEffectDesc.bDissolve = TRUE;
	//			if (m_tEffectDesc.bDissolve)
	//			{
	//				m_tEffectDesc.fDissolveAmount = Easing::LerpToType(0.f, 1.f, m_fRemainAcc, m_fRemainTime, m_tEffectDesc.eType_Easing);
	//			}
	//			// ������ ��


	//			// �� ����Ʈ�� Ÿ�Ӷ��� ����
	//			if (m_fRemainAcc >= m_fRemainTime)
	//			{
	//				m_tEffectDesc.bRender = TRUE;
	//				return;
	//			}
	//		}
	//	}
	//}

}

void CEffect_Void::Late_Tick(_float fTimeDelta)
{



}

HRESULT CEffect_Void::Render()
{


	return S_OK;
}

void CEffect_Void::Update_PivotMat()
{
	if (nullptr != m_pOwner)	// ���� ����Ʈ�� �ְ�
	{
		if (m_tVoidDesc.bParentPivot)		//������ ��Ʈ������ ����ҰŰ�
		{
			CGameObject* pParentOwner = m_pOwner->Get_Object_Owner();
			if (nullptr != pParentOwner)
			{
				// �θ�(Effect)�� ���ʰ� ������ �θ��� �Ĺ��� ��Ʈ���� ���
				m_tVoidDesc.matPivot = dynamic_cast<CEffect*>(m_pOwner)->Get_Desc()->matCombined;
				XMStoreFloat4x4(&m_tVoidDesc.matCombined, m_pTransformCom->Get_WorldMatrix() * m_tVoidDesc.matPivot); // �� * ����
			}
			else
			{
				// �θ�(Effect)�� ���ʰ� ������ �θ��� ���常 ���
				m_tVoidDesc.matPivot = m_pOwner->Get_Transform()->Get_WorldFloat4x4();
				XMStoreFloat4x4(&m_tVoidDesc.matCombined, m_pTransformCom->Get_WorldMatrix() * m_tVoidDesc.matPivot);
			}
		}
	}
}

_bool CEffect_Void::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Write_VoidDesc(Out_Json);

	return true;
}

void CEffect_Void::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);


	Load_VoidDesc(In_Json);

}


void CEffect_Void::Write_VoidDesc(json& Out_Json)
{
	Out_Json["eType_Effect"] = m_tVoidDesc.eType_Effect;
	Out_Json["eType_Easing"] = m_tVoidDesc.eType_Easing;


	string strTag = "";
	m_pGameInstance->WString_To_String(m_tVoidDesc.strProtoTag, strTag);
	Out_Json["strProtoTag"] = strTag;

	m_pGameInstance->WString_To_String(m_tVoidDesc.strPartTag, strTag);
	Out_Json["strPartTag"] = strTag;


	/* Model */
	for (_int i = 0; i < 2; i++)
	{
		m_pGameInstance->WString_To_String(m_tVoidDesc.strModelTag[i], strTag);
		Out_Json["strModelTag"][i] = strTag;
	}

	/* Texture */
	for (_int i = 0; i < (_int)TEXTURE_END; i++)
	{
		m_pGameInstance->WString_To_String(m_tVoidDesc.strTextureTag[i], strTag);
		Out_Json["strTextureTag"][i] = strTag;

		Out_Json["iTextureIndex"][i] = m_tVoidDesc.iTextureIndex[i];
	}

	Out_Json["iCurNumInstance"] = m_tVoidDesc.iCurNumInstance;

	Out_Json["iRenderGroup"] = m_tVoidDesc.iRenderGroup;
	Out_Json["iShaderPassIndex"] = m_tVoidDesc.iShaderPassIndex;

	Out_Json["bBillBoard"] = m_tVoidDesc.bBillBoard;
	Out_Json["bDissolve"] = m_tVoidDesc.bDissolve;
	Out_Json["bSoft"] = m_tVoidDesc.bSoft;
	Out_Json["bUseMask"] = m_tVoidDesc.bUseMask;

	CJson_Utility::Write_Float2(Out_Json["vUV_Offset"], m_tVoidDesc.vUV_Offset);
	CJson_Utility::Write_Float2(Out_Json["vUV_Scale"], m_tVoidDesc.vUV_Scale);

	Out_Json["fUV_RotDegree"] = m_tVoidDesc.fUV_RotDegree;

	Out_Json["bUV_Wave"] = m_tVoidDesc.bUV_Wave;
	CJson_Utility::Write_Float2(Out_Json["vUV_WaveSpeed"], m_tVoidDesc.vUV_WaveSpeed);
	CJson_Utility::Write_Float2(Out_Json["vUV_Offset_Mask"], m_tVoidDesc.vUV_Offset_Mask);
	CJson_Utility::Write_Float2(Out_Json["vUV_Scale_Mask"], m_tVoidDesc.vUV_Scale_Mask);


	CJson_Utility::Write_Float4(Out_Json["vColor_Offset"], m_tVoidDesc.vColor_Offset);
	CJson_Utility::Write_Float4(Out_Json["vColor_Clip"], m_tVoidDesc.vColor_Clip);
	CJson_Utility::Write_Float4(Out_Json["vColor_Mul"], m_tVoidDesc.vColor_Mul);
	Out_Json["eMode_Color"] = m_tVoidDesc.eMode_Color;
	

	/* Rim & Bloom */
	CJson_Utility::Write_Float3(Out_Json["vBloomPower"], m_tVoidDesc.vBloomPower);

	CJson_Utility::Write_Float4(Out_Json["vRimColor"], m_tVoidDesc.vRimColor);
	Out_Json["fRimPower"] = m_tVoidDesc.fRimPower;

	/* States */
	Out_Json["bPlay"]			= m_tVoidDesc.bPlay;
	Out_Json["bLoop"]			= m_tVoidDesc.bLoop;
	Out_Json["bReverse"]		= m_tVoidDesc.bReverse;
	Out_Json["bRender"]			= m_tVoidDesc.bRender;
	Out_Json["bUseSpriteAnim"]	= m_tVoidDesc.bUseSpriteAnim;
	Out_Json["bUseRigidBody"]	= m_tVoidDesc.bUseRigidBody;


	/* Times */
	Out_Json["fWaitingTime"]	= m_tVoidDesc.fWaitingTime;
	Out_Json["fLifeTime"]		= m_tVoidDesc.fLifeTime;
	Out_Json["fRemainTime"]		= m_tVoidDesc.fRemainTime;
	Out_Json["fSequenceTime"]	= m_tVoidDesc.fSequenceTime;


	/* ���� */
	Out_Json["bParentPivot"] = m_tVoidDesc.bParentPivot;


}

void CEffect_Void::Load_VoidDesc(const json& In_Json)
{
	/* Effect_Void */
	m_tVoidDesc.eType_Effect = (TYPE_EFFECT)In_Json["eType_Effect"];	
	m_tVoidDesc.eType_Easing = (EASING_TYPE)In_Json["eType_Easing"];


	string strTag = "";
	strTag = static_cast<string>(In_Json["strProtoTag"]);
	m_pGameInstance->String_To_WString(strTag, m_tVoidDesc.strProtoTag);


	strTag = static_cast<string>(In_Json["strPartTag"]);
	m_pGameInstance->String_To_WString(strTag, m_tVoidDesc.strPartTag);


	/* Model */
	for (_int i = 0; i < 2; i++)
	{
		strTag = static_cast<string>(In_Json["strModelTag"][i]);
		m_pGameInstance->String_To_WString(strTag, m_tVoidDesc.strModelTag[i]);
	}


	/* Texture */
	for (_int i = 0; i < (_int)TEXTURE_END; i++)
	{
		strTag = static_cast<string>(In_Json["strTextureTag"][i]);
		m_pGameInstance->String_To_WString(strTag, m_tVoidDesc.strTextureTag[i]);

		m_tVoidDesc.iTextureIndex[i] = (_int)In_Json["iTextureIndex"][i];
	}

	m_tVoidDesc.iCurNumInstance = (_int)In_Json["iCurNumInstance"];

	m_tVoidDesc.iRenderGroup = (_int)In_Json["iRenderGroup"];
	m_tVoidDesc.iShaderPassIndex = (_int)In_Json["iShaderPassIndex"];

	m_tVoidDesc.bBillBoard = (_bool)In_Json["bBillBoard"];
	m_tVoidDesc.bDissolve = (_bool)In_Json["bDissolve"];

	if (In_Json.contains("bSoft")) // �ٽ� ���� �� if�� ����
	{
		m_tVoidDesc.bSoft = (_bool)In_Json["bSoft"];
	}

	if (In_Json.contains("bUseMask")) // �ٽ� ���� �� if�� ����
	{
		m_tVoidDesc.bUseMask = (_bool)In_Json["bUseMask"];
	}


	CJson_Utility::Load_Float2(In_Json["vUV_Offset"], m_tVoidDesc.vUV_Offset);
	CJson_Utility::Load_Float2(In_Json["vUV_Scale"], m_tVoidDesc.vUV_Scale);

	m_tVoidDesc.fUV_RotDegree = (_float)In_Json["fUV_RotDegree"];

	m_tVoidDesc.bUV_Wave = (_bool)In_Json["bUV_Wave"];

	if (In_Json.contains("vUV_WaveSpeed")) // �ٽ� ���� �� if�� ����
		CJson_Utility::Load_Float2(In_Json["vUV_WaveSpeed"], m_tVoidDesc.vUV_WaveSpeed);


	if (In_Json.contains("vUV_Offset_Mask")) // �ٽ� ���� �� if�� ����
	{
		CJson_Utility::Load_Float2(In_Json["vUV_Offset_Mask"], m_tVoidDesc.vUV_Offset_Mask);
		CJson_Utility::Load_Float2(In_Json["vUV_Scale_Mask"], m_tVoidDesc.vUV_Scale_Mask);
	}


	CJson_Utility::Load_Float4(In_Json["vColor_Offset"], m_tVoidDesc.vColor_Offset);
	CJson_Utility::Load_Float4(In_Json["vColor_Clip"], m_tVoidDesc.vColor_Clip);
	CJson_Utility::Load_Float4(In_Json["vColor_Mul"], m_tVoidDesc.vColor_Mul);
	if(In_Json.contains("eMode_Color")) //! TODO �ٽ� ���� �� if�� ����
		m_tVoidDesc.eMode_Color = In_Json["eMode_Color"];

	/* Rim & Bloom */
	CJson_Utility::Load_Float3(In_Json["vBloomPower"], m_tVoidDesc.vBloomPower);	
	//CJson_Utility::Load_Float4(In_Json["vBloom_Clip"], m_tVoidDesc.vBloom_Clip); �����ϰ� �ּ� Ǯ��

	CJson_Utility::Load_Float4(In_Json["vRimColor"], m_tVoidDesc.vRimColor);
	m_tVoidDesc.fRimPower = (_float)In_Json["fRimPower"];


	/* States */
	m_tVoidDesc.bPlay = (_bool)In_Json["bPlay"];
	m_tVoidDesc.bLoop = (_bool)In_Json["bLoop"];
	m_tVoidDesc.bReverse = (_bool)In_Json["bReverse"];
	m_tVoidDesc.bRender = (_bool)In_Json["bRender"];
	m_tVoidDesc.bUseSpriteAnim = (_bool)In_Json["bUseSpriteAnim"];
	m_tVoidDesc.bUseRigidBody = (_bool)In_Json["bUseRigidBody"];


	m_tVoidDesc.fWaitingTime	= (_float)In_Json["fWaitingTime"];
	m_tVoidDesc.fLifeTime		= (_float)In_Json["fLifeTime"];
	m_tVoidDesc.fRemainTime		= (_float)In_Json["fRemainTime"];
	m_tVoidDesc.fSequenceTime	= (_float)In_Json["fSequenceTime"];


	/* ���� */
	m_tVoidDesc.bParentPivot = (_bool)In_Json["bParentPivot"];

}


void CEffect_Void::ReNumber_PartTag()
{
	// ���� ��Ʈ �±׿��� ��ȣ�� �����´�
	_int iNumLength = 3; // ��Ʈ ��ȣ�� ���� (000, 001, 002)
	wstring strNum = m_tVoidDesc.strPartTag.substr(m_tVoidDesc.strPartTag.size() - iNumLength);

	// ���� ��ȣ�� ������ ��ȯ
	_int iCurrentNum = stoi(strNum);

	// ���� ��ȣ�� 0���� ū�� Ȯ���ϰ�, 0���� ũ�ٸ� �ϳ� ����
	if (iCurrentNum > 0)
	{
		// ��ȣ�� �ϳ� ���δ�.
		--iCurrentNum;

		// ���ο� ��ȣ�� ���ڿ��� ��ȯ
		wstring strNewNum;
		if (iCurrentNum < 10)
			strNewNum = L"00" + std::to_wstring(iCurrentNum);
		else if (iCurrentNum < 100)
			strNewNum = L"0" + std::to_wstring(iCurrentNum);
		else
			strNewNum = std::to_wstring(iCurrentNum);

		// ��Ʈ �±׿��� ���� ��ȣ �κ��� �� ��ȣ�� ��ü�Ѵ�.
		m_tVoidDesc.strPartTag.replace(m_tVoidDesc.strPartTag.size() - iNumLength, iNumLength, strNewNum);
	}

}


void CEffect_Void::ReSet_Effect()
{
	// �ڽĿ��� ���� �ʿ��� ���� �ൿ���ֱ�

	m_tVoidDesc.Reset_Times();

	//m_tEffectDesc.fDissolveAmount	 = 0.f;
	//m_tEffectDesc.bDissolve		 = FALSE;
	//m_tEffectDesc.bRender			 = FALSE;

}

void CEffect_Void::Init_ReSet_Effect()
{
	m_tVoidDesc.Reset_Times();

}

void CEffect_Void::End_Effect()
{
	// �ڽĿ��� ���� �ʿ��� ���� �ൿ���ֱ�

	//__super::End_Effect();

	//if (m_tEffectDesc.bLoop)
	//{
	//	ReSet_Effect();
	//}

}



void CEffect_Void::Free()
{
	__super::Free();

}

