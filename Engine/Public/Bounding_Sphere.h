#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct tabSphere : public BOUNDING_DESC
	{
		_float		fRadius;
	}BOUNDING_SPHERE_DESC;
private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CBounding_Sphere() = default;

public:
	BoundingSphere* Get_Bounding() {
		return m_pSphere;
	}

	void Set_Bounding(BoundingSphere* pSphere) { m_pSphere = pSphere; }
	virtual void Set_matScale(_matrix _matScale) override { m_matScale = _matScale; }
	virtual void Set_Test(_vector _vTest) override { m_vTest = _vTest; }
public:
	HRESULT Initialize(BOUNDING_DESC* pBoundingDesc);
#ifdef _DEBUG
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _vector vColor) override;
#endif
	virtual void Update(_fmatrix TransformMatrix);

public:
	virtual _bool Collision(class CCollider* pTargetCollider, _bool* pisCollision) override;

private:
	BoundingSphere*			m_pOriginalSphere = { nullptr };
	BoundingSphere*			m_pSphere = { nullptr };
	


public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* pBoundingDesc);
	virtual void Free() override;
};

END