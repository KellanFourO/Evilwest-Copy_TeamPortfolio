#include "Mother_VomitLoop.h"
#include "Mother_VomitEnd.h"

void CMother_VomitLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_VomitLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//���� �ð��� �������� End�� ��ȯ 
	m_fTimeDelta += fTimeDelta;
	if (m_fTimeDelta >= 7.f)
	{
		//m_fTimeDelta = 0.0f;
		return new CMother_VomitEnd;
	}

	return nullptr;
}

void CMother_VomitLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}
