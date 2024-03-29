#include "Mother_ShakeTreeLoop.h"
#include "Mother_ShakeTreeEnd.h"

void CMother_ShakeTreeLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CMother>* CMother_ShakeTreeLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//������ �ð��� ������ ���� ���� End ��  ��ȯ ! 3������ �浹�ϸ� �ɰ� ����� �� �븻�� �س��� ī��Ʈ �ٿ����� �ѹ� �ִϸ��̼� �ٵ��� ���� �ѹ��� ��������
	//--����  �̷������� üũ�ؼ� �� ���ڰ� 0�� �Ǿ����� ������ ����� End�� ��ȯ ! 
	if (pActor->Is_Animation_End())
	{
		return new CMother_ShakeTreeEnd;
	}
	return nullptr;
}

void CMother_ShakeTreeLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}
