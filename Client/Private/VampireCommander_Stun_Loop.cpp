#include "VampireCommander_Stun_Loop.h"

void CVampireCommander_Stun_Loop::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CVampireCommander>* CVampireCommander_Stun_Loop::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	//�������� ������ ������ ���� ���¿��� ������ Stun_Stop���� ���� ��ų ���� �÷��̾��ʿ��� �̰� ���̰ų� ���������� �����ų� �ϴ� �� �־�־� ��
	return nullptr;
}

void CVampireCommander_Stun_Loop::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}
