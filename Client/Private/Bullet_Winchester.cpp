#include "Bullet_Winchester.h"
#include "GameInstance.h"
#include "Character.h"
#include "Data_Manager.h"
#include "Effect.h"
#include "Effect_Manager.h"

CBullet_Winchester::CBullet_Winchester(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CProjectile(pDevice, pContext, strPrototypeTag)
{
}

CBullet_Winchester::CBullet_Winchester(const CBullet_Winchester& rhs)
	: CProjectile(rhs)
{
}

HRESULT CBullet_Winchester::Initialize_Prototype()
{

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_Winchester::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

	GameObjectDesc.fSpeedPerSec = 30.f;
	GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	

	//m_pTransformCom->Look_At(m_vPlayerPos);

	m_iDamage = 10;

	// 이펙트 생성
	//m_pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_INTRO_BOSS, LAYER_EFFECT, "Test_Skull.json", this);

	return S_OK;
}

void CBullet_Winchester::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBullet_Winchester::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//생성되는 위치에서 그냥 앞방향으로 ㄱㄱ 
	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CBullet_Winchester::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CBullet_Winchester::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_Winchester::Render_Shadow()
{
	//이거 필요한거? 딱히 필요는 없어 보임 
	return S_OK;
}

void CBullet_Winchester::OnCollisionEnter(CCollider* other)
{

	CCharacter* pTarget_Character = Get_Target_Character(other);

	if (nullptr != pTarget_Character)// 일반 타격 
	{
		m_eHitDirection = Direction::Front;
		m_eHitPower = Power::Medium;
		m_fForce = 0.f;

		//_vector vDir = pTarget_Character->Calc_Look_Dir(m_pTransformCom->Get_Position());
		//pTarget_Character->Set_Hitted(m_iDamage, vDir, m_fForce, 1.f, m_eHitDirection, m_eHitPower);

		CEffect* pEffect = EFFECT_MANAGER->Create_Effect(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, "Test_Effect.json");
		_float3 vPos = m_pTransformCom->Get_Position();
		pEffect->Set_Position(vPos);
	}

	Set_Dead(true);


	//m_pEffect->Set_Dead(true);
}

void CBullet_Winchester::OnCollisionStay(CCollider* other)
{
}

void CBullet_Winchester::OnCollisionExit(CCollider* other)
{
}

HRESULT CBullet_Winchester::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	///* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::PLAYER_ATTACK);
	BoundingDesc.fRadius = { 0.3f };
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

CBullet_Winchester* CBullet_Winchester::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CBullet_Winchester* pInstance = new CBullet_Winchester(pDevice, pContext, strPrototypeTag);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Winchester");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_Winchester::Clone(void* pArg)
{
	CBullet_Winchester* pInstance = new CBullet_Winchester(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Winchester");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_Winchester::Pool()
{
	return new CBullet_Winchester(*this);
}

void CBullet_Winchester::Free()
{
	__super::Free();
	if (m_pEffect != nullptr)
		Safe_Release(m_pEffect);
}
