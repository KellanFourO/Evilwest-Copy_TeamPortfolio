#pragma once
#include "Client_Defines.h"
#include "UI_Text.h"
#include "Texture.h"

/* ü�� ������ */
class CUI_TutorialBox final : public CUI_Text
{
private:
	CUI_TutorialBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_TutorialBox(const CUI_TutorialBox& rhs);
	virtual ~CUI_TutorialBox() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	/* State */
	virtual void			UI_Ready(_float fTimeDelta);
	virtual void			UI_Enter(_float fTimeDelta);
	virtual void			UI_Loop(_float fTimeDelta);
	virtual void			UI_Exit(_float fTimeDelta);

public:
	virtual void			Add_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation) override;
	//virtual TEXTINFO*		Find_Text(const string& strTextTag) override;
	virtual HRESULT			Find_Change(const string& strTextTag) override;
	//virtual void			Change_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor) override;

public:
	void					Change_TutorialMassege(_int iTextNum) 
	{ 
		if (0 > iTextNum || m_iMaxTextNum <= iTextNum)
			return;

		m_iTextNum = iTextNum; 
	}
	_int					Get_TutorialMassege() { return m_iTextNum; }

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;
	HRESULT					Ready_Text();

private:
	void					Compute_OwnerCamDistance();
	_bool					In_Frustum();
	_int					m_iTextNum = 0;
	_int					m_iMaxTextNum = (_int)TUTORIAL_TEXT::TUTORIALTEXT_END;

public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_TutorialBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

