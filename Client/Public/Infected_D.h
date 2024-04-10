#pragma once

#include "Infected.h"

BEGIN(Client)

class CInfected_D final : public CInfected
{
protected:
	CInfected_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CInfected_D(const CInfected_D& rhs);
	virtual ~CInfected_D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
	virtual HRESULT Render_OutLine() override;
	virtual HRESULT Update_RenderState(_int CurrentHP) override;

public:
	virtual void Explosion() override;

protected:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Ready_Option();

	virtual void Check_Frustum() override;


private:
	CActor<CInfected>* m_pActor = { nullptr };

public:
	static CInfected_D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;
	virtual void Free() override;

};

END

/* ���� ����
* Infected_IdleAct_01 �̰ɷ� �� ����. �˿��� �¾�� ��Ʋ��ٰ� �޷����������ΰ��� 
0. ���� : �˿��� ���ִ� ���� �״�� ���� / �ٸ� �����ִϸ��̼��� ������ ( ���� ��ü�� 30���� �̻� �ٵ� ��δ� �����ִϸ��̼�X)
Infected_Sprint_F_01
1. ���� : ������ �Ÿ�������� �÷��̾����� �޷����� -> ������
Infected_Taunt_01 �������� �̰ɷζ� ? /electrocute loop / 
2. ���� : ������Ÿ��� ������ ���ڸ����� ���߰� �¸��� �и鼭 ���� ����
3. �������� ����Ƣ�� ��
*/




