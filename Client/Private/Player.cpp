#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "Weapon_Player.h"
#include "Player_IdleLoop.h"
#include "Data_Manager.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "SMath.h"

// Add_UIManager
#include "UI_Manager.h"

#include "Player_HitNormal_B.h"
#include "Player_HitNormal_F.h"
#include "Player_HitNormal_L.h"
#include "Player_HitNormal_R.h"
#include "Player_HitPeriodic_01.h"
#include "Player_HitPeriodic_03.h"
#include "Player_HitPeriodic_04.h"
#include "Player_KnockFrontLight_F_02.h"
#include "Player_HitHeavy_F_5m.h"

#include "Player_DeathLight_F_01.h"
#include "Player_DeathLight_F_02.h"
#include "Player_DeathNormal_F_01.h"
#include "Player_DeathNormal_F_02.h"


#include "Player_InteractionJumpDown100.h"
#include "Player_InteractionJumpDown200.h"
#include "Player_InteractionJumpDown300.h"

#include "Player_InteractionVault100.h"
#include "Player_InteractionVault200.h"

#include "Player_CartRide_Start.h"
#include "Player_L06_Wagon_Jump.h"
#include "Player_CartRide_Loop.h"

#include "Player_InteractionPull_Rock_Idle.h"
#include "Player_InteractionPush_Rock_Idle.h"

#include "Player_InteractionPush_Rock_End.h"
#include "Player_InteractionPull_Rock_End.h"



#include "Player_InteractionClimb100.h"
#include "Player_InteractionClimb200.h"
#include "Player_InteractionClimb300.h"
#include "Player_InteractionClimb450.h"

#include "Player_Interaction_Slide_Sequence.h"
#include "Player_Interaction_SmallLever.h"

#include "Player_InteractionPlank_Up_Start.h"
#include "Player_InteractionLadder_Up_Start.h"

#include "Player_InteractionWhipSwing.h"
#include "Player_InteractionWhipPull.h"

#include "Player_InteractionRotateValve_01.h"

#include "Player_InteractionDoor_Open.h"
#include "Player_InteractionDoorPush.h"

#include "Player_InteractionClimbRope_Start.h"
#include "Player_InteractionRope_Down_Start.h"
#include "Player_ZipLine_Start.h"
#include "Player_CrouchUnder_Start.h"
#include "Player_CrouchUnder_Gate.h"

#include "Environment_Interact.h"
#include "Player_ZipLine_Loop.h"

#include "PhysXCharacterController.h"
#include "PhysXCollider.h"
#include "Preset_PhysXColliderDesc.h"


#include "Effect.h"
#include "Effect_Manager.h"


//!For Debug
#include "Level_Loading.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CCharacter_Client(pDevice, pContext, strPrototypeTag)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CCharacter_Client(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

	//GameObjectDesc.fSpeedPerSec = 7.f;
	GameObjectDesc.fSpeedPerSec = 7.f;
	GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	FAILED_CHECK(__super::Initialize(&GameObjectDesc));

	m_fHp = 100;
	m_fMaxHp = m_fHp;



// 	if (m_pGameInstance->Get_NextLevel() != ECast(LEVEL::LEVEL_TOOL))
// 	{
		m_pActor = new CActor<CPlayer>(this);
		m_pActor->Set_State(new CPlayer_IdleLoop());
		//m_pActor->Set_State(new CPlayer_CartRide_Loop());

// 	}


	m_pRigidBody->Set_UseGravity(true);

	//_uint iNextLevel = m_pGameInstance->Get_NextLevel();


	CData_Manager::GetInstance()->Set_Player(this);
	m_pGameInstance->Set_Player(this);

	Set_HUD_MaxCooltime(HUD::LEFT_TOP,		SUPER_CHARGE_MAXCOOLTIME);		//��������
	Set_HUD_MaxCooltime(HUD::LEFT_RIGHT,	HEAL_MAXCOOLTIME);		//��
	Set_HUD_MaxCooltime(HUD::LEFT_BOTTOM,	REVOLVER_MAXCOOLTIME);		//������
	Set_HUD_MaxCooltime(HUD::LEFT_LEFT,		SHOTGUN_MAXCOOLTIME);		//����

	Set_HUD_MaxCooltime(HUD::RIGHT_TOP,		RIFLE_MAXCOOLTIME);	//������
	Set_HUD_MaxCooltime(HUD::RIGHT_RIGHT,	SLAM_MAXCOOLTIME);	//�������
	Set_HUD_MaxCooltime(HUD::RIGHT_BOTTOM,	KICK_MAXCOOLTIME);	//������
	Set_HUD_MaxCooltime(HUD::RIGHT_LEFT,	WHIP_MAXCOOLTIME);	//���� ��

	//m_pUIManager->Change_LeftHUD_MaxCoolTime("LeftHUD_Top", 5.f);
	//m_pUIManager->Change_LeftHUD_MaxCoolTime("LeftHUD_Right", 5.f);
	//m_pUIManager->Change_LeftHUD_MaxCoolTime("LeftHUD_Bottom", 5.f);
	//m_pUIManager->Change_LeftHUD_MaxCoolTime("LeftHUD_Left", 5.f);

	//m_pUIManager->Change_RightHUD_MaxCoolTime("RightHUD_Top", 5.f);
	//m_pUIManager->Change_RightHUD_MaxCoolTime("RightHUD_Right", 5.f);
	//m_pUIManager->Change_RightHUD_MaxCoolTime("RightHUD_Bottom", 5.f);
	//m_pUIManager->Change_RightHUD_MaxCoolTime("RightHUD_Left", 5.f);
	

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{
	//m_pGameInstance->Set_Player(this);

	__super::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	//!if (m_pGameInstance->Key_Down(DIK_H))
	//!{
	//!	if (GAME_STATE::UI == m_pDataManager->Get_GameState())
	//!	{
	//!		m_pDataManager->Set_GameState(GAME_STATE::GAMEPLAY);
	//!	}
	//!	else if (GAME_STATE::GAMEPLAY == m_pDataManager->Get_GameState())
	//!	{
	//!		m_pDataManager->Set_GameState(GAME_STATE::UI);
	//!	}
	//!}
	
	//! ���� �׽�Ʈ ���� ����
	if (m_bfirstcheck)
	{
		//EFFECT_MANAGER->Play_Effect("Parasiter/MotherShakeTree/", "Circle_Floor_05.json", this, _float3(m_pTransformCom->Get_Position().x, 0.1f, m_pTransformCom->Get_Position().z));
		//EFFECT_MANAGER->Play_Effect("Parasiter/MotherShakeTree/", "Circle_Floor_05.json", nullptr, _float3(m_pTransformCom->Get_Position().x + 2.f, 0.1f, m_pTransformCom->Get_Position().z + 2.f));
		
		//EFFECT_MANAGER->Play_Effect("Player/TeleportPunch/", "TeleportPunch_01.json", this, true, "Head");
		m_bfirstcheck = false;
	}

	//if (m_pGameInstance->Key_Down(DIK_GRAVE))
	//{
	//	_float3 vPos = Get_Position();
	//	EFFECT_MANAGER->Play_Effect("Hit/", "Hit_8hit.json", nullptr, Calc_Front_Pos(_float3(0.f, 0.f, 1.f)), true, Get_Position());

	//	// �ڷ���Ʈ ��ġ �ǰ� ����Ʈ ����
	//	//EFFECT_MANAGER->Play_Effect("Hit/", "Hit_TeleportPunch.json", this, Calc_Front_Pos(_float3(0.f, 0.f, 1.2f)), true, Get_Position());	//pActor->Calc_Front_Pos() // �÷��̾� �� ��ġ ���
	//}
	//! ���� �׽�Ʈ ���� ��

	if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
	{
		m_pActor->Set_State(new CPlayer_IdleLoop());
	}

	/* �����߰� : UIâ ����,�״� �ϴ� Key (�ɼ�â, ��ųâ ���) => GamePlay���µ� UI���µ� �Է��� �����ؾ��ؼ� �ۿ� ����. */
	if (m_pGameInstance->Get_NextLevel() != LEVEL::LEVEL_TOOL)
		KeyInput(fTimeDelta);

	if (GAME_STATE::GAMEPLAY != m_pDataManager->Get_GameState())
		return;

	__super::Tick(fTimeDelta);


	if (m_pActor)
	{
		m_pActor->Update_State(fTimeDelta);
	}
		
	Update_ChargingTime(fTimeDelta);

	Update_SuperCharge(fTimeDelta);

	Update_Voice_Cooltime(fTimeDelta);

	m_pDataManager->Set_CurHP(m_fHp);

	if (m_pGameInstance->Key_Down(DIK_T))
	{
		Teleport();
	}


	//if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
	//{
	//	m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_INTRO_BOSS));
	//}


	_bool bIsNotIdle = (m_pBody->Get_CurrentAnimIndex() != ECast(Player_State::Player_IdleLoop) || (true == Is_Splitted()));
	
	if(m_pDataManager->Get_GameState() == GAME_STATE::GAMEPLAY)
		m_pDataManager->Set_ShowInterface(bIsNotIdle);
	

	if (m_pNavigationCom != nullptr)
		m_pNavigationCom->Update(XMMatrixIdentity());

}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}
	
