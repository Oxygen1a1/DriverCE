//这个函数用于初始化真正version.dll的指针
#include "pch.h"
#include <Windows.h>
#include "Func.h"



PVOID GetVersionPointer(const char* szFuncName) {
	static char szSystem32Dir[MAX_PATH];

	GetSystemDirectory(szSystem32Dir, MAX_PATH);//获取system32系统目录
	strcat_s(szSystem32Dir, MAX_PATH, "\\Version.dll");
	static HMODULE hVerson = LoadLibraryA(szSystem32Dir);
	PVOID pRetAddress = GetProcAddress(hVerson, szFuncName);
	return pRetAddress;
}

extern "C"//一定要有extern "c"初始化这些全局函数指针 cpp函数约定问题
{
	PVOID pGetFileVersionInfoA;
	PVOID pGetFileVersionInfoByHandle;
	PVOID pGetFileVersionInfoExA; //win10专有 
	PVOID pGetFileVersionInfoExW;
	PVOID pGetFileVersionInfoSizeA;
	PVOID pGetFileVersionInfoSizeExA; //win10专有 
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
	pGetFileVersionInfoExA = GetVersionPointer("GetFileVersionInfoExA"); //win10专有 
	pGetFileVersionInfoExW = GetVersionPointer("GetFileVersionInfoExW");
	pGetFileVersionInfoSizeA = GetVersionPointer("GetFileVersionInfoSizeA");
	pGetFileVersionInfoSizeExA = GetVersionPointer("GetFileVersionInfoSizeExA"); //win10专有 
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