#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_Skill_Preview final : public CUI
{
public: /* �� UI�������� ��� �󸶳� ������ ����������. */
	enum TEXTUREKIND { SKILLPREVIEW, TEXTURE_END };

private:
	CUI_Skill_Preview(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_Skill_Preview(const CUI_Skill_Preview& rhs);
	virtual ~CUI_Skill_Preview() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	/* State */
	virtual void	UI_Ready(_float fTimeDelta);
	virtual void	UI_Enter(_float fTimeDelta);
	virtual void	UI_Loop(_float fTimeDelta);
	virtual void	UI_Exit(_float fTimeDelta);

public:
	void			Start_Setting();
	void			Change_Priview(const string& strUIName);

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };

private: // Frame
	_int		m_iCurrentFrame = 0;
	DWORD		m_Time = (DWORD)GetTickCount64();
	_float		m_fFrameChangeTime = 0.f;

private: // Sprite Info
	_int		m_iMaxFrame = 0;
	_bool		m_bFinish = false;
	_bool		m_bSkip = false;
	_bool		m_bLoop = false;
	string		m_strPreName = "";

public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_Skill_Preview* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