HRESULT CPlayer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Set_Navigation(CNavigation* pNavigation)
{
	if(m_pNavigationCom != nullptr)
		Safe_Release(m_pNavigationCom);

	m_pNavigationCom = pNavigation;
	m_pNavigationCom->Set_CurrentIndex(m_pNavigationCom->Find_CurrentCellIndex(m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION)));
	//Set_InitPosition(m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION));
	
	Safe_AddRef(pNavigation);
	
}

void CPlayer::Aim_Walk(_float fTimeDelta)
{
	_uint AnimIndex;
	//_uint ReversedAnimIndex;


	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		//if (m_pGameInstance->Key_Pressing(DIK_A))
		//{
		//	AnimIndex = ECast(CPlayer::Player_State::Player_Walk_FL45);
		//	if (Get_CurrentAnimIndex() != AnimIndex)
		//	{
		//		m_pBody->Set_RotateUpperX(MoveDirection::FrontLeft);
		//		Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		//	}
		//	Go_Straight_L45(fTimeDelta * 0.5f);
		//	
		//}
		//else if (m_pGameInstance->Key_Pressing(DIK_D))
		//{
		//	AnimIndex = ECast(CPlayer::Player_State::Player_Walk_FR45);
		//	if (Get_CurrentAnimIndex() != AnimIndex)
		//	{
		//		m_pBody->Set_RotateUpperX(MoveDirection::FrontRight);
		//		Set_Animation(AnimwwwddwIndex, CModel::ANIM_STATE_LOOP, true, false);
		//	}
		//	Go_Straight_R45(fTimeDelta * 0.5f);
		//	
		//}
		//else
		{
			AnimIndex = ECast(CPlayer::Player_State::Player_Walk_F);
			//AnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_F);

			if (Get_CurrentAnimIndex() != AnimIndex)
			{
				m_pBody->Set_RotateUpperX(MoveDirection::Front);
				Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
			}
			Go_Straight(fTimeDelta * 0.5f);
			
		}
	}
	else if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		//if (m_pGameInstance->Key_Pressing(DIK_A))
		//{
		//	AnimIndex = ECast(CPlayer::Player_State::Player_Walk_BL135);
		//	if (Get_CurrentAnimIndex() != AnimIndex)
		//	{
		//		m_pBody->Set_RotateUpperX(MoveDirection::BackLeft);
		//		Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		//	}
		//	Go_Backward_L45(fTimeDelta * 0.5f);
		//	
		//}
		//else if (m_pGameInstance->Key_Pressing(DIK_D))
		//{
		//	AnimIndex = ECast(CPlayer::Player_State::Player_Walk_BR135);
		//	if (Get_CurrentAnimIndex() != AnimIndex)
		//	{
		//		m_pBody->Set_RotateUpperX(MoveDirection::BackRight);
		//		Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		//	}
		//	Go_Backward_R45(fTimeDelta * 0.5f);
		//	
		//}
		//else
		{
			AnimIndex = ECast(CPlayer::Player_State::Player_Walk_B);
			//AnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_B);
			if (Get_CurrentAnimIndex() != AnimIndex)
			{
				m_pBody->Set_RotateUpperX(MoveDirection::Back);
				Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
			}
			Go_Backward(fTimeDelta * 0.5f);
			
		}
	}
	else if (m_pGameInstance->Key_Pressing(DIK_A))
	{
		AnimIndex = ECast(CPlayer::Player_State::Player_Walk_FL);
		//AnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_FL);
		//ReversedAnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_FR);
		//if (Get_CurrentAnimIndex() != AnimIndex || ReversedAnimIndex == AnimIndex)
		//{
		//	Set_Animation(ReversedAnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		//}
		if (Get_CurrentAnimIndex() != AnimIndex)
		{
			m_pBody->Set_RotateUpperX(MoveDirection::Left);
			Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		}
		Go_Left(fTimeDelta * 0.5f);
		
	}
	else if (m_pGameInstance->Key_Pressing(DIK_D))
	{
		AnimIndex = ECast(CPlayer::Player_State::Player_Walk_FR);
		//AnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_FR);
		//ReversedAnimIndex = ECast(CPlayer::Player_State::Player_Bandit_WalkAim_FL);
		//if (Get_CurrentAnimIndex() != AnimIndex || ReversedAnimIndex == AnimIndex)
		//{
		//	Set_Animation(ReversedAnimIndex, CModel::ANIM_STATE_LOOP_REVERSE, true, false);
		//}
		if (Get_CurrentAnimIndex() != AnimIndex)
		{
			m_pBody->Set_RotateUpperX(MoveDirection::Right);
			Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
		}
		Go_Right(fTimeDelta * 0.5f);
		
	}
	else
	{
		AnimIndex = ECast(CPlayer::Player_State::Player_IdleLoop);
		if (Get_CurrentAnimIndex() != AnimIndex)
		{
			m_pBody->Set_RotateUpperX(MoveDirection::Front);
			Set_Animation(AnimIndex, CModel::ANIM_STATE_LOOP, true, false);
			
		}
	}
}

void CPlayer::Activate_ShootingReaction(_float fHeight)
{
	m_pBody->Activate_ShootingReaction(fHeight);
}

string CPlayer::Get_HUD_Tag(HUD eHUD)
{
	string strHUDTag = "";

	switch (eHUD)
	{
	case Client::CPlayer::HUD::LEFT_TOP:
		strHUDTag = "LeftHUD_Top";
		break;
	case Client::CPlayer::HUD::LEFT_RIGHT:
		strHUDTag = "LeftHUD_Right";
		break;
	case Client::CPlayer::HUD::LEFT_BOTTOM:
		strHUDTag = "LeftHUD_Bottom";
		break;
	case Client::CPlayer::HUD::LEFT_LEFT:
		strHUDTag = "LeftHUD_Left";
		break;
	case Client::CPlayer::HUD::RIGHT_TOP:
		strHUDTag = "RightHUD_Top";
		break;
	case Client::CPlayer::HUD::RIGHT_RIGHT:
		strHUDTag = "RightHUD_Right";
		break;
	case Client::CPlayer::HUD::RIGHT_BOTTOM:
		strHUDTag = "RightHUD_Bottom";
		break;
	case Client::CPlayer::HUD::RIGHT_LEFT:
		strHUDTag = "RightHUD_Left";
		break;
	}

	return strHUDTag;
}

void CPlayer::Set_HUD_MaxCooltime(HUD eHUD, _float fCurrnetCooltime)
{
	string strHUDTag = Get_HUD_Tag(eHUD);
	_uint iIndex = ECast(eHUD);
	m_MaxCooltimes[iIndex] = fCurrnetCooltime;

	switch (eHUD)
	{
	case Client::CPlayer::HUD::LEFT_TOP:
	case Client::CPlayer::HUD::LEFT_RIGHT:
	case Client::CPlayer::HUD::LEFT_BOTTOM:
	case Client::CPlayer::HUD::LEFT_LEFT:
		m_pUIManager->Change_LeftHUD_MaxCoolTime(strHUDTag, fCurrnetCooltime);
		break;

	case Client::CPlayer::HUD::RIGHT_TOP:
	case Client::CPlayer::HUD::RIGHT_RIGHT:
	case Client::CPlayer::HUD::RIGHT_BOTTOM:
	case Client::CPlayer::HUD::RIGHT_LEFT:
		m_pUIManager->Change_RightHUD_MaxCoolTime(strHUDTag, fCurrnetCooltime);
		break;
	}
}

