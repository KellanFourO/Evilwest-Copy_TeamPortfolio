#include "Mother_Egg.h"
#include "GameInstance.h"
#include "Character.h"
#include "Data_Manager.h"
#include "Player.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "Mother.h"
#include "Bone.h"

CMother_Egg::CMother_Egg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CProjectile(pDevice, pContext, strPrototypeTag)
{
}

CMother_Egg::CMother_Egg(const CMother_Egg& rhs)
	: CProjectile(rhs)
{
}

HRESULT CMother_Egg::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMother_Egg::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

	GameObjectDesc.fSpeedPerSec = 12.f;
	GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	m_pMother = CData_Manager::GetInstance()->Get_Mother();

	_float4x4 BoneMatrix = m_pMother->Get_Body()->Get_BonePtr("Jaws_Center")->Get_CombinedTransformationMatrix();
	_float4x4 MotherMatrix = m_pMother->Get_Transform()->Get_WorldMatrix();
	_float4x4 Temp = BoneMatrix * MotherMatrix;

	m_pTransformCom->Set_WorldMatrix(Temp);

	m_vPlayerPos = CData_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Look_At(m_vPlayerPos);

	m_fDamage = 10.f;

	// ����Ʈ ����
	//m_pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_INTRO_BOSS, LAYER_EFFECT, "Test_Skull_04.json", this);


	return S_OK;
}

void CMother_Egg::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMother_Egg::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//�����Ǵ� ��ġ���� �׳� �չ������� ���� 
	if(m_pTransformCom->Get_Position().y >=0.f)
		m_pTransformCom->Go_Straight(fTimeDelta);


}

void CMother_Egg::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CMother_Egg::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMother_Egg::Render_Shadow()
{
	//�̰� �ʿ��Ѱ�? ���� �ʿ�� ���� ���� 
	return S_OK;
}

void CMother_Egg::OnCollisionEnter(CCollider* other)
{

	CCharacter* pTarget_Character = Get_Target_Character(other);

	if (nullptr != pTarget_Character)// �Ϲ� Ÿ�� 
	{
		pTarget_Character->Set_Hitted(m_fDamage, pTarget_Character->Calc_Look_Dir_XZ(m_pTransformCom->Get_Position()), m_fForce, 1.f, m_eHitDirection, m_eHitPower);

		CEffect* pEffect = EFFECT_MANAGER->Create_Effect(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, "Test_Effect.json");
		_float3 vPos = m_pTransformCom->Get_Position();
		pEffect->Set_Position(vPos);

	}
	//m_pCollider->Set_Enable(false);
	//this->Set_Dead(true);
	//m_pEffect->Set_Dead(true);	// ����Ʈ ���̱�
}

void CMother_Egg::OnCollisionStay(CCollider* other)
{
}

void CMother_Egg::OnCollisionExit(CCollider* other)
{
}

HRESULT CMother_Egg::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Model_Mother_Egg"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	///* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER_ATTACK);
	BoundingDesc.fRadius = { 1.f };
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

CMother_Egg* CMother_Egg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CMother_Egg* pInstance = new CMother_Egg(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMother_Egg");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMother_Egg::Clone(void* pArg)
{
	CMother_Egg* pInstance = new CMother_Egg(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMother_Egg");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMother_Egg::Pool()
{
	return new CMother_Egg(*this);
}

void CMother_Egg::Free()
{
	__super::Free();

	//if(nullptr != m_pEffect)
	//	Safe_Release(m_pEffect);

}