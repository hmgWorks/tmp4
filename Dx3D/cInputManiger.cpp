#include "stdafx.h"
#include "cInputManiger.h"


cInputManiger::cInputManiger()
	:m_bKeyDownOnceCheck(FALSE)
{
	memset(m_bKeyCode, 0, sizeof(m_bKeyCode));
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
	if (m_bKeyCode[dw_key] && m_bKeyDownOnceCheck)
	{
		m_bKeyDownOnceCheck = FALSE;
		return TRUE;
	}
	return FALSE;
}

void cInputManiger::KeyDown(WPARAM dw_key)
{
	m_bKeyCode[dw_key] = TRUE;
	if (m_bKeyDownOnceCheck == FALSE)
	{
		m_bKeyDownOnceCheck = TRUE;
	}
}

void cInputManiger::KeyUp(WPARAM dw_key)
{
	m_bKeyCode[dw_key] = FALSE;
	m_bKeyDownOnceCheck = FALSE;
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