void CPlayer::Set_HUD_Cooltime(HUD eHUD, _float fCurrnetCooltime)
{
	string strHUDTag = Get_HUD_Tag(eHUD);

	switch (eHUD)
	{
	case Client::CPlayer::HUD::LEFT_TOP:
	case Client::CPlayer::HUD::LEFT_RIGHT:
	case Client::CPlayer::HUD::LEFT_BOTTOM:
	case Client::CPlayer::HUD::LEFT_LEFT:
		m_pUIManager->Change_LeftHUD_CurrentCoolTime(strHUDTag, fCurrnetCooltime);
		break;

	case Client::CPlayer::HUD::RIGHT_TOP:
	case Client::CPlayer::HUD::RIGHT_RIGHT:
	case Client::CPlayer::HUD::RIGHT_BOTTOM:
	case Client::CPlayer::HUD::RIGHT_LEFT:
		m_pUIManager->Change_RightHUD_CurrentCoolTime(strHUDTag, fCurrnetCooltime);
		break;
	}
}

_float CPlayer::Get_HUD_Cooltime(HUD eHUD)
{
	_float fCooltime = 10000.f;
	string strHUDTag = Get_HUD_Tag(eHUD);

	switch (eHUD)
	{
	case Client::CPlayer::HUD::LEFT_TOP:
	case Client::CPlayer::HUD::LEFT_RIGHT:
	case Client::CPlayer::HUD::LEFT_BOTTOM:
	case Client::CPlayer::HUD::LEFT_LEFT:
		fCooltime = m_pUIManager->Get_LeftHUD_CurrentCoolTime(strHUDTag);
		break;
	case Client::CPlayer::HUD::RIGHT_TOP:
	case Client::CPlayer::HUD::RIGHT_RIGHT:
	case Client::CPlayer::HUD::RIGHT_BOTTOM:
	case Client::CPlayer::HUD::RIGHT_LEFT:
		fCooltime = m_pUIManager->Get_RightHUD_CurrentCoolTime(strHUDTag);
		break;
	}

	return fCooltime;
}

//_bool CPlayer::Activate_HUD_Skill(HUD eHUD)
//{
//	_uint iIndex = ECast(eHUD);
//	_float fCooltime = m_MaxCooltimes[iIndex];
//
//	Set_HUD_Cooltime(eHUD, fCooltime);
//
//	return true;
//}

_bool CPlayer::Activate_HUD_Skill(HUD eHUD, _float fCost)
{
	_bool bIsCooltimeEnd = Is_HUD_Cooltime_End(eHUD, fCost);
	if (false == bIsCooltimeEnd)
	{

		return false;
	}

	_float fCurrentCooltime = Get_HUD_Cooltime(eHUD);
	_float fResultCooltime = fCurrentCooltime;
	if (0 > fCost)
	{
		_uint iIndex = ECast(eHUD);
		fResultCooltime = m_MaxCooltimes[iIndex];
	}
	else 
	{
		fResultCooltime += fCost;
	}

	Set_HUD_Cooltime(eHUD, fResultCooltime);

	return true;
}

_bool CPlayer::Is_HUD_Cooltime_End(HUD eHUD, _float fCost)
{
	_float fCooltime = 10000.f;

	_uint iIndex = ECast(eHUD);
	
	_float fMaxCooltime = m_MaxCooltimes[iIndex];
	string strHUDTag = Get_HUD_Tag(eHUD);

	switch (eHUD)
	{
	case Client::CPlayer::HUD::LEFT_TOP:
	case Client::CPlayer::HUD::LEFT_RIGHT:
	case Client::CPlayer::HUD::LEFT_BOTTOM:
	case Client::CPlayer::HUD::LEFT_LEFT:
		fCooltime = m_pUIManager->Get_LeftHUD_CurrentCoolTime(strHUDTag);
		//fMaxCooltime = m_pUIManager->Get_LeftHUD_MaxCoolTime(strHUDTag);
		break;
	case Client::CPlayer::HUD::RIGHT_TOP:
	case Client::CPlayer::HUD::RIGHT_RIGHT:
	case Client::CPlayer::HUD::RIGHT_BOTTOM:
	case Client::CPlayer::HUD::RIGHT_LEFT:
		fCooltime = m_pUIManager->Get_RightHUD_CurrentCoolTime(strHUDTag);
		//fMaxCooltime = m_pUIManager->Get_RightHUD_MaxCoolTime(strHUDTag);
		break;
	}

	_bool bResult = { false };
	if (0 > fCost) 
	{	
		bResult = 0.1f >= fCooltime;
	}
	else 
	{
		_float fDiff = 0.f;

		if (fCooltime >= fMaxCooltime || 0.f >= fCooltime )
		{
			fDiff = fCost + 1.f;
		}
		else 
		{
			fDiff = fMaxCooltime - fCooltime;
		}

		bResult = fDiff >= fCost;
	}

	return bResult;
}

CPlayer::HUD CPlayer::Get_Skill_HUD_Enum(Player_Skill ePlayer_Skill)
{
	switch (ePlayer_Skill)
	{
	case Client::CPlayer::Player_Skill::SUPER_CHARGE:
		return HUD::LEFT_TOP;
	case Client::CPlayer::Player_Skill::HEAL:
		return HUD::LEFT_RIGHT;
	case Client::CPlayer::Player_Skill::REVOLVER:
		return HUD::LEFT_BOTTOM;
	case Client::CPlayer::Player_Skill::SHOTGUN:
		return HUD::LEFT_LEFT;

	case Client::CPlayer::Player_Skill::RIFLE:
		return HUD::RIGHT_TOP;
	case Client::CPlayer::Player_Skill::SLAM_DOWM:
		return HUD::RIGHT_RIGHT;
	case Client::CPlayer::Player_Skill::KICK:
		return HUD::RIGHT_BOTTOM;
	case Client::CPlayer::Player_Skill::ELECTRIC_WHIP:
		return HUD::RIGHT_LEFT;
	}

	return HUD();
}



#pragma region ��ȣ�ۿ�

void CPlayer::SetState_InteractJumpDown100()
{
	m_pActor->Set_State(new CPlayer_InteractionJumpDown100());
}

void CPlayer::SetState_InteractJumpDown200()
{
	m_pActor->Set_State(new CPlayer_InteractionJumpDown200());
}

void CPlayer::SetState_InteractJumpDown300()
{
	m_pActor->Set_State(new CPlayer_InteractionJumpDown300());
}

void CPlayer::SetState_InteractVault100()
{
	m_pActor->Set_State(new CPlayer_InteractionVault100());
}

void CPlayer::SetState_InteractVault200()
{
	m_pActor->Set_State(new CPlayer_InteractionVault200());
}

void CPlayer::SetState_InteractCartRideStart()
{
	m_pActor->Set_State(new CPlayer_CartRide_Start());
}

void CPlayer::SetState_InteractCartRideLoop()
{	
	m_pActor->Set_State(new CPlayer_CartRide_Loop());
}

void CPlayer::SetState_InteractCartRideWagonJump()
{
	m_pActor->Set_State(new CPlayer_L06_Wagon_Jump());
}


void CPlayer::SetState_InteractionPush_Rock_Idle()
{
	m_pActor->Set_State(new CPlayer_InteractionPush_Rock_Idle());
}

void CPlayer::SetState_InteractionPull_Rock_Idle()
{
	m_pActor->Set_State(new CPlayer_InteractionPull_Rock_Idle());
}

void CPlayer::SetState_InteractionPush_End()
{
	m_pActor->Set_State(new CPlayer_InteractionPush_Rock_End());
}

void CPlayer::SetState_InteractionPull_End()
{
	m_pActor->Set_State(new CPlayer_InteractionPull_Rock_End());
}

void CPlayer::SetState_InteractClimb100()
{
	m_pActor->Set_State(new CPlayer_InteractionClimb100());
}

void CPlayer::SetState_InteractClimb200()
{
	m_pActor->Set_State(new CPlayer_InteractionClimb200());
}

void CPlayer::SetState_InteractClimb300()
{
	m_pActor->Set_State(new CPlayer_InteractionClimb300());
}

void CPlayer::SetState_InteractClimb450()
{
	m_pActor->Set_State(new CPlayer_InteractionClimb450());
}

void CPlayer::SetState_InteractSlide()
{
	m_pActor->Set_State(new CPlayer_Interaction_Slide_Sequence());
}

void CPlayer::SetState_InteractSmallLever()
{
	m_pActor->Set_State(new CPlayer_Interaction_SmallLever());
}

void CPlayer::SetState_InteractPlankStart()
{
	m_pActor->Set_State(new CPlayer_InteractionPlank_Up_Start());
}

void CPlayer::SetState_InteractRopeDown()
{
	m_pActor->Set_State(new CPlayer_InteractionRope_Down_Start());
}

void CPlayer::SetState_InteractClimbRope()
{
	m_pActor->Set_State(new CPlayer_InteractionClimbRope_Start());
}


