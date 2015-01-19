#pragma once
#define HEIGHT_SIZE 257
class cHeightMap
{
private:
	std::vector<ST_PNT_VERTEX>		m_vecVertex;
	//std::vector<std::vector<int>>	m_vecIndex;
	std::vector<ST_INDEX>	m_vecIndex;

	//LPD3DXMESH	m_pMesh;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexture;
public:
	cHeightMap();
	~cHeightMap();

	//raw������ �о ���ý��� �����.
	void Load(std::string file);
	void Render();
};

