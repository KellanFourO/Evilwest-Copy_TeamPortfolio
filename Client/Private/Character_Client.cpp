#include "Character_Client.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "SpringCamera.h"
#include "Data_Manager.h"
#include "MasterCamera.h"

CCharacter_Client::CCharacter_Client(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CCharacter(pDevice, pContext, strPrototypeTag)
{
}

CCharacter_Client::CCharacter_Client(const CCharacter_Client& rhs)
	: CCharacter(rhs)
{
}

HRESULT CCharacter_Client::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CCharacter_Client::Initialize(void* pArg)
{
	FAILED_CHECK(__super::Initialize(pArg));

	return S_OK;
}

void CCharacter_Client::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CCharacter_Client::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCharacter_Client::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CCharacter_Client::Render()
{
	FAILED_CHECK(__super::Render());

	return S_OK;
}

CEffect* CCharacter_Client::Create_Effect(CGameObject* pOwner)
{
	string strEffectFileName = Get_CharcterDesc().EffectFileName;
	if ("" == strEffectFileName)
	{
		strEffectFileName = "Hit_Normal";
	}
	return EFFECT_MANAGER->Create_Effect("Hit/", strEffectFileName + ".json", pOwner);
}

CEffect* CCharacter_Client::Create_Effect(_float3 vPos, CGameObject* pOwner)
{
	CEffect* pEffect = Create_Effect(pOwner);
	if (pEffect) 
		pEffect->Set_Position(vPos);

	return pEffect;
}

CEffect* CCharacter_Client::Create_Effect(const wstring& strPartTag)
{
	CGameObject* pPartObject = Find_PartObject(strPartTag);
	

	//CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_R"));
	//_float3 vPos = pWeapon->Get_WorldPosition();

	//pActor->Create_Effect(vPos);

	return nullptr;
}

void CCharacter_Client::Apply_Shake_And_Blur(Power ePower)
{
	if (nullptr == m_pDataManager)
	{
		m_pDataManager = CData_Manager::GetInstance();
	}

	CSpringCamera* pSpringCam = m_pDataManager->Get_MasterCamera()->Get_SpringCamera();
	if (pSpringCam)
	{
		pSpringCam->Set_ShakeCamera(true);
	}

	m_pGameInstance->Set_RadialBlurTime(0.2f * ECast(ePower));
}

void CCharacter_Client::Create_Hitting_Effect(_float3 vPos, Power ePower, string strEffectName, CGameObject* pOwner)
{
	//string strEffectFileName;
	//strEffectFileName = strEffectName != "" ? strEffectName : Get_CharcterDesc().EffectFileName;
	//strEffectFileName = strEffectFileName != "" ? strEffectFileName : "Hit_Normal";

	if (nullptr == m_pEffectManager)
	{
		m_pEffectManager = EFFECT_MANAGER;
	}

	//CEffect* pEffect = m_pEffectManager->Create_Effect(strEffectFileName + ".json", pOwner);
	CEffect* pEffect = EFFECT_MANAGER->Create_Effect("Hit/", "Hit_Distortion.json", vPos, TRUE, m_pGameInstance->Get_Player()->Get_Position());

	if (pEffect)
		pEffect->Set_Position(vPos);


	Apply_Shake_And_Blur(ePower);
}

_bool CCharacter_Client::Check_EffectOnTrackPosition()
{
	return Compare_TrackPosition_Is_Over(Get_CharcterDesc().EffectOnTrackPosition);
}

void CCharacter_Client::Free()
{
	__super::Free();
}