void CPlayer::SetState_InteractDoorOpen()
{
	m_pActor->Set_State(new CPlayer_InteractionDoor_Open());
}

void CPlayer::SetState_InteractDoorPush()
{
	m_pActor->Set_State(new CPlayer_InteractionDoorPush());
}

void CPlayer::SetState_InteractLadderUpStart()
{
	m_pActor->Set_State(new CPlayer_InteractionLadder_Up_Start());
}

void CPlayer::SetState_InteractWhipSwing()
{
	m_pActor->Set_State(new CPlayer_InteractionWhipSwing());
}

void CPlayer::SetState_InteractWhipPull()
{
	m_pActor->Set_State(new CPlayer_InteractionWhipPull());
}

void CPlayer::SetState_InteractRotationValve()
{
	m_pActor->Set_State(new CPlayer_InteractionRotateValve_01());
}

void CPlayer::SetState_InteractZipLine()
{
	m_pActor->Set_State(new CPlayer_ZipLine_Start());
}

void CPlayer::SetState_CrouchUnder()
{
	m_pActor->Set_State(new CPlayer_CrouchUnder_Start());
}

void CPlayer::SetState_CrouchUnderGate()
{
	m_pActor->Set_State(new CPlayer_CrouchUnder_Gate());
}


#pragma endregion ��ȣ�ۿ�

void CPlayer::Search_Target(_float fMaxDistance)
{
	__super::Search_Target(LAYER_MONSTER, fMaxDistance);
	__super::Search_Target(LAYER_BOSS, fMaxDistance);
}

void CPlayer::Chasing_Attack(_float fTimeDelta, _float fMaxDistance, _uint iCount)
{
	if (nullptr == m_pTarget || 0 >= m_pTarget->Get_Hp() || true == m_pTarget->Is_Dead() || false == m_pTarget->Get_Enable())
	{
		Search_Target(fMaxDistance);
	}

	//Search_Target(fMaxDistance);

	if (m_pTarget)
	{
		Look_At_Target();
		for (_uint i = 0; i < iCount; ++i) 
		{
			Move_In_Proportion_To_Enemy(fTimeDelta);
		}
	}
}

void CPlayer::LeftHUDCoolDown(const string& strUIName, _float fCoolTime)
{
	m_pUIManager->Change_LeftHUD_CurrentCoolTime(strUIName, fCoolTime); // ��� : UI��ü ã�Ƽ� �ٷ� �����ϴ� �� (�ȹް� ��������)
}

_float CPlayer::Get_LeftHUDMaxCoolTime(const string& strUIName)
{
	return m_pUIManager->Get_LeftHUD_MaxCoolTime(strUIName); // ��� : UI��ü ã�Ƽ� �ٷ� �����ϴ� �� (�ȹް� ��������)
}

void CPlayer::Set_DiedScreen(_bool _bShowDiedScreen)
{
	m_bShowDiedScreen = _bShowDiedScreen;

	if (m_bShowDiedScreen == true)
	{
		wstring strFileName = L"";
		strFileName = L"HM_UI_YouDiedScreen_Continue.wav";

		m_pGameInstance->Play_Sound(L"UI_MouseOver", strFileName, CHANNELID::SOUND_UI_MOUSEOVER, 10.f);

		m_pUIManager->Active_DiedScreen();
		m_pDataManager->Set_GameState(GAME_STATE::UI);
	}
	else
	{
		m_fHp = m_fMaxHp;
		m_pUIManager->NonActive_DiedScreen();
		m_pDataManager->Set_GameState(GAME_STATE::GAMEPLAY);
	}

}

void CPlayer::KeyInput(_float fTimeDelta)
{
	/* ! UI : ShaderOption Window / Key : Esc */
	if (m_pGameInstance->Key_Down(DIK_ESCAPE))
	{
		m_bShowOption = !m_bShowOption;
	
		if (m_bShowOption == true)
		{
			m_pGameInstance->Get_Renderer()->Set_DOF_Active(true);
			m_pUIManager->Active_Option();
			m_pUIManager->Active_MouseCursor();
			m_pDataManager->Set_GameState(GAME_STATE::UI);
		}
		else
		{
			m_pGameInstance->Get_Renderer()->Set_DOF_Active(false);
			m_pUIManager->NonActive_Option();
			m_pUIManager->Active_MouseCursor();
			m_pDataManager->Set_GameState(GAME_STATE::GAMEPLAY);
		}
	}

	if (m_pGameInstance->Key_Down(DIK_P))
	{
		m_pUIManager->Active_LetterBox();
	}
	if (m_pGameInstance->Key_Down(DIK_O))
	{
		m_pUIManager->NonActive_LetterBox();
	}

	/* ! UI : SkillWindow / Key : K */
	if (m_pGameInstance->Key_Down(DIK_K))
	{
		m_bShowSkillWindow = !m_bShowSkillWindow;

		if (m_bShowSkillWindow == true)
		{
			/* Sound */
			wstring strFileName = L"";
			strFileName = L"Player_Rage_On_01.wav";

			m_pGameInstance->Play_Sound(L"UI_SkillWindow", strFileName, CHANNELID::SOUND_UI_SKILLWINDOWON, 13.f);

			/* voice */
			_uint iRand = SMath::Random(0, 3);
			switch (iRand)
			{
			case 0:
				strFileName = L"oneliner_dynamite_jesse_1.wav";
				break;
			case 1:
				strFileName = L"oneliner_healingkit_jesse_1.wav";
				break;
			case 2:
				strFileName = L"oneliner_zapperdash_jesse_2_ALT01.wav";
				break;
			default:
				strFileName = L"oneliner_zapperblock_jesse_1_ALT01.wav";
				break;
			}
			m_pGameInstance->Play_Sound(L"UI_SkillWindow", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

			//oneliner_dynamite_jesse_1
			//oneliner_healingkit_jesse_1
			//oneliner_zapperdash_jesse_2_ALT01
			//oneliner_zapperblock_jesse_1_ALT01

			m_pUIManager->Active_SkillWindowBackground();
			m_pUIManager->NonActive_PlayerHUD(); // PlayerHUD Off
			m_pUIManager->NonActive_MouseCursor(); // MouseCursor Off
			m_pUIManager->NonActive_Crosshair(); // Crosshair Off
			m_pDataManager->Set_GameState(GAME_STATE::UI);
		}
		else
		{
			wstring strFileName = L"";
			strFileName = L"Player_Rage_Full.wav";

			m_pGameInstance->Play_Sound(L"UI_SkillWindow", strFileName, CHANNELID::SOUND_UI_SKILLWINDOWON, 13.f);

			m_pUIManager->NonActive_SkillWindowAll();
			m_pUIManager->Active_Crosshair(true); // Crosshair On
			m_pDataManager->Set_GameState(GAME_STATE::GAMEPLAY);
		}
	}

	/* ! UI : DiedScreen / Key : I */
	if (m_pGameInstance->Key_Down(DIK_I))
	{
		m_bShowDiedScreen = !m_bShowDiedScreen;
	
		if (m_bShowDiedScreen == true)
		{
			m_pUIManager->Active_DiedScreen();
			m_pDataManager->Set_GameState(GAME_STATE::UI);
		}
		else
		{
			m_fHp = m_fMaxHp;
			m_pUIManager->NonActive_DiedScreen();
			m_pDataManager->Set_GameState(GAME_STATE::GAMEPLAY);
		}
	}

	/* ! UI : TestText / Key : 6 */
	if (m_pGameInstance->Key_Down(DIK_5))
	{
		m_pUIManager->Set_SkillLevel("ElectricCord", CUI::UI_LEVEL::LEVEL1); // LEVEL1 == ��� ���� ����
	}
	/* ! UI : TestText / Key : 7 */
	if (m_pGameInstance->Key_Down(DIK_6))
	{
		m_pUIManager->Set_SkillLevel("ElectricDash", CUI::UI_LEVEL::LEVEL1); // LEVEL1 == ��� ���� ����
	}

	/* ! UI : TestText / Key : 7 */
	if (m_pGameInstance->Key_Down(DIK_7))
	{
		/* �Լ� �̸� �������̰� �ٲ���ϴ�. */

		// !���� �߰� : =>Ư�� ��ų�� ��Ÿ���� ã�Ƽ� CurrentCoolTime�� �����ϴ� ��� 2����<=
		//CUI* pUI = m_pUIManager->Get_LeftHUD("LeftHUD_Right"); // 1. ù��° ��� : UI��ü �޾ƿ��� �� (�޾Ƽ� ��������)
		m_pUIManager->Change_LeftHUD_CurrentCoolTime("LeftHUD_Right", 5.f); // 2. �ι�° ��� : UI��ü ã�Ƽ� �ٷ� �����ϴ� �� (�ȹް� ��������)
		
		// !���� �߰� : =>Ư�� ��ų�� ��Ÿ���� ã�Ƽ� MaxCoolTime�� �����ϴ� ���<=
		m_pUIManager->Change_LeftHUD_MaxCoolTime("LeftHUD_Right", 5.f); // 2. �ι�° ��� : UI��ü ã�Ƽ� �ٷ� �����ϴ� �� (�ȹް� ��������)
	}
	/* ! UI : TestText / Key : 8 */
	if (m_pGameInstance->Key_Down(DIK_8))
	{
		m_pUIManager->Active_TutorialBox();
		m_pUIManager->Change_TutorialText(TUTORIAL_TEXT::START);
	}
	///* ! UI : TestText / Key : 9 */
	//if (m_pGameInstance->Key_Down(DIK_9))
	//{
	//	m_pUIManager->Active_TutorialBox();
	//	m_pUIManager->Change_TutorialText(TUTORIAL_TEXT::CRANE);
	//}

	/* ! UI : SkillWindow / Key : K (!���� UI �ȳ���) */
	if (m_pGameInstance->Key_Down(DIK_K))
	{
		//m_bShowSkillWindow = !m_bShowSkillWindow;

		//if (m_bShowSkillWindow == true)
		//	m_pUIManager->Active_SkillWindow();
		//else
		//	m_pUIManager->NonActive_SkillWindwo();
	}
}

HRESULT CPlayer::Ready_Components()
{
	/* ������ ����Ʈ �߰� */
	if(m_iCurrnetLevel == ECast(LEVEL::LEVEL_SNOWMOUNTAIN) || m_iCurrnetLevel == ECast(LEVEL::LEVEL_SNOWMOUNTAINBOSS) )
		m_pEffect = EFFECT_MANAGER->Play_Effect("Player/Breath/", "SY_Player_Breath04.json", this, true, "lips_H_close_upnode");
	
	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody::BODY_DESC		BodyDesc = {};
	FAILED_CHECK(Add_Body(TEXT("Prototype_GameObject_Body_Player"), BodyDesc));

	//if (m_pGameInstance->Get_NextLevel() != ECast(LEVEL_TOOL))
	//{
		
	CWeapon::WEAPON_DESC		WeaponDesc = {};
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Punch"), "LeftHandIK", WeaponDesc,				PLAYER_WEAPON_PUNCH_L));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Punch"), "RightHandIK", WeaponDesc,			PLAYER_WEAPON_PUNCH_R));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_ELWinchester"), "RightHandIK", WeaponDesc,		PLAYER_WEAPON_WINCHESTER));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Revolver"), "RightInHandIndex", WeaponDesc,	PLAYER_WEAPON_REVOLVER));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_ELShotgun"), "RightInHandIndex", WeaponDesc,	PLAYER_WEAPON_SHOTGUN));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Dynamite"), "RightInHandIndex", WeaponDesc,	PLAYER_WEAPON_DYNAMITE));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Whip"), "RightInHandIndex", WeaponDesc,		PLAYER_WEAPON_WHIP));


	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Kick"), "RightFoot", WeaponDesc,			PLAYER_WEAPON_KICK));
	FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Player_Weapon_Zapper"), "LeftHandIK", WeaponDesc,		PLAYER_WEAPON_ZAPPER));

	
	Set_Weapons_Enable_False();

	//CWeapon* m_pWeapon_Punch_L = Get_Weapon(PLAYER_WEAPON_PUNCH_L);
	//m_pWeapon_Punch_L->Set_Enable(false);
	//
	//CWeapon* m_pWeapon_Punch_R = Get_Weapon(PLAYER_WEAPON_PUNCH_R);
	//m_pWeapon_Punch_R->Set_Enable(false);
	//
	//CWeapon* m_pWeapon_Winchester = Get_Weapon(PLAYER_WEAPON_WINCHESTER);
	//m_pWeapon_Winchester->Set_Enable(false);

	//CWeapon* m_pWeapon_Revolver = Get_Weapon(PLAYER_WEAPON_REVOLVER);
	//m_pWeapon_Revolver->Set_Enable(false);

	//CWeapon* m_pWeapon_Shotgun = Get_Weapon(PLAYER_WEAPON_SHOTGUN);
	//m_pWeapon_Shotgun->Set_Enable(false);

	//CWeapon* m_pWeapon_Kick = Get_Weapon(PLAYER_WEAPON_KICK);
	//m_pWeapon_Kick->Set_Enable(false);

	//CWeapon* m_pWeapon_Zapper = Get_Weapon(PLAYER_WEAPON_ZAPPER);
	//m_pWeapon_Zapper->Set_Enable(false);

	

	
	return S_OK;
}

