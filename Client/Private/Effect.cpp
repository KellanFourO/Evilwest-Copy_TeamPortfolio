#include "stdafx.h"
#include "..\Public\Effect.h"

#include "GameInstance.h"
#include "Effect_Manager.h"

#include "Effect_Particle.h"
#include "Effect_Rect.h"
#include "Effect_Instance.h"
#include "Effect_Trail.h"

#include "Bone.h"
#include "Character.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CGameObject(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = FALSE;
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{	
	XMStoreFloat4x4(&m_tEffectDesc.matPivot, XMMatrixIdentity());
	XMStoreFloat4x4(&m_tEffectDesc.matCombined, XMMatrixIdentity());


	m_tEffectDesc = *(EFFECT_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}

void CEffect::Priority_Tick(_float fTimeDelta)
{
#ifdef _DEBUG

		if (m_tEffectDesc.bActive_Tool)
		{
#endif // _DEBUG
			for (auto& Pair : m_PartObjects)
			{
				if (nullptr != Pair.second)
					Pair.second->Priority_Tick(fTimeDelta);
			}
#ifdef _DEBUG
		}

#endif // _DEBUG

}

void CEffect::Tick(_float fTimeDelta)
{

#ifdef _DEBUG
		if (m_tEffectDesc.bActive_Tool)
		{
#endif // _DEBUG

			m_tEffectDesc.fSequenceTime = m_tEffectDesc.fWaitingTime + m_tEffectDesc.fLifeTime + m_tEffectDesc.fRemainTime;
			//m_fEasingTimeAcc = Easing::LerpToType(0.f, m_tEffectDesc.fSequenceTime, m_tEffectDesc.fSequenceAcc, m_tEffectDesc.fSequenceTime, m_tEffectDesc.eType_Easing);

			if (m_tEffectDesc.bPlay)
			{
				m_tEffectDesc.fSequenceAcc += fTimeDelta;

				// �������� �����ð��� ������ ���� ����(����Ʈ ����)
				if (m_tEffectDesc.fWaitingAcc < m_tEffectDesc.fWaitingTime)
				{
					m_tEffectDesc.fWaitingAcc += fTimeDelta;

					if (m_tEffectDesc.fWaitingAcc >= m_tEffectDesc.fWaitingTime)
					{
						m_tEffectDesc.bRender = true;

						if (nullptr != m_pTrail)	// Ʈ������ �����ϸ� �̶� �÷��� ����
							m_pTrail->Set_Play(TRUE);
					}					
					else
						return;
				}

				// �ð� ���� ����
				m_tEffectDesc.fTimeAcc += fTimeDelta;
				m_tEffectDesc.fLifeTimeRatio = min(1.0f, m_tEffectDesc.fTimeAcc / m_tEffectDesc.fLifeTime);


				/* ======================= ������ Ÿ�� ���� ���� ======================= */

				// Ʈ���� ������Ʈ
				if (nullptr != m_pTrail)	// Ʈ������ �����ϰ�
				{
					//m_pTrail->Set_Play(m_bTrailPlay);
					if (m_tEffectDesc.bParentPivot)
					{
						// ������ ��Ʈ������ ����ҰŸ� �Ĺ��ε� ��Ʈ���� ���
						m_pTrail->Tick_Trail(fTimeDelta, m_tEffectDesc.matCombined);
					}
					else
					{
						// �ƴϸ� �� ����
						m_pTrail->Tick_Trail(fTimeDelta, m_pTransformCom->Get_WorldFloat4x4());
					}
				}


				/* ======================= ������ Ÿ�� ���� ��  ======================= */

				if (m_tEffectDesc.fTimeAcc >= m_tEffectDesc.fLifeTime)
				{
					// ���� ���ð� ���� ����
					m_tEffectDesc.fRemainAcc += fTimeDelta;

					if (m_tEffectDesc.fRemainAcc >= m_tEffectDesc.fRemainTime)
					{
						End_Effect();
						return;
					}
				}


				/* ��Ʈ ����Ʈ�� Tick */
				for (auto& Pair : m_PartObjects)
				{
					if (nullptr != Pair.second)
					{
						//dynamic_cast<CEffect_Void*>(Pair.second)->Set_TimeAcc(m_fEasingTimeAcc);
						Pair.second->Tick(fTimeDelta);
					}

				}

			}
#ifdef _DEBUG
		}
#endif // _DEBUG
}


void CEffect::Late_Tick(_float fTimeDelta)
{
#ifdef _DEBUG
		if (m_tEffectDesc.bActive_Tool)
		{
#endif // _DEBUG

			Update_PivotMat();

			for (auto& Pair : m_PartObjects)
			{
				if (nullptr != Pair.second)
					Pair.second->Late_Tick(fTimeDelta);
			}


#ifdef _DEBUG
		}
#endif // _DEBUG
}

HRESULT CEffect::Render()
{
#ifdef _DEBUG
		if (m_tEffectDesc.bActive_Tool)
		{
#endif // _DEBUG
			if (m_tEffectDesc.bPlay)
			{
				if (m_tEffectDesc.bRender)
				{
					for (auto& Pair : m_PartObjects)
					{
						if (nullptr != Pair.second)
							Pair.second->Render();
					}
				}

			}

#ifdef _DEBUG
		}
#endif // _DEBUG

	return S_OK;
}

_bool CEffect::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json["Effect"]["eType_Dead"] = m_tEffectDesc.eType_Dead;

	Out_Json["Effect"]["bPlay"] = m_tEffectDesc.bPlay;
	Out_Json["Effect"]["bLoop"] = m_tEffectDesc.bLoop;
	Out_Json["Effect"]["bFinished"] = m_tEffectDesc.bFinished;
	Out_Json["Effect"]["bReverse"] = m_tEffectDesc.bReverse;
	Out_Json["Effect"]["bRender"] = m_tEffectDesc.bRender;

	Out_Json["Effect"]["iPartSize"] = m_tEffectDesc.iPartSize;

	Out_Json["Effect"]["fLifeTime"] = m_tEffectDesc.fLifeTime;
	Out_Json["Effect"]["fWaitingTime"] = m_tEffectDesc.fWaitingTime;
	Out_Json["Effect"]["fRemainTime"] = m_tEffectDesc.fRemainTime;
	Out_Json["Effect"]["fSequenceTime"] = m_tEffectDesc.fSequenceTime;

	Out_Json["Effect"]["bParentPivot"] = m_tEffectDesc.bParentPivot;

	_int iCount = 0;
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second)
		{
			Pair.second->Write_Json(Out_Json["Part"][iCount]);
			iCount += 1;
		}
	}

	return true;
}

