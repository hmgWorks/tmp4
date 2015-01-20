#include "StdAfx.h"
#include "cFrame.h"
#include "cMtlTex.h"


cFrame::cFrame(void)
	: m_pMesh(NULL)
	, m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);	
}


cFrame::~cFrame(void)
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::AddChild( cFrame* pChild )
{
	pChild->CalcLocalTM(&m_matWorldTM);
	m_vecChildren.push_back(pChild);
}

void cFrame::CalcLocalTM( D3DXMATRIXA16* pParent )
{
	D3DXMATRIXA16 matInvParent;
	D3DXMatrixInverse(&matInvParent, 0, pParent);
	m_matLocalTM = m_matWorldTM * matInvParent;	
}

void cFrame::BuidlMesh( std::vector<ST_PNT_VERTEX>& vecVertex )
{
	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_matWorldTM);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

	HRESULT hr = D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (size_t i = 0; i < vecVertex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(vecVertex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
}

void cFrame::SetMtlTex( cMtlTex* pMtlTex )
{
	if(!m_pMtlTex)
	{
		pMtlTex->AddRef();
		m_pMtlTex = pMtlTex;
	}
}

void cFrame::Update( D3DXMATRIXA16* pmatParent, int nkey)
{

	D3DXMATRIXA16  matT, matR;
	
	CalcPosTM(matT, nkey);
	CalcRotTM(matR, nkey);

	m_matWorldTM = matR * matT;

	if(pmatParent)
		m_matWorldTM *= (*pmatParent);
	for each(auto pChild in m_vecChildren)
	{
		pChild->Update(&m_matWorldTM, nkey);
	}
}

void cFrame::Render()
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->pTex);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
		m_pMesh->DrawSubset(0);
	}
	
	for each(auto pChild in m_vecChildren)
	{
		pChild->Render();
	}
}

void cFrame::Destroy()
{
	for each(auto pChild in m_vecChildren)
	{
		pChild->Destroy();
	}
	delete this;
}

void cFrame::CalcPosTM(D3DXMATRIXA16& mat, int nkey)
{
	//파일에서 읽어 온게 없으면
	if (m_vecPosTrack.empty())
	{
		D3DXMatrixIdentity(&mat);
		mat._41 = m_matLocalTM._41;
		mat._42 = m_matLocalTM._42;
		mat._43 = m_matLocalTM._43;
		return;
	}

	
#pragma region ======1개이하의 데이터 처리=====

	if (m_vecPosTrack.front().nKey >= nkey)
	{
		D3DXMatrixTranslation(&mat,
			m_vecPosTrack.front().v.x,
			m_vecPosTrack.front().v.y,
			m_vecPosTrack.front().v.z);
		return;
	}
	if (m_vecPosTrack.back().nKey <= nkey)
	{
		D3DXMatrixTranslation(&mat,
			m_vecPosTrack.back().v.x,
			m_vecPosTrack.back().v.y,
			m_vecPosTrack.back().v.z);
		return;
	}
#pragma endregion 


	int nPervIndex = -1;
	int nNextIndex = -1;

	for (int i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (m_vecPosTrack[i].nKey > nkey)
		{
			nNextIndex = i;
			nPervIndex = nNextIndex - 1;
			break;
		}
	}
	
	float t = (float)(nkey - m_vecPosTrack[nPervIndex].nKey) / (float)(m_vecPosTrack[nNextIndex].nKey - m_vecPosTrack[nPervIndex].nKey);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_vecPosTrack[nPervIndex].v, &m_vecPosTrack[nNextIndex].v, t);

	D3DXMatrixTranslation(&mat, v.x, v.y, v.z);	
}

void cFrame::CalcRotTM(D3DXMATRIXA16& mat, int nkey)
{
	if (m_vecRotTrack.empty())
	{
		mat = m_matLocalTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
		return;
	}

	if (m_vecRotTrack.front().nKey >= nkey)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
		return;
	}
	if (m_vecRotTrack.back().nKey <= nkey)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
		return;
	}

	int nPervIndex = -1;
	int nNextIndex = -1;


	for (int i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (m_vecRotTrack[i].nKey > nkey)
		{
			nNextIndex = i;
			nPervIndex = nNextIndex - 1;
			break;
		}
	}

	float t = (float)(nkey - m_vecRotTrack[nPervIndex].nKey) / (float)(m_vecRotTrack[nNextIndex].nKey - m_vecRotTrack[nPervIndex].nKey);

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPervIndex].q, &m_vecRotTrack[nNextIndex].q, t);
	
	D3DXMatrixRotationQuaternion(&mat, &q);
}