void CPlayer::Check_Frustum()
{
	m_bIsInFrustum = true;
}

void CPlayer::Update_ChargingTime(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Pressing(DIM_LB)) 
	{
		m_fChargingTime += fTimeDelta;
	}
	else 
	{
		m_fChargingTime = 0.f;
	}
}

void CPlayer::Slam()
{
	CGameObject* pSlam = m_pGameInstance->Add_CloneObject_And_Get(m_iCurrnetLevel, LAYER_PLAYER_BULLET, L"Prototype_GameObject_Impact_Slam");
	pSlam->Set_Position(Get_Position());
}

void CPlayer::Throw_Dynamite()
{
	CGameObject* pDynamite = m_pGameInstance->Add_CloneObject_And_Get(m_iCurrnetLevel, LAYER_PLAYER_BULLET, L"Prototype_GameObject_Bullet_Dynamite");
	
	_float3 vSpawnPos = Get_Position();
	vSpawnPos.y += 1.f;
	
	_float3 vTargetPos = Calc_Front_Pos(_float3(0.f, 1.f, 1.f));
	
	pDynamite->Set_Position(vSpawnPos);
	pDynamite->Get_Transform()->Look_At_OnLand(vTargetPos);

}

void CPlayer::Teleport()
{
	CGameObject* pTeleport = m_pGameInstance->Add_CloneObject_And_Get(m_iCurrnetLevel, LAYER_PLAYER_BULLET, L"Prototype_GameObject_Bullet_Teleport");

	_float3 vSpawnPos = Get_Position();
	vSpawnPos.y += 1.f;

	_float3 vTargetPos = Calc_Front_Pos(_float3(0.f, 1.f, 1.f));

	pTeleport->Set_InitPosition(vSpawnPos);
	pTeleport->Get_Transform()->Look_At_OnLand(vTargetPos);
}

void CPlayer::Update_SuperCharge(_float fTimeDelta)
{
	_bool bIs_SuperCharge = Is_SuperCharge();
	if (false == bIs_SuperCharge)
		return;

	_float fTime = m_fSuperChargeTime - fTimeDelta;

	if (fTime > 0.f) 
	{
		m_fSuperChargeTime = fTime;
		Set_BodyRender(2); // SuperCharge Renderpass

		// �������� ��� ����Ʈ ����
		if (m_bFirst_SuperCharge)
		{
			if (nullptr == m_pChargeEffect)
			{
				m_pChargeEffect = EFFECT_MANAGER->Play_Effect("Player/SuperCharge/", "SuperCharge_Always_10SC_03.json", this, true, "Head");
				m_bFirst_SuperCharge = false;
			}
		}
		//EFFECT_MANAGER->Generate_Effect(&m_fEffectTimeAcc[0], 0.5f, fTimeDelta, "Player/SuperCharge/", "SuperCharge_Always_Pos_Down_04.json", Get_Position());
		//EFFECT_MANAGER->Generate_Effect(&m_fEffectTimeAcc[1], 0.5f, fTimeDelta, "Player/SuperCharge/", "SuperCharge_Always_Pos_Up_04.json", Get_Position());
		//EFFECT_MANAGER->Generate_Effect(&m_fEffectTimeAcc, 0.5f, fTimeDelta, "Player/SuperCharge/", "SuperCharge_Always_Pos_03.json", Get_Position());
		//EFFECT_MANAGER->Generate_Effect_AttachBone(&m_fEffectTimeAcc, 0.5f, fTimeDelta, "Player/SuperCharge/", "SuperCharge_Always_02.json", this, true, "Head");
	}
	else
	{
		m_pUIManager->NonActive_SuperChargeMod(); // !���� : �������� ��� UI ����
		m_fSuperChargeTime = 0.f;

		m_fEffectTimeAcc = 0.f;

		if (false == m_bFirst_SuperCharge) // ���������� �������� ����Ʈ ��ȯ�ϱ�
		{
			/* Bdoy �⺻���������� ��ȯ */
			if (m_iCurrnetLevel == ECast(LEVEL::LEVEL_SNOWMOUNTAIN))
				Set_BodyRender(3);
			else
				Set_BodyRender(0);

			EFFECT_MANAGER->Return_ToPool(m_pChargeEffect);
			m_pChargeEffect = nullptr;
			m_bFirst_SuperCharge = true;
			m_bSuperCharge_State = false;
		}
			
	}


	if (0.f == m_fSuperChargeTime)
	{
		Play_Sound_SuperCharge_Exit();
	}

	//m_fSuperChargeTime = fTime > 0 ? fTime : 0.f;
}

