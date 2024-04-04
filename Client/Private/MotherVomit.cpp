#include "MotherVomit.h"
#include "GameInstance.h"
#include "Character.h"
#include "Data_Manager.h"
#include "Player.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "Mother.h"
#include "Bone.h"
#include "MasterCamera.h"

CMotherVomit::CMotherVomit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CProjectile(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = true;
}

CMotherVomit::CMotherVomit(const CMotherVomit& rhs)
	: CProjectile(rhs)
{
}

HRESULT CMotherVomit::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMotherVomit::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

	GameObjectDesc.fSpeedPerSec = 20.f;
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

	m_fDamage = 1.f;

	Set_Enable(true);
	// ����Ʈ ����
	//m_pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_INTRO_BOSS, LAYER_EFFECT, "Test_Skull_04.json", this);


	return S_OK;
}

void CMotherVomit::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMotherVomit::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//�����Ǵ� ��ġ���� �׳� �չ������� ���� 
	//if (m_pTransformCom->Get_Position().y >= 0.f)
	m_pTransformCom->Go_Straight(fTimeDelta);
	if (m_pTransformCom->Get_Position().y <= -5.f)
		Set_Enable(false);
	//�÷��̾�� ������ �극���� ������ �ȵɰ� ���⵵ �ϰ� 

}

void CMotherVomit::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CMotherVomit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMotherVomit::Render_Shadow()
{
	//�̰� �ʿ��Ѱ�? ���� �ʿ�� ���� ���� 
	return S_OK;
}

void CMotherVomit::OnCollisionEnter(CCollider* other)
{
	//�浹 ������ ī�޶� ����ŷ ����� �� ! 
	CSpringCamera* pSpringCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_SpringCamera();
	pSpringCam->Set_ShakeCameraTime(0.2f);
	pSpringCam->Set_ShakeCameraMinMax(_float2(0.f, 0.1f));

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

void CMotherVomit::OnCollisionStay(CCollider* other)
{

}

void CMotherVomit::OnCollisionExit(CCollider* other)
{
}

HRESULT CMotherVomit::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Model_MotherVomit"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	///* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER_ATTACK);
	BoundingDesc.fRadius = { 100.f };
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

CMotherVomit* CMotherVomit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CMotherVomit* pInstance = new CMotherVomit(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMotherVomit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMotherVomit::Clone(void* pArg)
{
	CMotherVomit* pInstance = new CMotherVomit(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMotherVomit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMotherVomit::Pool()
{
	return new CMotherVomit(*this);
}

void CMotherVomit::Free()
{
	__super::Free();

	//if(nullptr != m_pEffect)
	//	Safe_Release(m_pEffect);

}