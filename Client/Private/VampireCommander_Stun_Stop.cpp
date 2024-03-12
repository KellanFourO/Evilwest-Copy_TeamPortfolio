#include "VampireCommander_Stun_Stop.h"

void CVampireCommander_Stun_Stop::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CVampireCommander>* CVampireCommander_Stun_Stop::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	//���࿡ Stun ������ ������ ���� ���ϸ� ���� Ÿ�°Ű� �ƴ϶�� cut scene���� ó�� �Ұ���
	//���� �� �� ������ ������ �̰� 
	return nullptr;
}

void CVampireCommander_Stun_Stop::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}