void CPlayer::Play_Sound_SuperCharge_Enter()
{
	wstring strFileName = L"";
	_uint iRand = SMath::Random(0, 3);
	switch (iRand)
	{
	case 0:
		strFileName = L"Player_EnergyMode_Enter_01.wav";
		break;
	case 1:
		strFileName = L"Player_EnergyMode_Enter_02.wav";
		break;
	case 2:
		strFileName = L"Player_EnergyMode_Enter_03.wav";
		break;
	default:	
		strFileName = L"Player_EnergyMode_Enter_01.wav";
		break;
	}
	m_pGameInstance->Play_Sound(L"SUPER_CHARGE", strFileName, CHANNELID::SOUND_SUPER_CHARGE, 10.f);

}

void CPlayer::Play_Sound_SuperCharge_Exit()
{
	wstring strFileName = L"";
	_uint iRand = SMath::Random(0, 5);		
	switch (iRand)
	{
	case 0:
		strFileName = L"player_supercharged_energy_mode_off_001.wav";
		break;
	case 1:
		strFileName = L"player_supercharged_energy_mode_off_002.wav";
		break;
	case 2:
		strFileName = L"player_supercharged_energy_mode_off_003.wav";
		break;
	case 3:
		strFileName = L"player_supercharged_energy_mode_off_004.wav";
		break;
	case 4:
		strFileName = L"player_supercharged_energy_mode_off_005.wav";
		break;
	default:
		strFileName = L"player_supercharged_energy_mode_off_001.wav";
		break;
	}
	m_pGameInstance->Play_Sound(L"SUPER_CHARGE", strFileName, CHANNELID::SOUND_SUPER_CHARGE, 10.f);
}

// !���� : �������� ��� UI ON
void CPlayer::Activate_SuperChargeUI()
{
	m_pUIManager->Active_SuperChargeMod();
}

void CPlayer::Search_LockOn_Target()
{
	m_pLockOnTarget = Select_The_Nearest_Enemy(LAYER_MONSTER);
}

void CPlayer::Hitted_Left(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CPlayer_HitNormal_L());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CPlayer_HitNormal_L());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CPlayer_HitHeavy_F_5m());
		break;
	default:
		m_pActor->Set_State(new CPlayer_HitNormal_L());
		break;
	}
}

void CPlayer::Hitted_Right(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CPlayer_HitNormal_R());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CPlayer_HitNormal_R());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CPlayer_HitHeavy_F_5m());
		break;
	default:
		m_pActor->Set_State(new CPlayer_HitNormal_R());
		break;
	}
}

void CPlayer::Hitted_Front(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CPlayer_HitNormal_F());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CPlayer_HitNormal_F());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CPlayer_HitHeavy_F_5m());
		break;
	default:
		m_pActor->Set_State(new CPlayer_HitNormal_F());
		break;
	}
}

void CPlayer::Hitted_Knock(_bool bIsCannonball)
{
	//if (bIsCannonball)
	//{
	//	m_pActor->Set_State(new CInfected_KnockFrontCannonball_F_01_TEMP());
	//}
	//else
	//{
	//	m_pActor->Set_State(new CInfected_KnockFrontLight_F_01_NEW());
	//}

	m_pActor->Set_State(new CPlayer_KnockFrontLight_F_02());
}

void CPlayer::Hitted_Dead(Power ePower)
{

	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CPlayer_DeathLight_F_01());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CPlayer_DeathNormal_F_01());
		break;
	default:
		m_pActor->Set_State(new CPlayer_DeathNormal_F_01());
		break;
	}
}

void CPlayer::Play_Whoosh_Sound()
{
	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 4);
	switch (iRand)
	{
	case 0:
		strFileName = L"Player_Punch_Whoosh_01_new.wav";
		break;
	case 1:
		strFileName = L"Player_Punch_Whoosh_02_new.wav";
		break;
	case 2:
		strFileName = L"Player_Punch_Whoosh_03_new.wav";
		break;
	case 3:
		strFileName = L"Player_Punch_Whoosh_04_new.wav";
		break;
	default:
		strFileName = L"Player_Punch_Whoosh_01_new.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_WHOOSH", strFileName, CHANNELID::SOUND_PLAYER_WHOOSH, 10.f);
}

void CPlayer::Play_Hitted_Sound()
{
	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 23);
	switch (iRand)
	{
	case 0:
		strFileName = L"VO_Jesse_TakingDamageHitA01.wav";
		break;
	case 1:
		strFileName = L"VO_Jesse_TakingDamageHitA02.wav";
		break;
	case 2:
		strFileName = L"VO_Jesse_TakingDamageHitA03.wav";
		break;
	case 3:
		strFileName = L"VO_Jesse_TakingDamageHitA04.wav";
		break;
	case 4:
		strFileName = L"VO_Jesse_TakingDamageHitA05.wav";
		break;
	case 5:
		strFileName = L"VO_Jesse_TakingDamageHitA06.wav";
		break;
	case 6:
		strFileName = L"VO_Jesse_TakingDamageHitA07.wav";
		break;
	case 7:
		strFileName = L"VO_Jesse_TakingDamageHitA08.wav";
		break;
	case 8:
		strFileName = L"VO_Jesse_TakingDamageHitA09.wav";
		break;
	case 9:
		strFileName = L"VO_Jesse_TakingDamageHitA10.wav";
		break;
	case 10:
		strFileName = L"VO_Jesse_TakingDamageHitA11.wav";
		break;
	case 11:
		strFileName = L"VO_Jesse_TakingDamageHitA12.wav";
		break;
	case 12:
		strFileName = L"VO_Jesse_TakingDamageHitA13.wav";
		break;
	case 13:
		strFileName = L"VO_Jesse_TakingDamageHitA14.wav";
		break;
	case 14:
		strFileName = L"VO_Jesse_TakingDamageHitA15.wav";
		break;
	case 15:
		strFileName = L"VO_Jesse_TakingDamageHitA16.wav";
		break;
	case 16:
		strFileName = L"VO_Jesse_TakingDamageHitA17.wav";
		break;
	case 17:
		strFileName = L"VO_Jesse_TakingDamageHitA18.wav";
		break;
	case 18:
		strFileName = L"VO_Jesse_TakingDamageHitA19.wav";
		break;
	case 19:
		strFileName = L"VO_Jesse_TakingDamageHitA20.wav";
		break;
	case 20:
		strFileName = L"VO_Jesse_TakingDamageHitA21.wav";
		break;
	case 21:
		strFileName = L"VO_Jesse_TakingDamageHitA22.wav";
		break;
	case 22:
		strFileName = L"VO_Jesse_TakingDamageHitA23.wav";
		break;
	default:
		strFileName = L"VO_Jesse_TakingDamageHitA01.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_HITTED", strFileName, CHANNELID::SOUND_PLAYER_HITTED, 10.f);
}

void CPlayer::Play_Dead_Sound()
{
	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 5);
	switch (iRand)
	{
	case 0:
		strFileName = L"Jesse_Die_VO_01.wav";
		break;
	case 1:
		strFileName = L"Jesse_Die_VO_02.wav";
		break;
	case 2:
		strFileName = L"Jesse_Die_VO_03.wav";
		break;
	case 3:
		strFileName = L"Jesse_Die_VO_04.wav";
		break;
	case 4:
		strFileName = L"Jesse_Die_VO_05.wav";
		break;
	default:
		strFileName = L"Jesse_Die_VO_01.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_DEAD", strFileName, CHANNELID::SOUND_PLAYER_DEAD, 10.f);

}

