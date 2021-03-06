#pragma once

class cFrame;
class cMtlTex;

class cAseLoader
{
private:
	FILE*		m_fp;
	char		m_szToken[1024];
	cFrame*		m_pRootFrame;
	std::string	m_sFolder;

	std::vector<cMtlTex*>			m_vecMtlTex;
	std::map<std::string, cFrame*>	m_mapFrame;

public:
	cAseLoader(void);
	~cAseLoader(void);

	cFrame* Load(std::string& sFolder, std::string& sFileName);
	int nSceneFirstframe, nSceneLastframe, nSceneFramespeed, nSceneTicksperframe;
private:
	char*	GetToken();
	bool	IsWhite(char c);
	bool	IsEqual(char* szStr1, char* szStr2);
	int		GetInteger();
	float	GetFloat();
	void	SkipBlock();
	
	void ProcessScene(int& nFirstframe, int& nLastframe, int& nFramespeed, int& nTicksperframe);
	void	ProcessMaterialList();
	void	ProcessMaterial(cMtlTex* pMtlTex);
	void	ProcessMapDiffuse(cMtlTex* pMtlTex);
	cFrame*	ProcessGeomObject();
	void	ProcessNodeTM(cFrame* pFrame);
	void	ProcessMesh(cFrame* pFrame);
	void	ProcessMeshVertexList(std::vector<D3DXVECTOR3>& vecV);
	void	ProcessMeshFaceList(OUT std::vector<ST_PNT_VERTEX>& vecVertex, IN std::vector<D3DXVECTOR3>& vecV);
	void	ProcessMeshTVertList(std::vector<D3DXVECTOR2>& vecVT);
	void	ProcessMeshTFaceList(OUT std::vector<ST_PNT_VERTEX>& vecVertex, IN std::vector<D3DXVECTOR2>& vecVT);
	void	ProcessMeshNormals(OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessAnimation(cFrame* pFrame);
	void ProcessControlPosTrack(cFrame* pFrame);
	void ProcessControlRotTrack(cFrame* pFrame);
	void ProcessControlRotTcb(cFrame* pFrame);
	
};

