#include "stdafx.h"
#include "cWoman.h"
#include "cFrame.h"
#include "cAseLoader.h"

cWoman::cWoman()
	:m_pCurrentFrame(NULL)
	, m_nCurrentANI(ANI::IDLE)
	, m_vPosition(0, 0, 0)
	, m_vForward(0, 0, -1)
	, m_fAngle(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cWoman::~cWoman()
{	
	m_vecFrame[0]->Destroy();
	m_vecFrame[1]->Destroy();
}

void cWoman::Setup()
{
	m_nOld = GetTickCount();
	std::string strFolder = "../../Resources/ase/woman/";
	m_vecFrame.resize(2);
	cAseLoader aseLoader, aseLoader2;
	m_vecFrame[ANI::IDLE] = aseLoader.Load(strFolder, std::string("woman_01_all_stand.ASE"));
	m_vecFrame[ANI::WALKING] = aseLoader2.Load(strFolder, std::string("woman_01_all.ASE"));
}

void cWoman::Update()
{
	if (g_pInputManager->GetKeyDown('W'))
	{

		m_nCurrentANI = ANI::WALKING;				
		m_vPosition += m_vForward;/// *0.09f;
	}
	else
	{
		m_nCurrentANI = ANI::IDLE;
	}
	if (g_pInputManager->GetKeyDown('A'))//ÁÂÈ¸Àü
	{
		m_nCurrentANI = ANI::WALKING;
		m_fAngle -= 0.1f;
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, m_fAngle);
		m_vForward = D3DXVECTOR3(0, 0, -1);
		D3DXVec3TransformNormal(&m_vForward, &m_vForward, &matR);
	}
	if (g_pInputManager->GetKeyDown('D'))//¿ìÈ¸Àü
	{
		m_nCurrentANI = ANI::WALKING;
		m_fAngle += 0.1f;
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, m_fAngle);
		m_vForward = D3DXVECTOR3(0, 0, -1);
		D3DXVec3TransformNormal(&m_vForward, &m_vForward, &matR);
	}


	static int cnt = m_vecFrame[m_nCurrentANI]->m_nSceneFirstframe;
	m_nCurt = GetTickCount();
	if (m_nCurt > m_nOld + m_vecFrame[m_nCurrentANI]->m_nSceneFramespeed)
	{
		cnt++;
		m_nOld = m_nCurt;
		if (cnt > m_vecFrame[m_nCurrentANI]->m_nSceneLastframe)
		{
			cnt = m_vecFrame[m_nCurrentANI]->m_nSceneFirstframe;
		}
	}
	D3DXMATRIXA16 matT, matR, matS;
	D3DXMatrixScaling(&matS, 10, 10, 10);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld =matS* matR * matT;
	m_vecFrame[m_nCurrentANI]->Update(&m_matWorld, cnt);
}

void cWoman::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_vecFrame[m_nCurrentANI]->Render();
}