#include "stdafx.h"
#include "cHeightMap.h"
#include <fstream>

cHeightMap::cHeightMap()
	:m_pVB(NULL)// m_pMesh(NULL)
	, m_pIB(NULL)
	, m_pTexture(NULL)
{
}

cHeightMap::~cHeightMap()
{
	//SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void cHeightMap::Load(std::string file)
{
	D3DXCreateTextureFromFile(g_pD3DDevice, "../../Resources/HeightMapData/terrain.jpg",&m_pTexture);
	//D3DXCreateTextureFromFile(g_pD3DDevice, "terrain.jpg", &m_pTexture);
	
	ST_PNT_VERTEX v;	
	FILE* pFile;
	fopen_s(&pFile, file.c_str(), "rb");
	{
		unsigned char ch;
		for (int z = 0; z < HEIGHT_SIZE; z++)
		{
			for (int x = 0; x < HEIGHT_SIZE; x++)
			{	
				ch = fgetc(pFile);
				v.p = D3DXVECTOR3(x, (float)ch / 3.0f, z);
				v.t = D3DXVECTOR2((float)x / HEIGHT_SIZE, (float)z / HEIGHT_SIZE);
				m_vecVertex.push_back(v);
			}
		}	

		//버택스 노말
		D3DXVECTOR3 vNormal;
		D3DXVECTOR3 v1,v2;
		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			for (int j = 0; j < HEIGHT_SIZE; j++)
			{
				if (j == 0 || j == (HEIGHT_SIZE - 1) || i == 0 || i == (HEIGHT_SIZE - 1))
				{
					m_vecVertex[i * HEIGHT_SIZE + j].n = D3DXVECTOR3(0, 1, 0);
				}
				else
				{					
					v1 = m_vecVertex[i*HEIGHT_SIZE + (j + 1)].p - m_vecVertex[i*HEIGHT_SIZE + (j - 1)].p;
					v2 = m_vecVertex[(i +1) *HEIGHT_SIZE + j].p - m_vecVertex[(i-1)*HEIGHT_SIZE + j].p;
					D3DXVec3Cross(&vNormal, &v2, &v1);
					D3DXVec3Normalize(&vNormal,&vNormal);
					m_vecVertex[i * HEIGHT_SIZE + j].n = vNormal;
				}				
			}
		}
	}
	fclose(pFile);
	
	ST_INDEX id;
	int index;
	for (int z = 0; z < HEIGHT_SIZE-1; z++)
	{
		for (int x = 0; x < HEIGHT_SIZE-1; x++)
		{		
			
			index = x + HEIGHT_SIZE  * z;
			
			id.i1 = index;
			id.i2 = index + HEIGHT_SIZE;
			id.i3 = index + 1;
			m_vecIndex.push_back(id);

			id.i1 = id.i3;
			id.i3 = id.i2 + 1;
			m_vecIndex.push_back(id);

		}
	}
	g_pD3DDevice->CreateVertexBuffer(m_vecVertex.size()*sizeof(ST_PNT_VERTEX),
		0, ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, 0);
	g_pD3DDevice->CreateIndexBuffer(m_vecIndex.size()*sizeof(ST_INDEX),
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);

	ST_PNT_VERTEX* vp;
	m_pVB->Lock(0, 0, (LPVOID*)&vp, 0);
	{
		memcpy(vp, &m_vecVertex[0], m_vecVertex.size()*sizeof(ST_PNT_VERTEX));
	}
	m_pVB->Unlock();

	ST_INDEX* ip;
	m_pIB->Lock(0, 0, (LPVOID*)&ip, 0);
	{
		memcpy(ip, &m_vecIndex[0], m_vecIndex.size()*sizeof(ST_INDEX));
	}
	m_pIB->Unlock();
}

void cHeightMap::Render()
{	
	D3DXMATRIXA16 mat;
	D3DXMatrixTranslation(&mat, -257.0f / 2.0f, 0, -257.0f / 2.0f);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3DDevice->SetTexture(0, m_pTexture/*NULL*/);
	//g_pD3DDevice->SetTexture(0, /*m_pTexture*/NULL);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetIndices(m_pIB);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vecVertex.size(), 0, m_vecIndex.size());
}