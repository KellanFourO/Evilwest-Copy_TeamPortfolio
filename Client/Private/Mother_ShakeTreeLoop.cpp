#include "Mother_ShakeTreeLoop.h"
#include "Mother_ShakeTreeEnd.h"
#include "MasterCamera.h"
#include "Data_Manager.h"

void CMother_ShakeTreeLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_ShakeTreeLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//������ �ð��� ������ ���� ���� End ��  ��ȯ ! 5������ �浹�ϸ� �ɰ� ����� �� �븻�� �س��� ī��Ʈ �ٿ����� �ѹ� �ִϸ��̼� �ٵ��� ���� �ѹ��� ��������
	//--����  �̷������� üũ�ؼ� �� ���ڰ� 0�� �Ǿ����� ������ ����� End�� ��ȯ ! 
	m_fTimeDelta += fTimeDelta;
	if (m_bFlags[0] == false && pActor->Is_Inputable_Front(37))
	{
		CSpringCamera* pSpringCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_SpringCamera();
		pSpringCam->Set_ShakeCameraMinMax(_float2(0.f, 0.2f));
		pSpringCam->Set_ShakeCameraTime(0.3f);
		pActor->Apply_Shake_And_Blur(Power::Medium);
		//�̋� ī�޶� ����ŷ �ϸ鼭 �ʿ� ��ü ���� ���� �߰��ϸ� �� �Ű��� 
		m_bFlags[0] = true;
	}

	if(pActor->Is_Inputable_Front(41))
		m_bFlags[0] = false;

	if (m_fTimeDelta >= 10.f && pActor->Is_Inputable_Front(41))
	{
		m_fTimeDelta = 0.f;
		return new CMother_ShakeTreeEnd;
	}
	return nullptr;
}

void CMother_ShakeTreeLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
	CSpringCamera* pSpringCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_SpringCamera();
	pSpringCam->Set_ShakeCameraMinMax(_float2(0.3f, 0.3f));

	pSpringCam->Set_ShakeCameraTime(0.2f);
}
