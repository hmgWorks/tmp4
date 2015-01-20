#include "StdAfx.h"
#include "cMainGame.h"
#include "cCube.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cCubeMan.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "iMap.h"
#include "cIndexCube.h"
#include "cMtlTex.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cTeapot.h"
#include "cActionMove.h"
#include "cWayPoint.h"
#include "cActionSeq.h"
#include "cActionRepeat.h"
#include "cWoman.h"
#include "cHeightMap.h"

cMainGame::cMainGame(void)
	: m_pAseRoot(NULL)
	, m_pMap(NULL)
	, m_pFont(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pWoman(NULL)
	, m_pHeightMap(NULL)
{
	m_bUpKey = FALSE;	
	m_bDownKey = FALSE;
	m_nKeyValue = 0;
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pHeightMap);
	SAFE_DELETE(m_pWoman);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pMap);
	/*if(m_pAseRoot)
		m_pAseRoot->Destroy();*/



	g_pTextureManager->Destroy();
	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cMainGame::Setup()
{
	std::string sFolder(RESOURCE_FOLDER);
	sFolder += std::string("ase/woman/");

	//cAseLoader AseLoader;
	//m_pAseRoot = AseLoader.Load(sFolder, std::string("woman_01_all_stand.ASE"));
	//m_pAseRoot = AseLoader.Load(sFolder, std::string("woman_01_all.ASE"));

	m_pGrid = new cGrid;
	m_pGrid->Setup(30, 1.0f);

	m_pWoman = new cWoman;
	m_pWoman->Setup();

	m_pHeightMap = new cHeightMap;
	std::string file = std::string(RESOURCE_FOLDER);
	file += std::string("HeightMapData/HeightMap.raw");
	m_pHeightMap->Load(file);


	m_pCamera = new cCamera;
	m_pCamera->Setup();
	//m_pCamera->SetTarget(&m_pCubeMan->GetPosition());
	m_pCamera->SetTarget(&m_pWoman->GetPosition());

	//폰트 생성
	D3DXFONT_DESC fd;
	ZeroMemory(&fd,sizeof(D3DXFONT_DESC));
	fd.Height			= 50;
	fd.Width			= 25;
	fd.Weight			= FW_MEDIUM;
	fd.Italic			= false;
	fd.CharSet			= DEFAULT_CHARSET;
	fd.OutputPrecision  = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily   = FF_DONTCARE;
	//strcpy_s(fd.FaceName, "궁서체");//글꼴 스타일
	// ttf 사용하기
	AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "umberto");//글꼴 스타일

	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	assert(S_OK == hr);
	
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir = D3DXVECTOR3(1.5, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}


void cMainGame::Update()
{
	if (m_pWoman)
	{
		m_pWoman->Update();
	}
	
	m_pCamera->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//D3DCOLOR_XRGB(47, 121, 112),
		D3DCOLOR_XRGB(33, 40, 48),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);
	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGrid->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_pHeightMap)
	{
		m_pHeightMap->Render();
	}
	if (m_pWoman)
	{
		m_pWoman->Render();
	}
// 	RECT rc;
// 	SetRect(&rc, 100, 100, 101, 101);
// 	char szTemp[1024];
//// 	//sprintf(szTemp, "%d / %d", dwGroup , dwMesh);
//	sprintf(szTemp, "%d", m_nKeyValue);
// 	m_pFont->DrawTextA(NULL, szTemp, strlen(szTemp),
// 		&rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255,0,0));


	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			//m_pCircle1->SetCenter(D3DXVECTOR2(nX, nY));
		}
		
		break;
	case WM_MOUSEWHEEL:
		GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}

void cMainGame::OnActionFinish( cAction* pSender )
{
	int a = 0;
}

