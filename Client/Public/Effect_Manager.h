#pragma once
#include "Client_Defines.h"
#include "Engine_Macro.h"

#include "Base.h"
#include "GameInstance.h"


BEGIN(Client)
class CEffect;
class CEffect_Trail;

//! EFFECT_MANAGER ��ũ�� : GetInstance()���ص� ��. (EFFECT_MANAGER->Create_Effect(~~);)

//! �� ������� �𸣰ڴ�!! -> �ϴ� Create_Effect(string strFileName, CGameObject* pOwner = nullptr); ȣ��
//! ����Ʈ�� � ��ü�� ���� ���������Ѵ�! pOwner�� �ش� ��ü �־��ֱ�(��Ʈ������Ʈ ����ó�� ����ٴ�), bParentPivot�� true���� Ȯ��

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

	// �� ���� ��ο� �ִ� ����Ʈ ������ ���� ����ϰ���� �� ȣ�� : strAddPath�� "/" ���� (Data_Effect/Zapper_Dash/Zapper_Dash_10.json)
	CEffect* Create_Effect(string strAddPath, string strFileName, CGameObject* pOwner = nullptr);


	// Ư�� ��ġ�� ����Ʈ�� �����ϰ� ���� ��� ȣ��(vPos�� ������ų ��ġ)
	CEffect* Create_Effect_Pos(string strFileName, _float3 vPos, CGameObject* pOwner = nullptr);
	CEffect* Create_Effect_Pos(string strAddPath, string strFileName, _float3 vPos, CGameObject* pOwner = nullptr);

	// Ʈ������ �޸� ����Ʈ�� ������ �� ���(����Ʈjsno����, Ʈ����json����, ����Ʈ�� ������������� ��ü�� �ִ� ��� ���� �ֱ�)
	CEffect* Create_Effect_With_Trail(string strEffectFileName, string strTrailFileName, CGameObject* pOwner = nullptr);

	// �εεε� ����Ʈ ������ �ڸ��� ����Ʈ ����(�����ð� ����, �����ð� ��, Ÿ�ӵ�Ÿ, ������ ����Ʈ ����.json, ������ ��ġ, �ٶ� Ÿ���� �ִ���, Ÿ���� ��ġ)
	HRESULT Tick_Create_Effect(_float* fTimeAcc, _float fCreateTime, _float fTimeDelta, string strEffectFileName
		, _float3 vPos = _float3(0.f, 0.f, 0.f)
		, _bool bLookTarget = FALSE, _float4 vTargetPos = _float4(0.f, 0.f, 0.f, 0.f));



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

