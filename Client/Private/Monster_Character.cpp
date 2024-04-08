#include "Monster_Character.h"
#include "Transform.h"
#include "Navigation.h"
#include "Player.h"
#include "Data_Manager.h"
#include "Transform.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_EnemyHUD_Shard.h"

CMonster_Character::CMonster_Character(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CCharacter_Client(pDevice, pContext, strPrototypeTag)
{
}

CMonster_Character::CMonster_Character(const CMonster_Character& rhs)
	: CCharacter_Client(rhs)
{
}

HRESULT CMonster_Character::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CMonster_Character::Initialize(void* pArg)
{
	FAILED_CHECK(__super::Initialize(pArg));

	_bool bMonsterDesc = false;


	if (pArg != nullptr)
	{
		CGameObject::GAMEOBJECT_DESC ObjectDesc = *(CGameObject::GAMEOBJECT_DESC*)pArg;
		
		if(ObjectDesc.eDescType == CGameObject::MONSTER_DESC)
			bMonsterDesc = true;


		if (bMonsterDesc == true)
		{
			m_tMonsterDesc = *(MONSTER_DESC*)pArg;

			if (m_tMonsterDesc.bPreview == false)
			{
				m_pTransformCom->Set_WorldMatrix(m_tMonsterDesc.WorldMatrix);
				m_pNavigationCom->Set_CurrentIndex(m_tMonsterDesc.iStartNaviIndex);
				//m_pNavigationCom->Find_NaviIndex(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				//m_pNavigationCom->Set_CurrentIndex(m_pNavigationCom->Get_SelectRangeCellIndex(this));
			}
		}
	}

	

	return S_OK;
}

void CMonster_Character::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMonster_Character::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CMonster_Character::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (true == m_bDead || false == m_bEnable)
	{
		Set_EnemyHUD_Dead();
	}
}

HRESULT CMonster_Character::Render()
{
	FAILED_CHECK(__super::Render());

	return S_OK;
}


void CMonster_Character::Search_Target(const _float fSearchDistance)
{
	__super::Search_Target(LAYER_PLAYER, fSearchDistance);
}

CPlayer* CMonster_Character::Set_Player_Finisher_Pos(_float3 vPlayerPos)
{
	m_pBody->Collider_Off();

	CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();
	_float3 vPos = m_pTransformCom->Calc_Front_Pos(vPlayerPos);
	pPlayer->Set_Position(vPos);
	pPlayer->Get_Transform()->Look_At(m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION));

	return pPlayer;
}

CPlayer* CMonster_Character::Set_Finish_Pos(_float3 vPos)
{
	m_pBody->Collider_Off();

	CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();

	_float3 vTargetPos = pPlayer->Calc_Front_Pos(vPos);
	Set_Position(vTargetPos);
	m_pTransformCom->Look_At(pPlayer->Get_Transform()->Get_State(CTransform::STATE::STATE_POSITION));

	return pPlayer;
}

void CMonster_Character::Check_Frustum()
{
	m_bIsInFrustum = m_pGameInstance->isIn_WorldPlanes(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f);
}

// ���� HUD�� �غ��մϴ�. (���� : Level, Owner)
void CMonster_Character::Ready_EnemyHUD_Shard(_uint iLevelIndex, CGameObject* pOwner)
{
	m_pEnemyHUD = m_pUIManager->Ready_EnemyHUD_Shard(iLevelIndex, pOwner);
}

// ���� HUD�� ��ġ�� ����ݴϴ�. (�� ƽ���� �ҷ��ּ���. | ������ World��Ʈ������ �־��ּ���. | Offset��ŭ ��ġ�� ������ �� �ֽ��ϴ�. [Defualt (������ ������ ���) : 0, 2, 0]
void CMonster_Character::Check_EnemyHUD_World(_matrix matWorld, _float3 vOffsetPos)
{
	if (m_pEnemyHUD != nullptr)
		m_pEnemyHUD->Set_EnemyHUD_World(matWorld, vOffsetPos);
}

// ���� HUD�� ����(��Ȱ��ȭ)�մϴ�. (���Ͱ� ������ �ҷ��ּ���)
void CMonster_Character::Set_EnemyHUD_Dead()
{
	if (m_pEnemyHUD != nullptr) 
	{
		m_pEnemyHUD->Set_Dead_Owner(true);
		m_pEnemyHUD = nullptr;
	}
		
}

void CMonster_Character::Free()
{
	__super::Free();
}
