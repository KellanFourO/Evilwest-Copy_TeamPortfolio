#include "stdafx.h"
#include "VampireCommander_Weapon_Hand.h"
#include "GameInstance.h"
#include "Character.h"
#include "VampireCommander_SyncedAttack.h"
#include "Player.h"
#include "Player_VampireCommander_SyncedAttack.h"
#include "Data_Manager.h"
#include "MasterCamera.h"
#include "Transform.h"

CVampireCommander_Weapon_Hand::CVampireCommander_Weapon_Hand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CVampireCommander_Weapon(pDevice, pContext, strPrototypeTag)
{

}

CVampireCommander_Weapon_Hand::CVampireCommander_Weapon_Hand(const CVampireCommander_Weapon_Hand& rhs)
	: CVampireCommander_Weapon(rhs)
{
}

HRESULT CVampireCommander_Weapon_Hand::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVampireCommander_Weapon_Hand::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVampireCommander_Weapon_Hand::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CVampireCommander_Weapon_Hand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CVampireCommander_Weapon_Hand::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CVampireCommander_Weapon_Hand::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVampireCommander_Weapon_Hand::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	//if (FAILED(__super::Ready_Components()))
	//	return E_FAIL;

	m_iColliderSize = 1;
	m_pColliders.resize(m_iColliderSize);

	///* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER_ATTACK);
	BoundingDesc.fRadius = { 0.8f };
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliders[0]), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

void CVampireCommander_Weapon_Hand::OnCollisionEnter(CCollider* other)
{
	CCharacter* pTarget_Character = Get_Target_Character(other);
	if (nullptr != pTarget_Character && m_bSynced == false)
	{
		_vector vTargetPos = pTarget_Character->Get_Position_Vector();
		pTarget_Character->Set_Hitted(m_iDamage, Get_Object_Owner()->Calc_Look_Dir(vTargetPos) * -1, m_fForce, 1.f, m_eHitDirection, m_eHitPower);
		//pTarget_Character->Set_Hitted(0, Get_Object_Owner()->Calc_Look_Dir(vTargetPos) * -1, 0.5f, 1.f, Direction::Front, Power::Light);

	}
	else if(nullptr != pTarget_Character && m_bSynced == true)
	{
		CVampireCommander* parent = dynamic_cast<CVampireCommander*>(Get_Object_Owner());
		parent->Get_Actor()->Set_State(new CVampireCommander_SyncedAttack);
		CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();
		pPlayer->Get_Actor()->Set_State(new CPlayer_VampireCommander_SyncedAttack);
		
		CCamera* pCam;
		pCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_vectorCamera()[1];
		CSpringCamera* pSpringCam = dynamic_cast<CSpringCamera*>(pCam);
		pSpringCam->Set_CameraOffset(_float3(2, 1, -5));

		//CData_Manager::GetInstance()->Get_MasterCamera()->Get_vectorCamera()[0]->Set_Position(pPlayer->Get_Position() + _float3(-1.f, 1.5f, 2.f));
		//CData_Manager::GetInstance()->Get_MasterCamera()->Get_vectorCamera()[0]->Get_Transform()->Look_At(pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION));
		//CData_Manager::GetInstance()->Get_MasterCamera()->Set_CameraType(CMasterCamera::DynamicCamera);	


	}
	Set_Enable(false);
}

void CVampireCommander_Weapon_Hand::OnCollisionStay(CCollider* other)
{
	/*_int a = 0;*/
}

void CVampireCommander_Weapon_Hand::OnCollisionExit(CCollider* other)
{
	/*_int a = 0;*/

}

CVampireCommander_Weapon_Hand* CVampireCommander_Weapon_Hand::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CVampireCommander_Weapon_Hand* pInstance = new CVampireCommander_Weapon_Hand(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVampireCommander_Weapon_Hand");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CVampireCommander_Weapon_Hand::Clone(void* pArg)
{
	CVampireCommander_Weapon_Hand* pInstance = new CVampireCommander_Weapon_Hand(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVampireCommander_Weapon_Hand");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CVampireCommander_Weapon_Hand::Pool()
{
	return new CVampireCommander_Weapon_Hand(*this);
}

void CVampireCommander_Weapon_Hand::Free()
{
	__super::Free();
}
