#pragma once
#include "Client_Defines.h"
#include "Engine_Macro.h"

#include "Base.h"
#include "GameInstance.h"


BEGIN(Client)
class CEffect;
class CEffect_Trail;

//! EFFECT_MANAGER ��ũ�� : GetInstance()���ص� ��. (EFFECT_MANAGER->Create_Effect(~~);)

class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager);

private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:	
	// Effect ======================================================================================================================================

	//! ���� : strFileName�� json���� �̸�. ("../Bin/DataFiles/Data_Effect/" ��ο� ������ ���� json���� �� ���ϴ°� ��󾲱�! (.json �پ����))
	CEffect* Create_Effect(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner = nullptr);

	//! ��� : ���ϰ� ������ ���� �Լ��� Ȱ���� ����ȭ ��Ŵ
	CEffect* Create_Effect(string strFileName, CGameObject* pOwner = nullptr);

	// ���� ��ġ�� ����Ʈ�� �����ϰ� ���� ��� ȣ��(vPos�� ������ų ��ġ)
	CEffect* Create_Effect_Pos(string strFileName, _float3 vPos);

	// Ʈ������ �޸� ����Ʈ�� ������ �� ���(����Ʈjsno����, Ʈ����json����, ����Ʈ�� ������������� ��ü�� �ִ� ��� ���� �ֱ�)
	CEffect* Create_Effect_With_Trail(string strEffectFileName, string strTrailFileName, CGameObject* pOwner = nullptr);

	// �εεε� �� ����Ʈ �׽�Ʈ
	HRESULT Tick_Create_Effect(_float* fTimeAcc, _float fCreateTime, _float fTimeDelta, string strEffectFileName
		, _float3 vLocalPos = _float3(0.f, 0.f, 0.f), _float3 vLocalScale = _float3(1.f, 1.f, 1.f), _float3 vLocalRotation = _float3(0.f, 0.f, 0.f));



public:
	// Trail ========================================================================================================================================

	// Ʈ���� ����(�غ�)
	CEffect_Trail* Ready_Trail(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner = nullptr);

	// Ʈ���� ����(�غ�) : ����ȭ
	CEffect_Trail* Ready_Trail(string strFileName, CGameObject* pOwner = nullptr);


private:
	class CGameInstance*	m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };


public:
	virtual void	Free();
};

END

