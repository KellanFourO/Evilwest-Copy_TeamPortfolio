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
#include "Bone.h"
#include "Effect.h"
#include "Clone_Manager.h"

#include "Effect_Manager.h"
#include "Effect_Trail.h"

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

	//! ����: Ʈ���� �׽�Ʈ
	if (nullptr != m_pTrail)
	{
		m_pTrail->Tick_Trail(fTimeDelta, m_WorldMatrix);
		cout << "VampireCommander_Hand_PositionX:" << m_WorldMatrix._41 << "VampireCommander_Hand_PositionY:" << m_WorldMatrix._42 << "VampireCommander_Hand_PositionZ:" << m_WorldMatrix._43 << endl;
		cout << "TrailX:" << m_pTrail->Get_Position().x << "TrailY:" << m_pTrail->Get_Position().y << "TrailZ:" << m_pTrail->Get_Position().z << endl;
	}
	
	// m_pTrail->Set_Play(TRUE /*FALSE*/); //! ����: Ʈ���� �¿���

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


	//! ����: Ʈ���� �׽�Ʈ
	m_pTrail = EFFECT_MANAGER->Ready_Trail(iNextLevel, LAYER_EFFECT, "Test_Trail.json",this);

	return S_OK;
}

void CVampireCommander_Weapon_Hand::OnCollisionEnter(CCollider* other)
{

	CCharacter* pTarget_Character = Get_Target_Character(other);
	CVampireCommander* parent = dynamic_cast<CVampireCommander*>(Get_Object_Owner());
	if (nullptr != pTarget_Character && m_bSynced == false)// �Ϲ� Ÿ�� 
	{
		_vector vTargetPos = pTarget_Character->Get_Position_Vector();
		pTarget_Character->Set_Hitted(m_iDamage, Get_Object_Owner()->Calc_Look_Dir(vTargetPos) * -1, m_fForce, 1.f, m_eHitDirection, m_eHitPower);
		//pTarget_Character->Set_Hitted(0, Get_Object_Owner()->Calc_Look_Dir(vTargetPos) * -1, 0.5f, 1.f, Direction::Front, Power::Light);
 		string Test = "Data_Animation/";
		parent->Set_EventNotify(Test, "Test2_AnimationData.json");
		CEffect* pEffect = EFFECT_MANAGER->Create_Effect(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, parent->Get_CharcterDesc().EffectFileName + ".json");
		_float3 vPos =this->Get_WorldPosition();
		pEffect->Set_Position(vPos);

	}
	else if(nullptr != pTarget_Character && m_bSynced == true)//��� �ɷ����� 
	{
		parent->Get_Actor()->Set_State(new CVampireCommander_SyncedAttack);
		CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();
		pPlayer->Get_Actor()->Set_State(new CPlayer_VampireCommander_SyncedAttack);
		pPlayer->Set_Rotate_In_CameraDir(false);
		pPlayer->m_bPlayerCheck =false;
		CCamera* pCam;
		pCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_vectorCamera()[1];
		CSpringCamera* pSpringCam = dynamic_cast<CSpringCamera*>(pCam);
		_float4x4 BoneMatrix = {};
		BoneMatrix = parent->Get_Body()->Get_BonePtr("Neck")->Get_CombinedTransformationMatrix();
		_float4x4 pMonsterPos = parent->Get_Transform()->Get_WorldMatrix();
		_float4x4 temp = {};
		XMStoreFloat4x4(&temp, BoneMatrix * pMonsterPos);
		_float3 TargetPosition = {};
		TargetPosition.x = temp._41;
		TargetPosition.y = temp._42;
		TargetPosition.z = temp._43;
		pSpringCam->Set_pTarget(parent->Get_Transform());
		pSpringCam->Set_TargetPosition(TargetPosition);
		pSpringCam->Set_pTargetCharacter(parent);
		pSpringCam->Set_CameraOffset(_float3(1.7f, -3.f, -8.5f));
		pPlayer->Get_Transform()->Set_State(CTransform::STATE_POSITION, parent->Get_Transform()->Get_State(CTransform::STATE_POSITION) +2*parent->Get_Transform()->Get_State(CTransform::STATE_LOOK));

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
	m_pTrail->Set_Dead(true);
}
