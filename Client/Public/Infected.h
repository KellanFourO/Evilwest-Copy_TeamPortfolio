#pragma once

#include "Client_Defines.h"
#include "Monster_Character.h"
#include "Actor.h"

BEGIN(Client)

/* �� : Character/Gores/Blow_Up �� �ִ� �� : ��� �¾Ƽ� �Ǻ� �޽� ��ܳ��� �޽� -> �������ϴ°� ������.. �갡 �ϸ� ���ֵ��� �� �ؾ���. */
/* Memo */
// Body, Weapon ���� Ŭ������ Monster�� ���� ���� �ƴ϶� �ٷ� CBody , CWeapon ��ӹ޾Ƽ� �����

class CInfected abstract : public CMonster_Character
{
public:
	enum class	INFECTED_TYPE {
	INFECTED_VESSEL_A, INFECTED_VESSEL_B, INFECTED_VESSEL_C, 
	INFECTED_PROTEUS, INFECTED_WASTER, INFECTED_END };

	typedef struct tagDesc
	{
		INFECTED_TYPE eType = INFECTED_TYPE::INFECTED_END;
			
		_float fAttack_Distance = 0.f; // 0 ~ Attack = ���ݻ�Ÿ� 
		_float fWalk_Distance = 0.f;   // Attack ~ Walk = walk Animation���� ���� 
								 // Walk Dist < ���� ���� Ŭ��� Run -> Attack �����ϸ� ����    
		_float WalkSpeed = 0.f;
		_float RunSpeed = 0.f;

		_int RandomNumber = 0; // SMath::Random(1, 10);

		//m_eInfo.fAttack_Distance = 3.5f;
		//m_eInfo.fWalk_Distance = 10.f;
	public:
		tagDesc()
		{
			ZeroMemory(this, sizeof(tagDesc));
		}
		INFECTED_TYPE Get_Type() { return eType; }
	}INFECTED_DESC;


protected:
	CInfected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CInfected(const CInfected& rhs);
	virtual ~CInfected() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	INFECTED_DESC Get_Info() { return m_eInfo; }
	virtual HRESULT Update_RenderState(_int CurrentHP) PURE;
	void	Set_CntDead_Time(_float CountDown) { m_fCntDead_Time = CountDown; }
	void	Set_CntDead_Active(_bool bActive) { m_bCntDead_Active = bActive; }

public:
	virtual void Explosion() {};


protected:
	HRESULT Ready_Components();
	_bool	m_bCntDead_Active = { false };
	_float	m_fCntDead_Time = {};
	_float fTimeAcc = 0.f;

protected:
	virtual void Hitted_Left(Power ePower)	override;	// (�÷��̾� ����)�������� ���� ��Ÿ 
	virtual void Hitted_Right(Power ePower) override;	// (�÷��̾� ����)���������� ���� ��Ÿ 
	virtual void Hitted_Front(Power ePower) override;	// (�÷��̾� ����)�������� ���� ��Ÿ 
	
	/* �÷��̾ ���� �ǰ� */
	virtual void Hitted_Knock(_bool bIsCannonball) override;	// �÷��̾� ĳ�� ��ų
	virtual void Hitted_Electrocute()	override;				// ������ų�°� ��� �Ÿ��� �������� 
	virtual void Hitted_OpenState_Pull() override;				// �÷��̾� ������½�ų?
	virtual void Hitted_Opened(Direction eDirection) override;  // ������� ���ڸ����� �ĸ´¾ִϸ��̼� 
	virtual void Hitted_KnockUp() override;						// �ۿø��� ��ų �¾����� 

	virtual void Hitted_Dead(Power ePower)	override;	// HP�� ��Ƽ� ������ 

protected:
	INFECTED_DESC	m_eInfo = {};
	
private:
	CActor<CInfected>* m_pActor = { nullptr };


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual CGameObject* Pool() PURE;

