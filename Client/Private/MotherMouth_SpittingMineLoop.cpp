#include "MotherMouth_SpittingMineLoop.h"
#include "MotherMouth_SpittingMineEnd.h"

void CMotherMouth_SpittingMineLoop::Initialize(CMotherMouth* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMotherMouth>* CMotherMouth_SpittingMineLoop::Update(CMotherMouth* pActor, _float fTimeDelta)
{
	//��������� ���� ���� ��� ������ �������� ���� �����ϰ� ���� �����ð��̳��� �������� ���ϸ� �������Ͱ� ������ �Ǹ鼭 Ƣ�� ���;� �� 

	return nullptr;
}

void CMotherMouth_SpittingMineLoop::Release(CMotherMouth* pActor)
{
	__super::Release(pActor);
}
