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
public:
	cWoman();
	~cWoman();

	void Setup();
	void Update();
	void Render();

};

