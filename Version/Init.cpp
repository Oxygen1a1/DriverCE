//����������ڳ�ʼ������version.dll��ָ��
#include "pch.h"
#include <Windows.h>
#include "Func.h"



PVOID GetVersionPointer(const char* szFuncName) {
	static char szSystem32Dir[MAX_PATH];

	GetSystemDirectory(szSystem32Dir, MAX_PATH);//��ȡsystem32ϵͳĿ¼
	strcat_s(szSystem32Dir, MAX_PATH, "\\Version.dll");
	static HMODULE hVerson = LoadLibraryA(szSystem32Dir);
	PVOID pRetAddress = GetProcAddress(hVerson, szFuncName);
	return pRetAddress;
}

extern "C"//һ��Ҫ��extern "c"��ʼ����Щȫ�ֺ���ָ�� cpp����Լ������
{
	PVOID pGetFileVersionInfoA;
	PVOID pGetFileVersionInfoByHandle;
	PVOID pGetFileVersionInfoExA; //win10ר�� 
	PVOID pGetFileVersionInfoExW;
	PVOID pGetFileVersionInfoSizeA;
	PVOID pGetFileVersionInfoSizeExA; //win10ר�� 
	PVOID pGetFileVersionInfoSizeExW;
	PVOID pGetFileVersionInfoSizeW;
	PVOID pGetFileVersionInfoW;
	PVOID pVerFindFileA;
	PVOID pVerFindFileW;
	PVOID pVerInstallFileA;
	PVOID pVerInstallFileW;
	PVOID pVerLanguageNameA;
	PVOID pVerLanguageNameW;
	PVOID pVerQueryValueA;
	PVOID pVerQueryValueW;
}
void init() {
	pGetFileVersionInfoA = GetVersionPointer("GetFileVersionInfoA");
	pGetFileVersionInfoByHandle = GetVersionPointer("GetFileVersionInfoByHandle");
	pGetFileVersionInfoExA = GetVersionPointer("GetFileVersionInfoExA"); //win10ר�� 
	pGetFileVersionInfoExW = GetVersionPointer("GetFileVersionInfoExW");
	pGetFileVersionInfoSizeA = GetVersionPointer("GetFileVersionInfoSizeA");
	pGetFileVersionInfoSizeExA = GetVersionPointer("GetFileVersionInfoSizeExA"); //win10ר�� 
	pGetFileVersionInfoSizeExW = GetVersionPointer("GetFileVersionInfoSizeExW");
	pGetFileVersionInfoSizeW = GetVersionPointer("GetFileVersionInfoSizeW");
	pGetFileVersionInfoW = GetVersionPointer("GetFileVersionInfoW");
	pVerFindFileA = GetVersionPointer("VerFindFileA");
	pVerFindFileW = GetVersionPointer("VerFindFileW");
	pVerInstallFileA = GetVersionPointer("VerInstallFileA");
	pVerInstallFileW = GetVersionPointer("VerInstallFileW");
	pVerLanguageNameA = GetVersionPointer("VerLanguageNameA");
	pVerLanguageNameW = GetVersionPointer("VerLanguageNameW");
	pVerQueryValueA = GetVersionPointer("VerQueryValueA");
	pVerQueryValueW = GetVersionPointer("VerQueryValueW");
}