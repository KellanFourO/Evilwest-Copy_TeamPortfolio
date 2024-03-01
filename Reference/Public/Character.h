#pragma once

#include "GameObject.h"
#include "Model.h"

#include "Body.h"
#include "Weapon.h"

#include "Engine_Enum.h"

BEGIN(Engine)

class CNavigation;
//class CPhysXCharacterController;

class ENGINE_DLL CCharacter abstract : public CGameObject
{
public:
	typedef struct tagGameCharacterDesc
	{
		string	Body = "";
		string	BoneName = "";

		_float  AnimationIndex = { 0.f };
		_float	Duration = { 0.f };
		_float  EventTrackposition = {0.f};
		_float  AnimationSpeed = { 0.f };
		_float	BoneIndex = { 0.f };

		_float	ColliderSize = { 0.f };
		_float	ColliderTrackPositionOn = { 0.f };
		_float	ColliderTrackPositionOff = { 0.f };
		//Weapon
		_float	EventWeaponTrackPosition = { 0.f };
		_float	ColliderWeaponSize = { 0.f };
		_float	ColliderWeaponTrackPositionOn = { 0.f };
		_float	ColliderWeaponTrackPositionOff = { 0.f };

		_float3 GuizmoTranslatrion = {};
		_float3 GuizmoRotation = {};
		_float3 GuizmoScale = {};
		//Effect
		string	EffectFileName = "";
		_float	EffectOnTrackPosition = { 0.f };
		_float3	EffectPosition = {};

		//Sound
	}CHARCTER_DESC;
protected:
	CCharacter(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strPrototypeTag);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CBody*			Get_Body();
	CWeapon*		Get_Weapon(const wstring & strWeaponTag);
	CCollider*		Get_Collider();
	CNavigation*	Get_Navigation();

	virtual _bool Picking(_Out_ _float3 * vPickedPos) override;
public:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_PartObjects() PURE;

	CGameObject* Find_PartObject(const wstring & strPartTag);
	HRESULT Add_PartObject(const wstring & strPrototypeTag, const wstring & strPartTag, void* pArg);
	HRESULT Add_Body(const wstring & strPrototypeTag, CBody::BODY_DESC pArg);
	HRESULT Add_Weapon(const wstring & strPrototypeTag, string strBoneName, CWeapon::WEAPON_DESC pArg, const wstring & strWeaponTag = TEXT("Part_Weapon"));
public:
	void	Set_EventNotify(string strPath, string JsonFileName);
	HRESULT	LoadAnimJson(string strPath, string strFileName);
public:
	void	Set_Animation(
		_uint _iNextAnimation
		, CModel::ANIM_STATE _eAnimState = CModel::ANIM_STATE::ANIM_STATE_NORMAL
		, _bool _bIsTransition = true
		, _bool _bUseAnimationPos = true
		, _uint iTargetKeyFrameIndex = 0);

	_bool	Is_Animation_End();
	_bool	Is_Inputable_Front(_uint _iIndexFront);

	void Go_Straight(_float fTimeDelta);
	void Go_Straight_L45(_float fTimeDelta);
	void Go_Straight_R45(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Backward_L45(_float fTimeDelta);
	void Go_Backward_R45(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);

public:
	virtual void Set_Enable(_bool _Enable) override;

public:
	virtual void Set_Hitted() {};	//test
	virtual Hit_Type Set_Hitted(_uint iDamage, _float3 vForce, _float fStiffnessRate, Direction eHitDirection, Power eHitPower);


	virtual void Hitted_Left() {};
	virtual void Hitted_Right() {};
	virtual void Hitted_Front() {};
	virtual void Hitted_Down() {};
	virtual void Hitted_Dead() {};

public:
	_int Get_Hp() {
		return m_iHp;
	};

	void Set_Hp(_uint _iHp) {
		m_iHp = _iHp;
	};

protected:
	_int m_iHp = { 0 };
	Power m_eStrength = { Power::Light };
	_float m_fStiffnessRate = { 1.f };


protected:
	CNavigation* m_pNavigationCom = { nullptr };
	CBody* m_pBody = { nullptr };
	vector<CWeapon*> m_Weapons;
	CHARCTER_DESC CharAnimDesc = {};

protected:
	CCharacter* m_pTarget = { nullptr };

protected:
	//CPhysXController* m_pPhysXControllerCom = { nullptr };
	PxControllerCollisionFlags m_LastCollisionFlags;

protected:
	map<const wstring, class CGameObject*>		m_PartObjects;

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual CGameObject* Pool() PURE;
	virtual void Free() override;
};

END