void CEffect::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	m_tEffectDesc.eType_Dead = In_Json["Effect"]["eType_Dead"];

	m_tEffectDesc.bPlay			= In_Json["Effect"]["bPlay"];
	m_tEffectDesc.bLoop			= In_Json["Effect"]["bLoop"];
	m_tEffectDesc.bFinished		= In_Json["Effect"]["bFinished"];
	m_tEffectDesc.bReverse		= In_Json["Effect"]["bReverse"];
	m_tEffectDesc.bRender		= In_Json["Effect"]["bRender"];

	m_tEffectDesc.iPartSize		= In_Json["Effect"]["iPartSize"];

	m_tEffectDesc.fLifeTime		= In_Json["Effect"]["fLifeTime"];
	m_tEffectDesc.fWaitingTime	= In_Json["Effect"]["fWaitingTime"];
	m_tEffectDesc.fRemainTime	= In_Json["Effect"]["fRemainTime"];
	m_tEffectDesc.fSequenceTime = In_Json["Effect"]["fSequenceTime"];

	m_tEffectDesc.bParentPivot = In_Json["Effect"]["bParentPivot"];

	if (m_PartObjects.empty() && 0 < m_tEffectDesc.iPartSize)
	{
		for (_int i = 0; i < m_tEffectDesc.iPartSize; ++i)
		{
			CEffect_Void::EFFECTVOID_DESC	tVoidDesc = {};
			string strTag = "";

			strTag = In_Json["Part"][i]["strProtoTag"];
			m_pGameInstance->String_To_WString(strTag, tVoidDesc.strProtoTag);

			strTag = In_Json["Part"][i]["strPartTag"];
			m_pGameInstance->String_To_WString(strTag, tVoidDesc.strPartTag);
		

			for (_int k = 0; k < ECast(CVIBuffer_Effect_Model_Instance::MORPH_END); ++k)
			{
				strTag = In_Json["Part"][i]["strModelTag"][k];
				m_pGameInstance->String_To_WString(strTag, tVoidDesc.strModelTag[k]);
			}

			for (_int j = 0; j < (_int)CEffect_Void::TEXTURE_END; ++j)
			{
				strTag = In_Json["Part"][i]["strTextureTag"][j];
				m_pGameInstance->String_To_WString(strTag, tVoidDesc.strTextureTag[j]);
			}

			tVoidDesc.iCurNumInstance = In_Json["Part"][i]["iCurNumInstance"];

			tVoidDesc.bUseSpriteAnim = In_Json["Part"][i]["bUseSpriteAnim"];
			tVoidDesc.bUseRigidBody = In_Json["Part"][i]["bUseRigidBody"];

			Ready_PartObjects(tVoidDesc.strProtoTag, tVoidDesc.strPartTag, &tVoidDesc);
		}
	}

	_int iCount = 0;
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second)
		{
			Pair.second->Load_FromJson(In_Json["Part"][iCount]);
			dynamic_cast<CEffect_Void*>(Pair.second)->Set_Object_Owner(this);
			iCount += 1;
		}
	}


	Init_ReSet_Effect();

}

