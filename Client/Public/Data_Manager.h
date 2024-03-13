#pragma once
#include "Client_Defines.h"
#include "Engine_Macro.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CCamera;
class CNavigation;
END


BEGIN(Client)

class CPlayer;
class CCamera_Dynamic;
class CMasterCamera;
class CSky;

class CData_Manager : public CBase
{
	DECLARE_SINGLETON(CData_Manager);

private:
	CData_Manager();
	virtual ~CData_Manager() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	
public: 
	void Set_Player(CPlayer* _pPlayer);
	CPlayer* Get_Player();
	void Reset_Player(LEVEL eLEVEL);

public:
	void Set_MasterCamera(CMasterCamera* _pMasterCamera);
	CMasterCamera* Get_MasterCamera();
	void Reset_MasterCamera(LEVEL eLEVEL);

public:
	CSky*	Get_pSkyBox() { return m_pSky; }
	void	Set_pSkybox(CSky* _pSkybox);

	_uint	Get_SkyTextureCount();
	_uint	Get_SkyCurrentTextureIndex();

	_uint   Get_CurrentSkyType();
	void	Set_SkyType(_uint iCastSkyType);

	void    Set_SkyTextureIndex(_uint iSkyTextureIndex);

public:
	CNavigation* Get_Navigation() { return m_pNavigation; }
	void		 Set_Navigation(CNavigation* pNavigation) { m_pNavigation = pNavigation; }

// ! SH_Add
private: /* _float */
	_float	m_fMaxHP = 100.f;
	_float	m_fCurHP = 100.f;

	_float	m_fMaxEXP = 100.f;
	_float	m_fCurEXP = 0.f;

