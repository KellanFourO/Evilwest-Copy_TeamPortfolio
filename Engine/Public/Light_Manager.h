#pragma once

#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;


class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	HRESULT			Initialize();
	HRESULT			Add_Light(const LIGHT_DESC& LightDesc, _int& outLightIndex);
	class CLight*	Find_Light(const _int iIndex);
	_bool			Remove_Light(const _uint& iIndex);
	HRESULT			Render(shared_ptr<CShader> pShader, shared_ptr<CVIBuffer_Rect> pVIBuffer);

private:
	list<class CLight*>			m_Lights;

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

END