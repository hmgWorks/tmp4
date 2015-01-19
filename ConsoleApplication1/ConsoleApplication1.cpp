// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream pFile("HeightMap.raw", std::ios::in | std::ios::binary);
	pFile.seekg(0, std::ios::end);   // 끝위치
	long fileSize = pFile.tellg();  // 파일사이즈
	pFile.seekg(0, std::ios::beg);   // 다시 시작으로

	unsigned char* buffer = new unsigned char[fileSize];
	pFile.read((char*)buffer, fileSize);
	pFile.close();
	cout << fileSize << endl;
	//cout << (float)buffer[1] << endl;
	//cout << (float)buffer[2] << endl;
	//cout << (float)buffer[3] << endl;
	//cout << (float)buffer[4] << endl;

	delete[] buffer; // 메모리 해제
	return 0;
}

