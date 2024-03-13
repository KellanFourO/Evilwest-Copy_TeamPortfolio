#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNaviDesc
	{
		_int		iCurrentIndex;
	}NAVI_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strNavigationFilePath);
	virtual HRESULT Initialize(void* pArg) override;	

#ifdef _DEBUG
	virtual HRESULT Render();
#endif

public:
	void	Update(_fmatrix WorldMatrix);
	_bool	isMove(_fvector vPosition);
	void	SaveData(wstring strSavePath);
	void	LoadData(wstring strLoadPath);

public:
	vector<class CCell*>	Get_Cells() { return m_Cells; }
	class CCell*			Get_CurrentCell() { return m_Cells[m_iCurrentIndex]; }
	_int					Get_CellSize() { return m_Cells.size(); };
	void					Set_CurrentIndex(_int iIndex) { m_iCurrentIndex = iIndex; }

	void					AddCell(class CCell* pCell);
	void					Delete_Cell(const _uint iIndex);
	void					AllSearchDelete_IsNan();

public:
	void					InRangeCellChange(class CCell* pCell, _int ePoint, _float3 vSearchPos);
	_int					Get_SelectRangeCellIndex(class CGameObject* pTargetObject);
	_float					Compute_Height(_float3 vPosition, _Out_ _bool* pGround);


private:
	vector<class CCell*>			m_Cells;
	static _float4x4				m_WorldMatrix;
	_int							m_iCurrentIndex = { -1 };

#ifdef _DEBUG
private:
	class CShader*					m_pShader = { nullptr };
#endif

private:
	HRESULT Make_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END