void CPlayer::Play_Kick_Sound()
{
	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 5);
	switch (iRand)
	{
	case 0:
		strFileName = L"player_kick_layer1_001.wav";
		break;
	case 1:
		strFileName = L"player_kick_layer1_002.wav";
		break;
	case 2:
		strFileName = L"player_kick_layer1_003.wav";
		break;
	case 3:
		strFileName = L"player_kick_layer1_004.wav";
		break;
	case 4:
		strFileName = L"player_kick_layer1_005.wav";
		break;
	default:
		strFileName = L"player_kick_layer1_001.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_ATTACK", strFileName, CHANNELID::SOUND_PLAYER_ATTACK, 10.f);
}

void CPlayer::Play_Walk_Sound()
{
	_bool bIsSnow = (
		ECast(LEVEL::LEVEL_SNOWMOUNTAIN) == m_iCurrnetLevel
		|| ECast(LEVEL::LEVEL_SNOWMOUNTAIN) == m_iCurrnetLevel
		);

	wstring strFileName = L"";

	if (true == bIsSnow) 
	{
		_uint iRand = SMath::Random(0, 10);
		switch (iRand)
		{
		case 0:
			strFileName = L"Player_footstep_snow_walk_01.wav";
			break;
		case 1:
			strFileName = L"Player_footstep_snow_walk_02.wav";
			break;
		case 2:
			strFileName = L"Player_footstep_snow_walk_03.wav";
			break;
		case 3:
			strFileName = L"Player_footstep_snow_walk_04.wav";
			break;
		case 4:
			strFileName = L"Player_footstep_snow_walk_05.wav";
			break;
		case 5:
			strFileName = L"Player_footstep_snow_walk_06.wav";
			break;
		case 6:
			strFileName = L"Player_footstep_snow_walk_07.wav";
			break;
		case 7:
			strFileName = L"Player_footstep_snow_walk_08.wav";
			break;
		case 8:
			strFileName = L"Player_footstep_snow_walk_09.wav";
			break;
		case 9:
			strFileName = L"Player_footstep_snow_walk_10.wav";
			break;
		default:
			strFileName = L"Player_footstep_snow_walk_01.wav";
			break;
		}

	}
	else 
	{
		_uint iRand = SMath::Random(0, 9);
		switch (iRand)
		{
		case 0:
			strFileName = L"Player_footstep_soil_walk_01.wav";
			break;
		case 1:
			strFileName = L"Player_footstep_soil_walk_02.wav";
			break;
		case 2:
			strFileName = L"Player_footstep_soil_walk_03.wav";
			break;
		case 3:
			strFileName = L"Player_footstep_soil_walk_04.wav";
			break;
		case 4:
			strFileName = L"Player_footstep_soil_walk_05.wav";
			break;
		case 5:
			strFileName = L"Player_footstep_soil_walk_06.wav";
			break;
		case 6:
			strFileName = L"Player_footstep_soil_walk_07.wav";
			break;
		case 7:
			strFileName = L"Player_footstep_soil_walk_08.wav";
			break;
		case 8:
			strFileName = L"Player_footstep_soil_walk_09.wav";
			break;
		default:
			strFileName = L"Player_footstep_soil_walk_01.wav";
			break;
		}
	}

	m_pGameInstance->Play_Sound(L"PLAYER_FOOTSTEP", strFileName, CHANNELID::SOUND_PLAYER_FOOTSTEP, 10.f);


}

void CPlayer::Play_Run_Sound()
{
	_bool bIsSnow = (
		ECast(LEVEL::LEVEL_SNOWMOUNTAIN) == m_iCurrnetLevel
		|| ECast(LEVEL::LEVEL_SNOWMOUNTAIN) == m_iCurrnetLevel
		);

	wstring strFileName = L"";

	if (true == bIsSnow)
	{
		_uint iRand = SMath::Random(0, 10);
		switch (iRand)
		{
		case 0:
			strFileName = L"Player_footstep_snow_run_01.wav";
			break;
		case 1:
			strFileName = L"Player_footstep_snow_run_02.wav";
			break;
		case 2:
			strFileName = L"Player_footstep_snow_run_03.wav";
			break;
		case 3:
			strFileName = L"Player_footstep_snow_run_04.wav";
			break;
		case 4:
			strFileName = L"Player_footstep_snow_run_05.wav";
			break;
		case 5:
			strFileName = L"Player_footstep_snow_run_06.wav";
			break;
		case 6:
			strFileName = L"Player_footstep_snow_run_07.wav";
			break;
		case 7:
			strFileName = L"Player_footstep_snow_run_08.wav";
			break;
		case 8:
			strFileName = L"Player_footstep_snow_run_09.wav";
			break;
		case 9:
			strFileName = L"Player_footstep_snow_run_10.wav";
			break;
		case 10:
			strFileName = L"Player_footstep_snow_run_11.wav";
			break;
		case 11:
			strFileName = L"Player_footstep_snow_run_12.wav";
			break;
		case 12:
			strFileName = L"Player_footstep_snow_run_13.wav";
			break;
		default:
			strFileName = L"Player_footstep_snow_run_01.wav";
			break;
		}

	}
	else
	{
		_uint iRand = SMath::Random(0, 9);
		switch (iRand)
		{
		case 0:
			strFileName = L"Player_footstep_soil_run_01.wav";
			break;
		case 1:
			strFileName = L"Player_footstep_soil_run_02.wav";
			break;
		case 2:
			strFileName = L"Player_footstep_soil_run_03.wav";
			break;
		case 3:
			strFileName = L"Player_footstep_soil_run_04.wav";
			break;
		case 4:
			strFileName = L"Player_footstep_soil_run_05.wav";
			break;
		case 5:
			strFileName = L"Player_footstep_soil_run_06.wav";
			break;
		case 6:
			strFileName = L"Player_footstep_soil_run_07.wav";
			break;
		case 7:
			strFileName = L"Player_footstep_soil_run_08.wav";
			break;
		case 8:
			strFileName = L"Player_footstep_soil_run_09.wav";
			break;
		default:
			strFileName = L"Player_footstep_soil_run_01.wav";
			break;
		}
	}

	m_pGameInstance->Play_Sound(L"PLAYER_FOOTSTEP", strFileName, CHANNELID::SOUND_PLAYER_FOOTSTEP, 10.f);

}

void CPlayer::Play_Dodge_Sound()
{

	wstring strFileName = L"";
	
	_uint iRand = SMath::Random(0, 9);
	switch (iRand)
	{
	case 0:
		strFileName = L"Player_doge_foley_fast_01.wav";
		break;
	case 1:
		strFileName = L"Player_doge_foley_fast_02.wav";
		break;
	case 2:
		strFileName = L"Player_doge_foley_fast_03.wav";
		break;
	case 3:
		strFileName = L"Player_doge_foley_fast_04.wav";
		break;
	case 4:
		strFileName = L"Player_doge_foley_fast_05.wav";
		break;
	case 5:
		strFileName = L"Player_doge_foley_fast_06.wav";
		break;
	case 6:
		strFileName = L"Player_doge_foley_fast_07.wav";
		break;
	case 7:
		strFileName = L"Player_doge_foley_fast_08.wav";
		break;
	case 8:
		strFileName = L"Player_doge_foley_fast_09.wav";
		break;
	default:
		strFileName = L"Player_doge_foley_fast_01.wav";
		break;
	}
	
	m_pGameInstance->Play_Sound(L"PLAYER_FOOTSTEP", strFileName, CHANNELID::SOUND_PLAYER_FOOTSTEP, 10.f);

}

void CPlayer::Play_Roll_Sound()
{
	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 9);
	switch (iRand)
	{
	case 0:
		strFileName = L"Player_Doge_Full_01.wav";
		break;
	case 1:
		strFileName = L"Player_Doge_Full_02.wav";
		break;
	case 2:
		strFileName = L"Player_Doge_Full_03.wav";
		break;
	case 3:
		strFileName = L"Player_Doge_Full_04.wav";
		break;
	default:
		strFileName = L"Player_Doge_Full_01.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_FOOTSTEP", strFileName, CHANNELID::SOUND_PLAYER_FOOTSTEP, 10.f);

}

