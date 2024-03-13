#pragma once

#include "Client_Defines.h"
#include "Monster_Character.h"
#include "Actor.h"

BEGIN(Client)

class CBandit_Sniper final : public CMonster_Character
{

private:
	CBandit_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CBandit_Sniper(const CBandit_Sniper& rhs);
	virtual ~CBandit_Sniper() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
public:
	_bool Get_ProtectExist() { return m_bProtectExist; }
	_float3 Get_BonePos() { return m_pWeapon->Get_Position(); }

	/* For. Bullet */
	_int Get_BulletCnt() { return m_iBulletCnt; }
	void Set_BulletCnt(_int iCnt) { m_iBulletCnt = iCnt; }
	void Add_BulletCnt() { m_iBulletCnt += 1; }


private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Ready_Option();

private:
	_bool					m_bProtectExist		= {};
	_bool					m_bTaunt = false;
	_int					m_iBulletCnt = 0;

	CActor<CBandit_Sniper>* m_pActor			= { nullptr };
	CWeapon*				m_pWeapon			= { nullptr };

public:
	virtual void Hitted_Left(Power ePower)	override;
	virtual void Hitted_Right(Power ePower) override;
	virtual void Hitted_Front(Power ePower) override;
	virtual void Hitted_Knock(_bool bIsCannonball) override;
	virtual void Hitted_Dead(Power ePower)	override;

public:
	static CBandit_Sniper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;
	virtual void Free() override;

public:
	enum Bandit_Sniper_State
	{
		Sniper_Ironsights_Reload					 , //  (0) Reload - �ϳ��� �� 5�� ������
		Sniper_Melee_01								 , //  (1) (Melee)CloseAttack - �� �޼տ� ��� ���������� ���ڸ� �ֵθ��� 
		Sniper_Melee_02								 , //  (2) (Melee)CloseAttack - �� �޼տ� ��� ���������� ���ڸ� �ֵθ���(��)
		Sniper_Melee_03								 , //  (3) (Melee)CloseAttack - �ѵ�� �߷����� 
		Sniper_MeleeDynamic_01						 , //  (4) (Melee)CloseAttack - �ѹ߳����� �� �޼տ� ��� ���������� �ֵθ��� 
		Sniper_MeleeDynamic_02						 , //  (5) (Melee)CloseAttack - �ѹ߳����� ���� ������� ��� �ѽ����� ������ 
		Sniper_MeleeDynamic_03						 , //  (6) (Melee)CloseAttack - �ѹ� ������ ���� ������� ��� �߷� ���� 
		Sniper_CoverHigh_Idle						 , //  (7) (Cover)Sniping - ���� ������� ��ä ���ڸ����� ������ 
		Sniper_CoverHigh_Reload						 , //  (8) (Cover)Sniping - ������ó�� �ѹ��� ������ ������ 
		Sniper_CoverHigh_Side_L_AimPose				 , //  (9) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(2)   - �ܴ����� (����)
		Sniper_CoverHigh_Side_L_Start				 , // (10) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(1)   - ��¦ �̵�
		Sniper_CoverHigh_Side_L_Stop				 , // (11) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(3)   - ���ڸ���
		Sniper_CoverHigh_Side_R_AimPose				 , // (12) (Cover)Sniping - ���� ���� ���� ���������� �ѽ�� ���ᵿ��(2) - �ܴ����� (����)
		Sniper_CoverHigh_Side_R_Start				 , // (13) (Cover)Sniping - ���� ���� ���� ���������� �ѽ�� ���ᵿ��(1) - ��¦ �̵�
		Sniper_CoverHigh_Side_R_Stop				 , // (14) (Cover)Sniping - ���� ���� ���� ���������� �ѽ�� ���ᵿ��(3) - ���ڸ���
		Sniper_CoverLow_Idle						 , // (15) (Cover)Sniping - ���׸��� ������
		Sniper_CoverLow_Over_AimPose				 , // (16) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(2) - �ܴ����� (����)
		Sniper_CoverLow_Over_Start					 , // (17) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(1) - ��¦ �̵�
		Sniper_CoverLow_Over_Stop					 , // (18) (Cover)Sniping - ���� ���� ���� �������� �ѽ�� ���ᵿ��(3) - ���ڸ���
		Sniper_CoverLow_Reload						 , // (19) (Cover)Sniping - �������¿��� ������ó�� �ټ��� ������
		Sniper_CoverLow_Side_L_AimPose				 , // (20) (Cover)Sniping - ���� ���� �ɾ��ִٰ� �������� ���� �ѽ�� ���ᵿ��(2)   - �ܴ����� (����)
		Sniper_CoverLow_Side_L_Start				 , // (21) (Cover)Sniping - ���� ���� �ɾ��ִٰ� �������� ���� �ѽ�� ���ᵿ��(1)   - ��¦ �̵�
		Sniper_CoverLow_Side_L_Stop					 , // (22) (Cover)Sniping - ���� ���� �ɾ��ִٰ� �������� ���� �ѽ�� ���ᵿ��(3)   - ���ڸ���
		Sniper_CoverLow_Side_R_AimPose				 , // (23) (Cover)Sniping - ���� ���� �ɾ��ִٰ� ���������� ���� �ѽ�� ���ᵿ��(2)   - �ܴ����� (����)
		Sniper_CoverLow_Side_R_Start				 , // (24) (Cover)Sniping - ���� ���� �ɾ��ִٰ� ���������� ���� �ѽ�� ���ᵿ��(1)   - ��¦ �̵�
		Sniper_CoverLow_Side_R_Stop					 , // (25) (Cover)Sniping - ���� ���� �ɾ��ִٰ� ���������� ���� �ѽ�� ���ᵿ��(3)   - ���ڸ���
		Sniper_Crouch_Idle							 , // (26) (Cover)Sniping - ���� ���� ���ִٰ� �������� �ɾƼ� �ѽ�� ���ᵿ��(2)   - �ܴ����� (����)
		Sniper_Crouch_Start							 , // (27) (Cover)Sniping - ���� ���� ���ִٰ� �������� �ɾƼ� �ѽ�� ���ᵿ��(1)   - ��¦ �̵�
		Sniper_Crouch_Stop							 , // (28) (Cover)Sniping - ���� ���� ���ִٰ� �������� �ɾƼ� �ѽ�� ���ᵿ��(3)   - ���ڸ���
		Sniper_Crouch_Turn_90_L						 , // (29) (Cover)Sniping - �ɾƼ� �ܴ��� �ִٰ� �������� 90�� ȸ�� 
		Sniper_Crouch_Turn_90_R						 , // (30) (Cover)Sniping - �ɾƼ� �ܴ��� �ִٰ� ���������� 90�� ȸ�� 
		Sniper_Idle									 , // (31) Idle - ���� ��������
		Sniper_IdlePose								 , // (32) Idle - 2������¥�� �߰������ 
		Sniper_Run_B								 , // (33) Movement - �ڷ� �޸��� 
		Sniper_Run_BL								 , // (34) Movement - ������ �ް������� �޸��� 
		Sniper_Run_BR								 , // (35) Movement - �������� �ް������� �޸���
		Sniper_Run_F								 , // (36) Movement - ���� �޸��� 
		Sniper_Run_FL								 , // (37) Movement - ���� �޸��� 
		Sniper_Run_FR								 , // (38) Movement - ������ �޸��� 
		Sniper_Sprint_F								 , // (39) Movement - ���� ���� 
		Sniper_Taunt_01								 , // (40) (Cover)Sniping - �ѽ�ڿ� �ϴ� ����(1)
		Sniper_Taunt_02								 , // (41) (Cover)Sniping - �ѽ�ڿ� �ϴ� ����(2)
		Sniper_Taunt_03								 , // (42) (Cover)Sniping - �ѽ�ڿ� �ϴ� ����(3) - �߾Ⱥ���
		Sniper_Turn_L180							 , // (43) Movement - �������� 180�� �ڵ��� 
		Sniper_Turn_L90								 , // (44) Movement - �������� 90�� ȸ�� 
		Sniper_Turn_R180							 , // (45) Movement - ���������� 180�� �ڵ��� 
		Sniper_Turn_R90								 , // (46) Movement - ���������� 90�� ȸ��
		Sniper_Walk_B								 , // (47) Movement - �ڷ� �ȱ� 
		Sniper_Walk_BL								 , // (48) Movement - �ް������� �������� �ȱ� 
		Sniper_Walk_BR								 , // (49) Movement - �ް������� ���������� �ȱ� 
		Sniper_Walk_F								 , // (50) Movement - �������� �ȱ� 
		Sniper_Walk_FL								 , // (51) Movement - �������� �ȱ� 
		Sniper_Walk_FR								 , // (52) Movement - ���������� �ȱ� 
		Sniper_WalkAim_B							 , // (53) Movement - ���� ������ä �ڷ� �ȱ� 
		Sniper_WalkAim_BL							 , // (54) Movement - ���� ������ä �ް������� �������� �ȱ� 
		Sniper_WalkAim_BR							 , // (55) Movement - ���� ������ä �ް������� ���������� �ȱ� 
		Sniper_WalkAim_F							 , // (56) Movement - ���� ������ä ������ �ȱ� 
		Sniper_WalkAim_FL							 , // (57) Movement - ���� ������ä �������� �ȱ� 
		Sniper_WalkAim_FR							 , // (58) Movement - ���� ������ä ���������� �ȱ� 
		Sniper_DeathLight_B_01						 , // (59) Death - �������� ����
		Sniper_DeathLight_F_01						 , // (60) Death - �ڷ� �������鼭 ����
		Sniper_DeathNormal_B_01						 , // (61) Death - �������� �� ���� �����鼭 ����
		Sniper_DeathNormal_F_01						 , // (62) Death - �ڷ� ������ �����鼭 ����
		Sniper_HitHeavy_B_01						 , // (63) Hit - ������ �������� ���� �̵��ϸ鼭 ���� �̵��ѵ� �ڵ��ƺ� - �������� ����
		Sniper_HitHeavy_BL_01						 , // (64) Hit - ������ �������� ���� �̵��ϸ鼭 ���� �̵��ѵ� �ڵ��ƺ� - �������� ����
		Sniper_HitHeavy_BR_01						 , // (65) Hit - ������ �������� ���� �̵��ϸ鼭 ���� �̵��ѵ� �ڵ��ƺ� - ���������� ����
		Sniper_HitHeavy_F_01						 , // (66) Hit - �ڷ� ���� �з����� - �������� ����
		Sniper_HitHeavy_FL_01						 , // (67) Hit - �ڷ� ���� �з����� - �������� ����
		Sniper_HitHeavy_FR_01						 , // (68) Hit - �ڷ� ���� �з����� - ���������� ����
		Sniper_KnockFrontLight_B_01					 , // (69) Hit - ������ �������� �ٽ� �Ͼ�� �ڵ��ƺ�
		Sniper_KnockFrontLight_F_01					 , // (70) Hit - �������� �¾Ƽ� �ڷ� �Ѿ����� �ٽ� �Ͼ - �������� 
		Sniper_KnockFrontLight_F_02					 , // (71) Hit - ���������� �¾Ƽ� �ڷ� �Ѿ����� �ٽ� �Ͼ - �������� 
		Sniper_Weakspot_Death_01					 , // (72) Death - ������ �����ϴٰ� ���� ���ڸ����� ���� 
		Sniper_Stun_Loop							 , // (73) Hit - �����ѻ���ó�� ��Ʋ��
		Sniper_Burn_Loop_TEMP						 , // (74) Hit - �ҿ� Ÿ����
		Sniper_CoverHigh_LeanLeft_HitHeavy_01_TEMP	 , // (75) ���� �н� 
		Sniper_CoverHigh_LeanRight_HitHeavy_01_TEMP	 , // (76) ���� �н� 
		Sniper_CoverLow_LeanLeft_HitHeavy_01_TEMP	 , // (77) ���� �н� 
		Sniper_CoverLow_LeanOver_HitHeavy_01_TEMP	 , // (78) ���� �н� 
		Sniper_CoverLow_LeanRight_HitHeavy_01_TEMP	 , // (79) ���� �н� 
		Sniper_Electrocute_Loop_TEMP				   // (80) Hit - �÷��̾� �������
	};
};

END







