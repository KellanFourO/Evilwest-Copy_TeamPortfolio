#pragma once

#include "Engine_Defines.h"
#include "Json_Utility.h"

/* ���۷���ī��Ʈ�� �����ϱ����� ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ű��. */
	/* _ulong : ������Ű�� �� ������ ���� ����. */
	_ulong AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ű�ų� �����ϰų�.. */
	/* _ulong : ���ҽ�Ű�� ������ ���� ���� .*/
	_ulong Release();

	virtual void Set_Enable(_bool _Enable);
	_bool Get_Enable();

	virtual _bool Write_Json(json& Out_Json) { return false; };
	virtual void Load_FromJson(const json& In_Json) {};

protected:
	_bool			m_bEnable = { true };

private:
	_ulong			m_dwRefCnt = { 0 };
	

public:
	virtual void Free() {}
};

END