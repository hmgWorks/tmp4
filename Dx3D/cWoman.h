#pragma once
class cFrame;

class cWoman
{
private:
	enum ANI{ IDLE = 0, WALKING = 1 };
	std::vector<cFrame*>	m_vecFrame;
	cFrame*					m_pCurrentFrame;
	ANI						m_nCurrentANI;
	int m_nKeyValue;
	int m_nOld, m_nCurt;

	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3		m_vForward;
	D3DXMATRIXA16	m_matWorld;

	float			m_fAngle;

public:
	cWoman();
	~cWoman();
	D3DXVECTOR3& GetPosition(){ return m_vPosition; }
	void Setup();
	void Update();
	void Render();

};

