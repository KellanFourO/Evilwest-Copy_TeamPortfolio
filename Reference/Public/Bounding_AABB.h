#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CCollider;

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagAABB: public BOUNDING_DESC
	{
		_float3		vExtents;
	}BOUNDING_AABB_DESC;
public:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	const shared_ptr<BoundingBox> Get_Bounding() {
		return m_pAABB;
	}

public:
	HRESULT Initialize(BOUNDING_DESC* pBoundingDesc);
#ifdef _DEBUG
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _vector vColor) override;
#endif
	virtual void Update(_fmatrix TransformMatrix);

public:
	virtual _bool Collision(shared_ptr<CCollider> pTargetCollider, _bool* pisCollision) override;

private:
	/* �ʱ���¸� ������ ������ */
	shared_ptr<BoundingBox>			m_pOriginalAABB = { nullptr };

	/* ����� ���ŵ� ���¸� ������ ������ */
	shared_ptr<BoundingBox>			m_pAABB = { nullptr };

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* pBoundingDesc);
	virtual void Free() override;
};

END