	virtual void Free() override;

public:
	enum Infected_State
	{
		Infected_Melee_RD_01					 , // (0) ���ڸ� ���� - �Ѽ� �ۿø��� 
		Infected_Melee_RM_01					 , // (1) ���ڸ� ���� - �Ѽ� �������
		Infected_Melee_RM_02					 , // (2) ���ڸ� ���� - ��� ���� - ���ʿ��� ����� 
		Infected_Melee_RU_01					 , // (3) ���ڸ� ���� - �Ѽ� ������� 
		Infected_Melee_RU_02					 , // (4) ���ڸ� ���� - ��� ���� - ��ո��� 
		Infected_MeleeDynamic_RU_01				 , // (5) �̵����� / ª�� �Ÿ� - �Ѽ� ������� 
		Infected_MeleeDynamic_RU_02				 , // (6) �̵����� / ª�� �Ÿ� - �Ѽ� �������
		Infected_MeleeDynamic_RU_03				 , // (7) �̵����� / ª�� �Ÿ� - ��� ������� 
		Infected_Sprint_F_Melee_02				 , // (8) �̵����� / �� �Ÿ� - �Ѽ� ������� 
		Infected_Sprint_F_Melee_03				 , // (9) �̵����� / �� �Ÿ� - �Ѽ� �������
		Infected_Idle							 , // (10) Idle / �׳� ���ֱ⸸�� 
		Infected_IdleAct_01						 , // (11) Idle / ���� ���帮����� �ϴ� Idle ��Ʋ�Ÿ�
		Infected_IdleAct_02						 , // (12) Idle / �����ؼ� �ణ ��Ʋ�Ÿ������� 
		Infected_IdleAct_03						 , // (13) Idle / ���ڸ����� �ֺ� �ѷ����� 
		Infected_IdlePose						 , // (14) Idle / �ȿ����̰� ���������� - �Ⱦ��¿뵵�ε� ?
		Infected_InteractionVault200			 , // (15) Dynamic / �ڱ� Ű���� ������ �� �Ѿ
		Infected_JumpDown_200_forward			 , // (16) ���������� �پ���� ��������η����ϰ� �Ͼ����� 
		Infected_RelaxedIdleAct_01				 , // (17) Idle / ������ ���Ƽ� ������� �ϰ� �ٽ� ������� ���ƿ�
		Infected_RelaxedIdleAct_02				 , // (18) Idle / ���� ���� ������ �ڱ��� �Ĵٺ����ڿ� ���ƿ�
		Infected_RelaxedIdleAct_03				 , // (19) Idle / �ڱ� �ܵ���� Ȯ����
		Infected_Run_B							 , // (20) Move / ������ �ڷ� ������
		Infected_Run_F							 , // (21) Move / ������ ������ ������
		Infected_Run_FL							 , // (22) Move / ������ ���ʷ� ������(���ͱ���) + �ٶ󺸱� 
		Infected_Run_FL45						 , // (23) Move / ������ ���� �밢�� ������(���ͱ���)	
		Infected_Run_FR							 , // (24) Move / ������ �����ʷ� ������(���ͱ���) + �ٶ󺸱� 
		Infected_Run_FR45						 , // (25) Move / ������ ������ �밢�� ������(���ͱ���)
		Infected_SpawnClimb_01					 , // (26) Spawn / �Ϲ������� �ö��
		Infected_SpawnClimb_02					 , // (27) Spawn / ������ �ձ������ϰ� �ö��
		Infected_SpawnCrawl_01					 , // (28) Spawn / ������ �����鼭 �ö��
		Infected_SpawnCrawl_02					 , // (29) Spawn / ������ �����鼭 �ö��(2) - ���̸𸣰ڴ�
		Infected_SpawnFromCoffin0				 , // (30) Spawn / �ϴú��� �����ִٰ� �Ͼ�鼭 �ö��
		Infected_SpawnFromCoffin65				 , // (31) Spawn / ������ �ɷ��� ������ ����
 		Infected_SpawnFromCoffin90				 , // (32) Dynamic / ��� ������ �����̱�  
		Infected_SpawnFromEgg_01				 , // (33) Spawn / �������� �����ϰ� �̻��ϰ� ���� 
		Infected_SpawnFromEgg_02				 , // (34) Spawn / ���ڸ� �Ͼ�鼭 ���� 
		Infected_SpawnJumpDown_01				 , // (35) Spawn / �������� �����ϸ鼭 ��Ÿ�� 
		Infected_SpawnJumpOut_01				 , // (36) Spawn / ���� ¤���鼭 ��Ÿ�� 
		Infected_SpawnJumpOut_03				 , // (37) Spawn / ���� ¤���鼭 ��Ÿ�� 
		Infected_SpecialIdle					 , // (38) Idle / �ٴڿ� �ɾ�����
		Infected_SpecialIdle_to_Idle			 , // (39) Idle / �ٴڿ� �ɾ��ִٰ� �Ͼ 
		Infected_Sprint_F_01					 , // (40) Move / ������ �μյ�� �޷��� 
		Infected_Taunt_01						 , // (41) Move / �μյ�� ����ó�� �ٰ��� 
		Infected_Taunt_02						 , // (42) Move / ���� 180�� ȸ�� 
		Infected_Turn_L180						 , // (43) Move / ���� 180�� ȸ�� 
		Infected_Turn_L90						 , // (44) Move / ���� 90�� ȸ�� 
		Infected_Turn_R180						 , // (45) Move / ������ 180�� ȸ��
		Infected_Turn_R90						 , // (46) Move / ������ 90�� ȸ��
		Infected_Walk_B							 , // (47) Move / õõ�� �ް����� 
		Infected_Walk_F							 , // (48) Move / õõ�� ������ �̵� 
		Infected_Walk_FL						 , // (49) Move / õõ�� (���ͱ���) ���� �ٶ󺸸鼭 �̵� 
		Infected_Walk_FL45						 , // (50) Move / õõ�� (���ͱ���) ���� �밢�� �̵� 
		Infected_Walk_FR						 , // (51) Move / õõ�� (���ͱ���) ������ �ٶ󺸸鼭 �̵�
		Infected_Walk_FR45						 , // (52) Move / õõ�� (���ͱ���) ������ �밢�� �̵� 
		Infected_DeathHeavy_F_01_NEW			 , // (53) Dead / �ڷ� �ʴ��ϰ� �����鼭 ����
		Infected_DeathLight_B_01_NEW			 , // (54) Dead / ���°� �������� �� �����鼭 ������ ��ٶ���
		Infected_DeathLight_F_01_NEW			 , // (55) Dead / ���°� ���������� �� �����鼭 �ڷ� ��ٶ���
		Infected_HitHeavy_B_01_NEW				 , // (56) Hit / ������ �Ѿ����� �ϴٰ� �ȳѾ����� �̵��ϴٰ� �ڵ��ƺ�
		Infected_HitHeavy_BL_01_NEW				 , // (57) Hit / �޻��°� ������ �̵� - �ȳѾ����� �ڵ��ƺ�
		Infected_HitHeavy_BR_01_NEW				 , // (58) Hit / ������ �� �°� ������ �̵� - �ȳѾ����� �ڵ��ƺ�
		Infected_HitHeavy_F_01_NEW				 , // (59) Hit / �θ°� �ް�����ġ�鼭 �ڷ� �̵� 
		Infected_HitHeavy_F_02_NEW				 , // (60) Hit / �θ°� �ް�����ġ�鼭 �ڷ� �̵� - ���� ��� ������?
		Infected_HitHeavy_FL90_01_NEW			 , // (61) Hit / ���ʰ���� �°� ���������� ���̵� ���� 
		Infected_HitHeavy_FL_01_NEW				 , // (62) Hit / ������ ���°� �ް����� - Ǯ�Ŀ��ε�
		Infected_HitHeavy_FL_02_NEW				 , // (63) Hit / ������ ���°� �ް����� - �� Ǯ�Ŀ��ε�
		Infected_HitHeavy_FR90_01_NEW			 , // (64) Hit / ������ ����� �°� �������� ���̵� ����
		Infected_HitHeavy_FR_01_NEW				 , // (65) Hit / ���� �� �°� �ް����� - Ǯ�Ŀ��ε� 
		Infected_HitHeavy_FR_02_NEW				 , // (66) Hit / ���� ���°� �ް����� - �� Ǯ�Ŀ��ε�
		Infected_HitLight_B_01_NEW				 , // (67) Hit / ���x �޽� �������� 
		Infected_HitLight_BL_01_NEW				 , // (68) Hit / ���x �޽� �������� 
		Infected_HitLight_BR_01_NEW				 , // (69) Hit / ���x �޽� �������� 
		Infected_HitLight_F_01_NEW				 , // (70) Hit / ���x �޽� �������� 
		Infected_HitLight_FL_01_NEW				 , // (71) Hit / ���x �޽� �������� 
		Infected_HitLight_FR_01_NEW				 , // (72) Hit / ���x �޽� �������� 
		Infected_HitLight_L_01_NEW				 , // (73) Hit / ���x �޽� �������� 
		Infected_HitLight_R_01_NEW				 , // (74) Hit / ���x �޽� �������� 
		Infected_HitLightOpened_F_01_NEW		 , // (75) Hit / ���x �޽� �������� 
		Infected_HitNormal_B_01_NEW				 , // (76) Hit / ���x �޽� �������� 
		Infected_HitNormal_BL_01_NEW			 , // (77) Hit / ���x �޽� �������� 
		Infected_HitNormal_BR_01_NEW			 , // (78) Attack / ���̰� �׷��ñ��� �ϰ� ������ ���� ������ �ڵ��ƺ�
		Infected_HitNormal_F_01_NEW				 , // (79) Hit / ��¦ �޻� ������ó�� �ϰ� ������ �̵��ؼ� �ڵ��ƺ�
		Infected_HitNormal_F_02_NEW				 , // (80) Hit / ���ְ��� �ΰ��� �ڷ� 
		Infected_HitNormal_FL90_01_NEW			 , // (81) Hit / �ƿ� �ϰ� �㸮������ ���������� �̵� 
		Infected_HitNormal_FL_01_NEW			 , // (82) Hit / ��¦ �޻� ������ó�� �ϰ� �ڷ� �̵�
		Infected_HitNormal_FR90_01_NEW			 , // (83) Hit / ��¦ ������ ������ó�� �ϰ� ���ʷ� �̵�
		Infected_HitNormal_FR_01_NEW			 , // (84) Hit / ������ �� �°� �ڷ� �̵� 
		Infected_KnockFrontLight_B_01_NEW		 , // (85) KnockDown / ���ڸ����� ������ �������� �Ͼ����� 
		Infected_KnockFrontLight_F_01_NEW		 , // (86) KnockDown / ���� ���°� �ڷ� �ں����� �Ͼ����� 
		Infected_Scared_01						 , // (87) KnockDown / ���� ���ݴ��ؼ� ���帮�鼭 �ڷ� ��������
		Infected_Scared_02						 , // (88) KnockDown / �������°� �� ����ä�� ������ �ϸ鼭 �� �����鼭 �ް����� 
		Infected_Scared_03						 , // (89) KnockDown / �޻��°� �� ����ä�� ������ �ϸ鼭 �� �����鼭 �ް����� 
		Infected_Scared_04						 , // (90) KnockDown / �������°� ��� �ϰ� �Ȼ���ä�� �ް����� 
		Infected_Burn_Loop						 , // (91) KnockDown / ��Ʋ��Ʋ �����Ѱ� + �ƴ� �����ߴٰ� 
		Infected_Electrocute_Loop				 , // (92) Electric / �����Ǽ� ������ �ϴ��� 
		Infected_HitHeavyToStun_01_NEW			 , // (93) KnockDown / ���ְ��� �°� �ڷ� �̵� 
		Infected_HitNormalToStun_01_NEW			 , // (94) KnockDown / �޻� ���� ���̸°� �ణ ������ 
		Infected_Stun_Loop_NEW					 , // (95) Electric / Stun���� ���� ���� ������ ����  
		Infected_Dodge_F_01_TEMP				 , // (96) Dodge / ������ ���ڱ� Ƣ��� 
		Infected_Dodge_L_01_TEMP				 , // (97) Dodge / �������� ���ڱ� Ƣ��� 
		Infected_Dodge_R_01_TEMP				 , // (98) Dodge / ���������� ���ڱ� Ƣ��� 
		Infected_Hit_whip_end_down				 , // (99) KnockDown / ���ڷ� ���߿��� �������ٰ� �ٴڿ� ��Ƽ� �� �ϰ� �Ͼ 
		Infected_HitLightOpened_L_01			 , // (100) Hit / �������� ��Ƽ�� ������ ù���� ��Ÿ�� �¾����� 
		Infected_HitLightOpened_R_01			 , // (101) Hit / ���������� ��Ƽ�� ������ ù���� ��Ÿ�� �¾����� 
		Infected_HitNormal_F_03_NEW				 , // (102) Hit / �θ°� �ڷ� �ΰ���
		Infected_KnockFrontCannonball_F_01_TEMP	 , // (103) Hit / ĳ�� �°� �ڷ� ���� 
		Infected_MeleeDynamic_Dodge_B_4m		 , // (104) Dodge / �ڷ� ��û �ָ� ȸ���� 
		Infected_OpenStatePull_F_01				 , // (105) Move / ����? ���� �����̴µ��� ������ 
		Infected_OpenStateTimeout				 , // (106) Hit / ������ ��� � 
		Infected_SpawnGround					 , // (107) Spawn / ���ӿ��� ���� 
		Infected_VortexGrenade_Loop				 , // (108) Hit / �ҿ뵹�� ��ź ??? ���������� 
		Infected_VortexGrenade_Stop				 , // (109) Hit / �ҿ뵹�� ��ź�� ������ ������ �۷����°��ε�? 
		Infected_KnockUp_Low_Fixed				
	};
};

END

