#pragma once
#include "Client_Defines.h"
#include "Engine_Macro.h"

#include "Base.h"
#include "GameInstance.h"


BEGIN(Client)
class CEffect;
class CEffect_Trail;

//! EFFECT_MANAGER ��ũ�� : GetInstance()���ص� ��. (EFFECT_MANAGER->Create_Effect(~~);)
//! ����Ʈ�� ����ƽ ����

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
	CEffect* Play_Effect(string strAddPath, string strFileName, CGameObject* pOwner = nullptr, _bool bUseSocket = false, string strBoneTag = "");
	CEffect* Play_Effect(string strAddPath, string strFileName, CGameObject* pOwner, _float3 vPos, _bool bLookTarget = false, _float3 vTargetPos = _float3(0.f, 0.f, 0.f));
	CEffect* Play_Effect_StaticPivot(string strAddPath, string strFileName, CGameObject* pOwner, _float4x4 matPivot);


	// �εεε� ����Ʈ ������ �ڸ��� ����Ʈ ����(�����ð� ����, �����ð� ��, Ÿ�ӵ�Ÿ, ������ ����Ʈ ����.json, ������ ��ġ, �ٶ� Ÿ���� �ִ���, Ÿ���� ��ġ)
	HRESULT Generate_Effect(_float* fTimeAcc, _float fGenerateTimeTerm, _float fTimeDelta, string strAddPath, string strFileName
		                   , _float3 vPos
						   , _bool bLookTarget = false, _float3 vTargetPos = _float3(0.f, 0.f, 0.f)
							, _bool bScaleLerp = false, _float3* vScaleAcc = nullptr);


	HRESULT Generate_Effect_AttachBone(_float* fTimeAcc, _float fGenerateTimeTerm, _float fTimeDelta, string strAddPath, string strFileName
										, CGameObject* pOwner = nullptr, _bool bUseSocket = false, string strBoneTag = "");

public:
	// Trail ========================================================================================================================================

	// Ʈ���� ����(�غ�)
	CEffect_Trail* Ready_Trail(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner = nullptr);

	// Ʈ���� ����(�غ�) : ����ȭ
	CEffect_Trail* Ready_Trail(string strFileName, CGameObject* pOwner = nullptr);



public:
	// Pool ===========================================================================================================================================
	HRESULT				Ready_EffectPool();

	HRESULT				Add_ToPool(_uint iLevelIndex, string strAddPath, string strFileName, _uint iCount = 1, _bool bHasTrail = false, string strTrailFileName = "");
	HRESULT				Return_ToPool(CEffect* pEffect);


private:
	HRESULT				Clear_EffectPool();
	queue<CEffect*>*	Get_EffectPool(string strFileName);

private:
	// strAddPath, strFileName ( "Player/Zapper_Dash/", "Zapper_Dash_26.json" )
	CEffect*			Load_Effect(_uint iLevelIndex, string strAddPath, string strFileName, _bool bHasTrail = false, string strTrailFileName = "");


//#ifdef _DEBUG
	CEffect*			Create_Effect_ForDebug(string strAddPath, string strFileName, CGameObject* pOwner = nullptr, _bool bUseSocket = false, string strBoneTag = "");
	CEffect*			Create_Effect_ForDebug(string strAddPath, string strFileName, _float3 vPos, _bool bLookTarget = false, _float3 vTargetPos = _float3(0.f, 0.f, 0.f));
	CEffect*			Create_Effect_ForDebug_StaticPivot(string strAddPath, string strFileName, CGameObject* pOwner, _float4x4 matPivot);
//#endif // _DEBUG



private:
	static const _uint iMaxManyEffect			= { 200 };
	static const _uint iMaxEffect				= { 50 };
	static const _uint iMaxFewEffect			= { 5 };
	unordered_map<string, queue<CEffect*>>		m_EffectPool;


private:
	class CGameInstance*	m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice		= { nullptr };
	ID3D11DeviceContext*	m_pContext		= { nullptr };


public:
	virtual void	Free();
};

END