void CPlayer::Play_Voice_Melee()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::MELEE);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = 7.f;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 10);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_gauntletattack_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_gauntletattack_jesse_1_ALT01.wav";
		break;
	case 2:
		strFileName = L"oneliner_gauntletattack_jesse_2.wav";
		break;
	case 3:
		strFileName = L"oneliner_gauntletattack_jesse_2_ALT01.wav";
		break;
	case 4:
		strFileName = L"oneliner_gauntletattack_jesse_3.wav";
		break;
	case 5:
		strFileName = L"oneliner_gauntletattack_jesse_3_ALT01.wav";
		break;
	case 6:
		strFileName = L"oneliner_gauntletattack_jesse_3_ALT02.wav";
		break;
	case 7:
		strFileName = L"oneliner_gauntletattack_jesse_4.wav";
		break;
	case 8:
		strFileName = L"oneliner_gauntletattack_jesse_4_ALT01.wav";
		break;
	case 9:
		strFileName = L"oneliner_gauntletattack_jesse_5.wav";
		break;
	default:
		strFileName = L"oneliner_gauntletattack_jesse_1.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Melee_Heavy()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::MELEE_HEAVY);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 3);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_cannonballpunch_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_cannonballpunch_jesse_1_ALT01.wav";
		break;
	case 2:
		strFileName = L"oneliner_cannonballpunch_jesse_1_ALT02.wav";
		break;
	case 3:
		strFileName = L"oneliner_cannonballpunch_jesse_2.wav";
		break;
	case 4:
		strFileName = L"oneliner_cannonballpunch_jesse_2_ALT01.wav";
		break;
	case 5:
		strFileName = L"oneliner_cannonballpunch_jesse_3.wav";
		break;
	case 6:
		strFileName = L"oneliner_cannonballpunch_jesse_3_ALT01.wav";
		break;
	case 7:
		strFileName = L"oneliner_cannonballpunch_jesse_3_ALT02.wav";
		break;
	case 8:
		strFileName = L"oneliner_cannonballpunch_jesse_4.wav";
		break;
	case 9:
		strFileName = L"oneliner_cannonballpunch_jesse_4_ALT01.wav";
		break;
	default:
		strFileName = L"oneliner_cannonballpunch_jesse_1.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Winchester()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::WINCHESTER);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 3);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_perfectdodge_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_perfectdodge_jesse_1_alt.wav";
		break;
	case 2:
		strFileName = L"oneliner_perfectdodge_jesse_1_ALT01.wav";
		break;
	default:
		strFileName = L"oneliner_perfectdodge_jesse_1_ALT01.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Shotgun()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::SHOTGUN);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;

	wstring strFileName = L"oneliner_shotgun_jesse_3_ALT01.wav";
	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Revolver()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::REVOLVER);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;


	//wstring strFileName = L"";

	//_uint iRand = SMath::Random(0, 3);
	//switch (iRand)
	//{
	//case 0:
	//	strFileName = L"oneliner_perfectdodge_jesse_1.wav";
	//	break;
	//case 1:
	//	strFileName = L"oneliner_perfectdodge_jesse_1_alt.wav";
	//	break;
	//case 2:
	//	strFileName = L"oneliner_perfectdodge_jesse_1_ALT01.wav";
	//	break;
	//default:
	//	strFileName = L"oneliner_perfectdodge_jesse_1_ALT01.wav";
	//	break;
	//}

	//m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Zapper_Block()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::ZAPPER_BLOCK);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 9);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_zapperblock_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_zapperblock_jesse_1_ALT01.wav";
		break;
	case 2:
		strFileName = L"oneliner_zapperblock_jesse_2.wav";
		break;
	case 3:
		strFileName = L"oneliner_zapperblock_jesse_2_ALT01.wav";
		break;
	case 4:
		strFileName = L"oneliner_zapperblock_jesse_3.wav";
		break;
	case 5:
		strFileName = L"oneliner_zapperblock_jesse_3_ALT01.wav";
		break;
	case 6:
		strFileName = L"oneliner_zapperblock_jesse_4.wav";
		break;
	case 7:
		strFileName = L"oneliner_zapperblock_jesse_1_ALT01.wav";
		break;
	case 8:
		strFileName = L"oneliner_zapperblock_jesse_2.wav";
		break;
	default:
		strFileName = L"oneliner_perfectdodge_jesse_1_ALT01.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Zapper_Dash()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::ZAPPER_DASH);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 4);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_zapperdash_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_zapperdash_jesse_2.wav";
		break;
	case 2:
		strFileName = L"oneliner_zapperdash_jesse_2_ALT01.wav";
		break;
	case 3:
		strFileName = L"oneliner_zapperdash_jesse_2_ALT02.wav";
		break;
	default:
		strFileName = L"oneliner_zapperdash_jesse_1.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Zapper_Pull()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::ZAPPER_PULL);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 4);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_zapperpull_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_zapperpull_jesse_2.wav";
		break;
	case 2:
		strFileName = L"oneliner_zapperpull_jesse_2_ALT01.wav";
		break;
	case 3:
		strFileName = L"oneliner_zapperpull_jesse_2_ALT02.wav";
		break;
	default:
		strFileName = L"oneliner_zapperpull_jesse_1.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Super_Charge()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::SUPER_CHARGE);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;

	//wstring strFileName = L"oneliner_zapperslam_jesse_1.wav";
	//m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Slam()
{
	if (0.f < m_fVoiceCooltime)
		return;

	_uint iIndex = ECast(Player_Voice_Cooltime::SLAM);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;
	m_fVoiceCooltime = 1.f;

	wstring strFileName = L"oneliner_zapperslam_jesse_1.wav";
	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_E_Combo()
{
	_uint iIndex = ECast(Player_Voice_Cooltime::E_COMBO);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;

	//wstring strFileName = L"oneliner_zapperslam_jesse_1.wav";
	//m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Play_Voice_Heal()
{
	_uint iIndex = ECast(Player_Voice_Cooltime::HEAL);
	if (0.f < m_VoiceCooltime[iIndex])
		return;

	m_VoiceCooltime[iIndex] = PLAYER_VOICE_DELAY;


	wstring strFileName = L"";

	_uint iRand = SMath::Random(0, 6);
	switch (iRand)
	{
	case 0:
		strFileName = L"oneliner_healingkit_jesse_1.wav";
		break;
	case 1:
		strFileName = L"oneliner_healingkit_jesse_1_ALT01.wav";
		break;
	case 2:
		strFileName = L"oneliner_healingkit_jesse_2.wav";
		break;
	case 3:
		strFileName = L"oneliner_healingkit_jesse_2_ALT01.wav";
		break;
	case 4:
		strFileName = L"oneliner_healingkit_jesse_2_ALT02.wav";
		break;
	case 5:
		strFileName = L"oneliner_healingkit_jesse_2_ALT03.wav";
		break;
	default:
		strFileName = L"oneliner_healingkit_jesse_1.wav";
		break;
	}

	m_pGameInstance->Play_Sound(L"PLAYER_VOICE", strFileName, CHANNELID::SOUND_PLAYER_VOICE, 10.f);

}

void CPlayer::Update_Voice_Cooltime(_float fTimeDelta)
{
	_float fResult = m_fVoiceCooltime - fTimeDelta;
	m_fVoiceCooltime = fResult > 0.f ? fResult : 0.f;
	
	for (_uint i = 0; i < (_uint)ECast(Player_Voice_Cooltime::Player_Voice_Cooltime_End); ++i)
	{
		_float fResult = m_VoiceCooltime[i] - fTimeDelta;
		m_VoiceCooltime[i] = fResult > 0.f ? fResult : 0.f;
	}
}

void CPlayer::Set_BodyRender(_uint iOption)
{
	switch (iOption)
	{
	case 0:
		dynamic_cast<CBody_Player*>(m_pBody)->Set_PlayerRender(CBody_Player::RENDER_PASS::RENDER_ORIGIN);
		break;
	case 1:
		dynamic_cast<CBody_Player*>(m_pBody)->Set_PlayerRender(CBody_Player::RENDER_PASS::RENDER_HEAL);
		break;
	case 2:
		dynamic_cast<CBody_Player*>(m_pBody)->Set_PlayerRender(CBody_Player::RENDER_PASS::RENDER_SUPERCHARGE);
		break;
	case 3:
		dynamic_cast<CBody_Player*>(m_pBody)->Set_PlayerRender(CBody_Player::RENDER_PASS::RENDER_SNOWMOUNTAIN);
		break;
	}
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Pool()
{
	return new CPlayer(*this);
}

void CPlayer::Free()
{
	__super::Free();

	if (m_pActor)
	{
		Safe_Delete(m_pActor);
	}

	if (nullptr != m_pEffect)
	{
		m_pEffect->Delete_Object_Owner();
		Safe_Release(m_pEffect);
	}


	if (nullptr != m_pChargeEffect)
	{
		m_pChargeEffect->Delete_Object_Owner();
		Safe_Release(m_pChargeEffect);
	}

}
