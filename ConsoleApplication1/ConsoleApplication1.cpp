// ConsoleApplication1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream pFile("HeightMap.raw", std::ios::in | std::ios::binary);
	pFile.seekg(0, std::ios::end);   // ����ġ
	long fileSize = pFile.tellg();  // ���ϻ�����
	pFile.seekg(0, std::ios::beg);   // �ٽ� ��������

	unsigned char* buffer = new unsigned char[fileSize];
	pFile.read((char*)buffer, fileSize);
	pFile.close();
	cout << fileSize << endl;
	//cout << (float)buffer[1] << endl;
	//cout << (float)buffer[2] << endl;
	//cout << (float)buffer[3] << endl;
	//cout << (float)buffer[4] << endl;

	delete[] buffer; // �޸� ����
	return 0;
}

