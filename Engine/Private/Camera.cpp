#include "Camera.h"
#include "Transform.h"
#include "Light_Manager.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strPrototypeTag)
	: CGameObject(pDevice, pContext, strPrototypeTag)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CAMERA_DESC*	pCameraDesc = (CAMERA_DESC*)pArg;	

	/* �׵������ Ʈ�������� ���� �����ϰ� ���� ī�޶��� ���·� ����ȭ�ϳ�. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pCameraDesc->vEye));
	m_pTransformCom->Look_At(XMLoadFloat4(&pCameraDesc->vAt));

	m_fFovy = pCameraDesc->fFovy;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;

	return S_OK;
}

void CCamera::Priority_Tick(_float fTimeDelta)
{
}

void CCamera::Tick(_float fTimeDelta)
{
	
		m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
		m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));

	
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}


void CCamera::Update_Cascade()
{
	// ī�޶��� ����İ� �þ߰�,ȭ���, ����� ��� Z,�� ��� Z
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matView = m_pTransformCom->Get_WorldMatrixInverse();


	//�þ߰��� �̿��Ͽ� ���� �þ߰��� ����
	_float tanHalfVFov = tanf(m_fFovy / 2.0f);
	// ���� �þ߰��� �̿��Ͽ� ���� �þ߰��� ����
	_float tanHalfHFov = tanHalfVFov * m_fAspect;

	// ����ü�� ���������� �� �κ� ����ü�� �� ���� ����
	m_fCascadeEnd[0] = m_fNear;
	m_fCascadeEnd[1] = 10.f;
	m_fCascadeEnd[2] = 20.f;
	m_fCascadeEnd[3] = 40.f;

	// 3���� ����ü�� ���������� 3�� �ݺ���
	for (uint32_t i = 0; i < MAX_CASCADES; ++i)
	{
		//+X,+Y ��ǥ�� ����,���� �þ߰��� �̿��Ͽ� ����. �� �κ� ����ü�� �����,��
		//����� ���� ���Ͽ� 4���� ���� ����
		_float xn = m_fCascadeEnd[i] * tanHalfHFov;
		_float xf = m_fCascadeEnd[i + 1] * tanHalfHFov;
		_float yn = m_fCascadeEnd[i] * tanHalfVFov;
		_float yf = m_fCascadeEnd[i + 1] * tanHalfVFov;

		// +��ǥ���� ���ϸ� -��ǥ���� ���Ͽ� ������ ����ü ����� ���Ҽ�����.
		// �� ����ü�� Z���� �����Ͽ� i �� ���� ������ ����� ��� ������� ������
		_float4 frustumCorners[8] = {
			//near �簢����
			{xn,yn,m_fCascadeEnd[i],1.0f},
			{-xn,yn,m_fCascadeEnd[i],1.0f},
			{xn,-yn,m_fCascadeEnd[i],1.0f},
			{-xn,-yn,m_fCascadeEnd[i],1.0f},

			//far �簢����
			{xf,yf,m_fCascadeEnd[i + 1],1.0f},
			{-xf,yf,m_fCascadeEnd[i + 1],1.0f},
			{xf,-yf,m_fCascadeEnd[i + 1],1.0f},
			{-xf,-yf,m_fCascadeEnd[i + 1],1.0f}
		};

		_float4 centerPos; // ����ü �߽���
		for (uint32_t j = 0; j < 8; ++j)
		{
			frustumCorners[j] = XMVector4Transform(frustumCorners[j], matWorld);
			centerPos += frustumCorners[j];
		}

		centerPos /= 8.0f;
		_float radius = 0.0f;

		for (uint32_t j = 0; j < 8; ++j)
		{
			_float distance = XMVectorGetX(XMVector4Length(frustumCorners[j] - centerPos));
			radius = max(radius, distance);
		}

		radius = ceil(radius * 16.0f) / 16.0f;
	}
		// using radius ,  we made aabb box
	//	_float3 maxExtents(radius, radius, radius + 60.0f);
	//	_float3 minExtents = -maxExtents;
	//
	//	_float3 vLightDir = CLight_Manager->Get_LightDir();
	//	_float3 shadowCamPos = Vec3(centerPos) + (vLightDir * minExtents.z);
	//	Matrix lightMatrix = Matrix::CreateWorld(shadowCamPos, -vLightDir, Vec3(0.0f, 1.0f, 0.0f));
	//	Matrix lightInv = lightMatrix.Invert();
	//
	//	Vec3 cascadeExtents = maxExtents - minExtents;
	//
	//	Matrix matProj = XMMatrixOrthographicLH(cascadeExtents.x, cascadeExtents.y, 0.0f, cascadeExtents.z);
	//	m_shadowOrthoProj[i] = lightInv * matProj;
	//
	//	Vec3 vPos, vScale;
	//	Quaternion vQuat;
	//	lightMatrix.Decompose(vScale, vQuat, vPos);
	//	m_tCascadeShadowBox[i].Orientation = vQuat;
	//	m_tCascadeShadowBox[i].Center = Vec3(centerPos);
	//	m_tCascadeShadowBox[i].Extents = maxExtents;
	//}
	//
	//CPipeLine::GetInstance()->Set_ShadowProj(m_shadowOrthoProj);
	//CPipeLine::GetInstance()->Set_CascadeBoxes(m_tCascadeShadowBox);
}


void CCamera::Free()
{
	__super::Free();

}
