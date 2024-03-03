#pragma once

#include "Base.h"

BEGIN(Engine)

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
	HRESULT			Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	/* For. Shadow */
	HRESULT Set_ShadowLight(_uint iLevelIndex, _float4 vEye, _float4 vAt, _float4 vUp);
	HRESULT Add_ShadowLight_View(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp);
	HRESULT Add_ShadowLight_Proj(_uint iLevelIndex, _float fFovAngleY, _float fAspectRatio, _float fNearZ, _float fFarZ);

	_float4x4 Get_ShadowLightViewMatrix(_uint iLevelIndex);
	_float4x4 Get_ShadowLightViewMatrix_Inverse(_uint iLevelIndex);
	_float4x4 Get_ShadowLightProjMatrix(_uint iLevelIndex);

private:
	_float4x4					Identity_Matrix;
	list<class CLight*>			m_Lights;

	map<_uint, _float4x4> m_ShadowLight_ViewMatrix;
	map<_uint, _float4x4> m_ShadowLight_ProjMatrix;

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

END