void CEffect::Update_PivotMat()
{
	if (nullptr != m_pOwner)	// ������ �����ϰ�,
	{
		// ����Ʈ�� ������ �׾���.
		if (m_pOwner->Is_Dead())
		{
			if (nullptr != m_pTrail)	// Ʈ������ �����ִ� ����Ʈ�� Ʈ���� ����
				m_pTrail->Set_Play(FALSE);
			

			// ����Ʈ�� ������ �׾����� ����Ʈ Ǯ�� �ݳ�
			EFFECT_MANAGER->Return_ToPool(this);

			return;
		}

		if (m_tEffectDesc.bParentPivot)
		{	
			if (m_tEffectDesc.bUseSocket)
			{
				// ���ϻ���� Ʈ���̸� �ۿ��� ���Ͽ� ���� ������ �����ְ� �̰� �����
				m_tEffectDesc.matPivot = dynamic_cast<CCharacter*>(m_pOwner)->Get_Body()->Get_BonePtr(m_tEffectDesc.strBoneTag.c_str())->Get_CombinedTransformationMatrix();
				XMStoreFloat4x4(&m_tEffectDesc.matCombined, m_pTransformCom->Get_WorldMatrix() * m_tEffectDesc.matPivot * m_pOwner->Get_Transform()->Get_WorldFloat4x4());	// �� * ���� * ����	
			}
			else
			{
				// ������ ��Ʈ������ ����ҰŸ� �޾ƿ���
				m_tEffectDesc.matPivot = m_pOwner->Get_Transform()->Get_WorldFloat4x4();
				XMStoreFloat4x4(&m_tEffectDesc.matCombined, m_pTransformCom->Get_WorldMatrix() * m_tEffectDesc.matPivot);
			}

		}
	}


}

void CEffect::ReSet_Effect()
{
	m_tEffectDesc.bFinished = FALSE;
	//m_tEffectDesc.bRender	= FALSE;

	m_tEffectDesc.fSequenceAcc	 = 0.f;
	m_tEffectDesc.fTimeAcc		 = 0.f;
	m_tEffectDesc.fWaitingAcc	 = 0.f;
	m_tEffectDesc.fRemainAcc	 = 0.f;
	m_tEffectDesc.fLifeTimeRatio = 0.f;

}

void CEffect::Init_ReSet_Effect()
{
	ReSet_Effect();
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second)
			dynamic_cast<CEffect_Void*>(Pair.second)->Init_ReSet_Effect();
	}

}