	_float	m_fMaxSkillGuige = 100.f;
	_float	m_fCurSkillGuige = 0.f;

private: /* _int */
	_int	m_iCurLevel = 1;

private: /* _bool */
	_bool	m_bLevelUp = false;
	_bool	m_bShowLevelBox = false;
	_bool	m_bShowInterface = false;
	_bool	m_bShowTutorial = false;
	_bool	m_bShowQuestBox = false;
	_bool	m_bShowRewardBox = false;


// ! SH_Add
public: /* ========================== Player Info ========================== */
	void	PlayerInfo_Setting();	// Player Setting
	void	Limit_Manager();		// Limit_Manager
#pragma region =========> MAX_HP <=========
	// => HP (Limit)
	void	Limit_HP();
	// =>�ִ�ü�� Get�Լ�
	_float	Get_MaxHP() { return m_fMaxHP; }
	// =>�ִ�ü�� Set�Լ�
	void	Set_MaxHP(_float fMaxHP) { m_fMaxHP = fMaxHP; }
	// =>�ִ�ü�� Add�Լ�
	void	Add_MaxHP(_float fAddMaxHP) { m_fMaxHP += fAddMaxHP; }
#pragma region =========> CUR_HP <=========
	// =>����ü�� Get�Լ�
	_float	Get_CurHP() { return m_fCurHP; }
	// =>����ü�� Set�Լ�
	void	Set_CurHP(_float fCurHP) { m_fCurHP = fCurHP; }
	// =>����ü�� Add�Լ�
	void	Add_CurHP(_float fAddCurHP) { m_fCurHP += fAddCurHP; }
#pragma region =========> MAX_EXP <=========
	// =>Player_EXP(Limit)
	_bool	Limit_EXP();
	// =>�ִ����ġ Get�Լ�
	_float	Get_MaxEXP() { return m_fMaxEXP; }
	// =>�ִ����ġ Set�Լ�
	void	Set_MaxEXP(_float fMaxEXP) { m_fMaxEXP = fMaxEXP; }
	// =>�ִ����ġ Add�Լ�
	void	Add_MaxEXP(_float fAddMaxEXP) { m_fMaxEXP += fAddMaxEXP; }
	// =>LevelUp�� ������ �ִ����ġ�� ������Ű�� �Լ�
	void	NextLevl_MaxEXP() { m_fMaxEXP = m_fMaxEXP + (m_fMaxEXP / 3); }
#pragma region =========> CUR_EXP <=========
	// =>���� ����ġ Get�Լ�
	_float	Get_CurEXP() { return m_fCurEXP; }
	// =>���� ����ġ Set�Լ�
	void	Set_CurEXP(_float fCurEXP) { m_fCurEXP = fCurEXP; }
	// =>���� ����ġ Add�Լ�
	void	Add_CurEXP(_float fAddCurEXP) { m_fCurEXP += fAddCurEXP; }
#pragma region =========> MAX_SKILLGUIGE <=========
	// =>SkillGuige (Limit)
	void	Limit_SkillGuige();
	// =>�ƽ� ��ų������ Get�Լ�
	_float	Get_MaxSkillGuige() { return m_fMaxSkillGuige; }
	// =>�ƽ� ��ų������ Set�Լ�
	void	Set_MaxSkillGuige(_float fMaxSkillGuige) { m_fMaxSkillGuige = fMaxSkillGuige; }
	// =>�ƽ� ��ų������ Add�Լ�
	void	Add_MaxSkillGuige(_float fAddMaxSkillGuige) { m_fMaxSkillGuige += fAddMaxSkillGuige; }
#pragma region =========> CUR_SKILLGUIGE <=========
	// =>���� ��ų������ Get�Լ�
	_float	Get_CurSkillGuige() { return m_fCurSkillGuige; }
	// =>���� ��ų������ Set�Լ�
	void	Set_CurSkillGuige(_float fCurSkillGuige) { m_fCurSkillGuige = fCurSkillGuige; }
	// =>���� ��ų������ Add�Լ�
	void	Add_CurSkillGuige(_float fAddCurSkillGuige) { m_fCurSkillGuige += fAddCurSkillGuige; }
#pragma region =========> PLAYER_LEVEL <=========
	// =>LevelUP (Limit)
	void	Limit_Level();
	// =>���� ���� Get�Լ�
	_int	Get_CurLevel() { return m_iCurLevel; }
	// =>���� ���� Set�Լ�
	void	Set_CurLevel(_int iCurLevel) { m_iCurLevel = m_iCurLevel; }
	// =>���� ���� Add�Լ�
	void	Add_CurLevel(_int fAddCurLevel) { m_iCurLevel += fAddCurLevel; }
	// =>LevelUP_UI�� �������� ���� �����ϴ� Get�Լ�
	_bool	Get_ShowLevelBox() { return m_bShowLevelBox; }
	// =>LevelUP_UI�� �������� ���� ������ �� �ִ� Set�Լ�
	void	Set_ShowLevelBox(_float bShowLevelBox) { m_bShowLevelBox = bShowLevelBox; }
#pragma region =========> HUD(Interface) <=========
	// =>PlayerHUD�� �������� ���� �����ϴ� Get�Լ�
	_bool	Get_ShowInterface() { return m_bShowInterface; };
	// =>PlayerHUD�� �������� ���� ������ �� �ִ� Set�Լ�
	void	Set_ShowInterface(_bool bShowInterface) { m_bShowInterface = bShowInterface; };
#pragma region =========> TutorialBox <=========
	// =>TutorialBox�� �������� ���� �����ϴ� Get�Լ�
	_bool	Get_ShowTutorialBox() { return m_bShowTutorial; };
	// =>TutorialBox�� �������� ���� ������ �� �ִ� Set�Լ�
	void	Set_ShowTutorialBox(_bool bShowTutorial) { m_bShowTutorial = bShowTutorial; };
#pragma region =========> QuestBox <=========
	// =>QuestBox�� �������� ���� �����ϴ� Get�Լ�
	_bool	Get_ShowQuestBox() { return m_bShowQuestBox; };
	// =>QuestBox�� �������� ���� ������ �� �ִ� Set�Լ�
	void	Set_ShowQuestBox(_bool bShowQuestBox) { m_bShowQuestBox = bShowQuestBox; };
#pragma region =========> RewardBox <=========
	// =>RewardBox�� �������� ���� �����ϴ� Get�Լ�
	_bool	Get_ShowRewardBox() { return m_bShowRewardBox; };
	// =>RewardBox�� �������� ���� ������ �� �ִ� Set�Լ�
	void	Set_ShowRewardBox(_bool bShowRewardBox) { m_bShowRewardBox = bShowRewardBox; };

private:
	CMasterCamera* m_pMasterCamera = { nullptr };
	CPlayer* m_pPlayer = { nullptr };

	CSky* m_pSky = { nullptr };
	CNavigation* m_pNavigation = { nullptr };

private:
	CCamera* m_pCamera = { nullptr };

	
private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };



public:
	virtual void	Free();
};

END

