#pragma once
#include "Client_Defines.h"
#include "UI_Base.h"

/* ü�� ������ */
class CUI_Anything final : public CUI_Base
{
public:
	enum MONSTERTYPE { SMALL, MID, LARGE, BOSS, SIDE, NONE };

	typedef struct tagUI_BasicInfo
	{
		_float		fX, fY, fSizeX, fSizeY;
	}UIBASICINFO;

	typedef struct tagUI_AnythingDesc : public CUI_Base::UI_DESC
	{
		CTransform*		pOwnerTransform;
		//STATUS_DESC*	pOwnerStatus;
		MONSTERTYPE		eMonsterType;
		string			strProtoTag = "";
		_float			fCrntHPUV = 1.0f;
		_float			fPrevHPUV = 1.0f;
		_float			fScreenPosXOffset = 1.f;

	}UIANYTHING;

private:
	CUI_Anything(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Anything(const CUI_Anything& rhs);
	virtual ~CUI_Anything() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

private:
	void					Compute_OwnerCamDistance();
	_bool					In_Frustum();

private:
	LEVEL					m_eCurrentLevel = LEVEL_END;

private:
	UIANYTHING				m_tInfo = {};
	_float					m_fCurrentHp;
	_float					m_fPrevHp;
	/*_float				m_fHPBarTimeDuration = 100.f;*/

	const _float			m_fDefaultScale = 0.5f;
	_float					m_fScaleOffsetX = 1.f;


	_float					m_fScaleOffsetY = 1.f;

	_float					m_fPosXOffset = 0.f;
	_float					m_fPosYOffset = 0.f;
	_float					m_fOwnerCamDistance;

public:
	void				 Save_Desc();
	void				 Load_Desc();

public:
	static CUI_Anything* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual void			Free() override;
};

