#include "stdafx.h"
#include "cWoman.h"
#include "cFrame.h"
#include "cAseLoader.h"

cWoman::cWoman()
	:m_pCurrentFrame(NULL)
	,m_nCurrentANI(ANI::IDLE)
{

}

cWoman::~cWoman()
{
	SAFE_DELETE(m_pCurrentFrame);
	/*for (auto p : m_vecFrame)
		SAFE_DELETE(p);*/
	
}

void cWoman::Setup()
{
	m_nOld = GetTickCount();
	std::string strFolder = "../../Resources/ase/woman/";
	m_vecFrame.resize(2);
	cAseLoader aseLoader;
	cFrame* frame;
	frame = aseLoader.Load(strFolder, std::string("woman_01_all_stand.ASE"));
	m_vecFrame[ANI::IDLE] = frame;

	frame = aseLoader.Load(strFolder, std::string("woman_01_all.ASE"));
	m_vecFrame[ANI::WALKING] = frame;
	m_pCurrentFrame = m_vecFrame[ANI::WALKING];
}

void cWoman::Update()
{
	/*if (g_pInputManager->GetKeyDown('D'))
	{
		m_pCurrentFrame =m_vecFrame[ANI::WALKING];
	}
	else
	{
		m_pCurrentFrame = m_vecFrame[ANI::IDLE];
	}*/
	static int cnt = m_pCurrentFrame->m_nSceneFirstframe;
	m_nCurt = GetTickCount();
	if (m_nCurt > m_nOld + m_pCurrentFrame->m_nSceneFramespeed)
	{
		cnt++;
		m_nOld = m_nCurt;
		if (cnt > m_pCurrentFrame->m_nSceneLastframe)
		{
			cnt = m_pCurrentFrame->m_nSceneFirstframe;
		}
	}
	m_pCurrentFrame->Update(NULL, cnt);
}

void cWoman::Render()
{
	m_vecFrame[0]->Render();
}