void CEffect::End_Effect()
{
	m_tEffectDesc.bFinished = TRUE;

	if (m_tEffectDesc.bLoop)
	{	
		for (auto& Pair : m_PartObjects)
		{
			if (nullptr != Pair.second)
			{
				dynamic_cast<CEffect_Void*>(Pair.second)->End_Effect();
			}				
		}
		ReSet_Effect();	// ������ ����. ������ �������� �׿����� �ʴ� �̻� �����ʴ´�. (�������ٰ� ���̰� ������ ����Ʈ�Ŵ����� Return_ToPool()ȣ���ϸ� �� ��.
	}
	else
	{
#ifdef _DEBUG
		if (LEVEL_TOOL == static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel()))
		{
			m_tEffectDesc.bActive_Tool = FALSE;
			ReSet_Effect();	//bLoop�� False�� ��, �� �����̸� ����, �ƴϸ� ���̱�
		}
		else
#endif // _DEBUG
		{		
			if (DEAD_AUTO == m_tEffectDesc.eType_Dead)	// ������ �ƴϰ� �ڵ����� �׾���ϴ� ����Ʈ��(��ƼŬ ��) ������ Ÿ���� ������ �� ���̱�.
			{
				EFFECT_MANAGER->Return_ToPool(this);
				//Set_Dead(TRUE);
			}
			else if (DEAD_OWNER == m_tEffectDesc.eType_Dead)	// ������ �ƴ϶� ������ �ܺο��� �������ٸ� ������ Ÿ���� ������ �� ���¸�.
			{
				ReSet_Effect();
			}

		}

	}

}

void CEffect::End_Effect_ForPool()
{
	if (nullptr != m_pTrail)
		m_pTrail->Set_Play(FALSE);

	m_tEffectDesc.bFinished = TRUE;	// ����Ʈ ����

	m_tEffectDesc.bPlay = FALSE;	// ��� ����
	m_bEnable = FALSE;				// ��� ��

	Init_ReSet_Effect();			// ����

	// ������ �����ϸ� ������
	if (nullptr != m_pOwner)
		Delete_Object_Owner();

	m_tEffectDesc.Reset_Pivot();
}


HRESULT CEffect::Ready_Trail(_uint iLevelIndex, string strFileName)
{
	m_pTrail = EFFECT_MANAGER->Ready_Trail(iLevelIndex, LAYER_EFFECT, strFileName, this);

	return S_OK;
}


CGameObject* CEffect::Find_PartObject(const wstring& strPartTag)
{
	auto	iter = m_PartObjects.find(strPartTag);

	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}


HRESULT CEffect::Add_PartObject(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg)
{
	//if (nullptr != Find_PartObject(strPrototypeTag))
	//	return E_FAIL;

	CGameObject* pPartObject = m_pGameInstance->Clone_Prototype(strPrototypeTag, pArg);
	if (nullptr == pPartObject)
		return E_FAIL;
	
	dynamic_cast<CEffect_Void*>(pPartObject)->Set_Object_Owner(this);	// �θ� ����
	m_PartObjects.emplace(strPartTag, pPartObject);
	m_tEffectDesc.iPartSize += 1;

	return S_OK;
}

void CEffect::Delete_PartObject(const wstring& strPartTag)
{
	auto	iter = m_PartObjects.find(strPartTag);

	if (iter == m_PartObjects.end())
		return;


	// ������ ��ü ������ ��� ��ü�� ��ȣ�� �ϳ��� ������ ���
	auto erase_iter = iter;
	++erase_iter; // ������ �׸� ������ ù ��° �׸�

	iter->second->Set_Dead(TRUE);		// ��ü ����
	m_PartObjects.erase(strPartTag);	// �������̳ʿ��� ����


	while (erase_iter != m_PartObjects.end())
	{
		// ��ȣ�� �ϳ� ������ ���
		dynamic_cast<CEffect_Void*>(erase_iter->second)->ReNumber_PartTag();

		++erase_iter;
	}

	m_tEffectDesc.iPartSize -= 1;

}


HRESULT CEffect::Ready_Components()
{


	return S_OK;
}

HRESULT CEffect::Ready_PartObjects(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg)
{
	/* Json�ε��ؼ� ����� ��Ʈ ������Ʈ �غ��ϵ�������. */

	CGameObject* pPartObject = m_pGameInstance->Clone_Prototype(strPrototypeTag, pArg);
	if (nullptr == pPartObject)
		return E_FAIL;

	pPartObject->Set_Object_Owner(this);	// �θ� ����
	m_PartObjects.emplace(strPartTag, pPartObject);

	return S_OK;
}


CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CEffect* pInstance = new CEffect(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect::Pool()
{
	return new CEffect(*this);
}

void CEffect::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);

	m_PartObjects.clear();


	if (nullptr != m_pTrail)
	{
		Safe_Release(m_pTrail);
	}	

}

