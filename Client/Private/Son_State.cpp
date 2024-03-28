#include "Son_State.h"
#include "GameInstance.h"
#include "Mother.h"

#include "Son_ShortAttack.h"
#include "Son_LongAttack.h"
#include "Son_CrossAttack.h"
#include "Son_RangeAttackFar.h"


void CSon_State::Initialize(CSon* pActor)
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

CState<CSon>* CSon_State::Update(CSon* pActor, _float fTimeDelta)
{
	return nullptr;
}

void CSon_State::Release(CSon* pActor)
{
	Safe_Release(m_pGameInstance);
}

CState<CSon>* CSon_State::Update_State(CSon* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CSon>* CSon_State::Normal_State(CSon* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return Normal(pActor,fTimeDelta,_iAnimIndex);
}

CState<CSon>* CSon_State::Attack_State(CSon* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (pActor->m_pMother->m_bPhase == true)//TODO������ 1 ���� ���� 2�� ���Ÿ��� �ֿ� �þ�� ���� 
	{
		if (20.f > pActor->Calc_Distance() && 0.f < pActor->Calc_Distance())
		{
			pActor->m_bLookAt = true;
			return new CSon_ShortAttack;
		}
		else if (50.f > pActor->Calc_Distance() && 2.f <= pActor->Calc_Distance())
		{
			pActor->m_bLookAt = true;
			return new CSon_LongAttack;
		}
		else if (50.f <= pActor->Calc_Distance())
		{
			pActor->m_bLookAt = true;
			return new CSon_RangeAttackFar;
		}
	}
	else if (pActor->m_pMother->m_bPhase == false)//TODO ������ 2,3 ���� ���� 3�� ���Ÿ� 1�� ���� Ÿ�� 2�� 
	{

	}

	return nullptr;
}

CState<CSon>* CSon_State::Normal(CSon* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CSon>* pState = { nullptr };

	if (100.f > pActor->Calc_Distance() && 0.f < pActor->Calc_Distance())
	{
		pActor->m_bLookAt = true;
		pState = Attack_State(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}

	return nullptr;
}
