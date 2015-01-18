#include "stdafx.h"
#include "cInputManiger.h"


cInputManiger::cInputManiger()
{
	memset(m_bKeyCode, 0, sizeof(m_bKeyCode));
	memset(m_bKeyCodeOnce, 0, sizeof(m_bKeyCodeOnce));
}

cInputManiger::~cInputManiger()
{
}

BOOL cInputManiger::GetKeyDown(WPARAM dw_key)
{
	return m_bKeyCode[dw_key];
}

BOOL cInputManiger::GetKeyDownOnce(WPARAM dw_key)
{
	if (m_bKeyCode[dw_key])
	{
		if (!m_bKeyCodeOnce[dw_key])
		{
			return m_bKeyCodeOnce[dw_key] = TRUE;
		}
	}
	return FALSE;
}

void cInputManiger::KeyDown(WPARAM dw_key)
{
	m_bKeyCode[dw_key] = TRUE;
}

void cInputManiger::KeyUp(WPARAM dw_key)
{
	m_bKeyCode[dw_key] = FALSE;
	m_bKeyCodeOnce[dw_key] = FALSE;
}

void cInputManiger::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		KeyDown(wParam);
		break;	
	case WM_KEYUP:
		KeyUp(wParam);		
		break;
	}
}
