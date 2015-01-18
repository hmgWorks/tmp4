#pragma once
#define g_pInputManager cInputManiger::GetInstance()

class cInputManiger
{
	SINGLETONE(cInputManiger);

private:
	BOOL m_bKeyCode[256];
	BOOL m_bKeyCodeOnce[256];
	//previous
private:
	void KeyDown(WPARAM dw_key);	
	void KeyUp(WPARAM dw_key);

public:	
	BOOL GetKeyDown(WPARAM dw_key);
	BOOL GetKeyDownOnce(WPARAM dw_key);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

