#include "VampireCommander_SyncedAttack.h"
#include "VampireCommander_Idle.h"

void CVampireCommander_SyncedAttack::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	//���⼭ Fail���� �б����� Trackposition �޾Ƽ� �����;� �� 
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true,true,0);

	
}

CState<CVampireCommander>* CVampireCommander_SyncedAttack::Update(CVampireCommander* pActor, _float fTimeDelta)
{

	//  �̽�ų�� �÷��̾ ��ƴٰ� ���� ������ �Ŷ� �÷��̾�� �����Ǿ�� ��! 
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_Idle();
	}


	return nullptr;
}

void CVampireCommander_SyncedAttack::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}