#include "..\Public\Bounding_AABB.h"
#include "DebugDraw.h"
#include "Collider.h"

CBounding_AABB::CBounding_AABB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBounding(pDevice, pContext)
{

}

HRESULT CBounding_AABB::Initialize(BOUNDING_DESC * pBoundingDesc)
{
	BOUNDING_AABB_DESC* pDesc = (BOUNDING_AABB_DESC*)pBoundingDesc;

	m_pOriginalAABB = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
	m_pAABB = new BoundingBox(*m_pOriginalAABB);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix TransformMatrix)
{
	_matrix		Matrix = TransformMatrix;

	Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[0]));
	Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[1]));
	Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[2]));

	m_pOriginalAABB->Transform(*m_pAABB, Matrix);


}

_bool CBounding_AABB::Collision(CCollider * pTargetCollider, _bool* pisCollision)
{
	CBounding*	pTargetBounding = pTargetCollider->Get_Bounding();

	*pisCollision = false;

	switch (pTargetCollider->Get_ColliderType())
	{
	case CCollider::TYPE_AABB:
		*pisCollision = m_pAABB->Intersects(*((CBounding_AABB*)pTargetBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_OBB:
		*pisCollision = m_pAABB->Intersects(*((CBounding_OBB*)pTargetBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_SPHERE:
		*pisCollision = m_pAABB->Intersects(*((CBounding_Sphere*)pTargetBounding)->Get_Bounding());
		break;
	}
	return *pisCollision;
}


#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _vector vColor)
{
	if (nullptr == pBatch ||
		nullptr == m_pAABB)
		return E_FAIL;

	/* m_pAABB�� ���彺���̽� ���� ������. */
	DX::Draw(pBatch, *m_pAABB, vColor);

	return S_OK;
}

#endif


CBounding_AABB * CBounding_AABB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, BOUNDING_DESC* pBoundingDesc)
{
	CBounding_AABB*		pInstance = new CBounding_AABB(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pBoundingDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalAABB);
	Safe_Delete(m_pAABB);
}
