// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


HANDLE hFile = 0;

#define DRIVER_NAME L"DriverCE"
#define DRIVER_PATH L"DriverCE.sys"
BOOL installDvr(CONST WCHAR drvPath[50], CONST WCHAR serviceName[20]) {

	WCHAR szFullPathDriverName[MAX_PATH] = { 0 };

	GetFullPathNameW(drvPath, MAX_PATH, szFullPathDriverName, 0);
	MessageBoxW(0, szFullPathDriverName, 0, 0);
	// 打开服务控制管理器数据库
	SC_HANDLE schSCManager = OpenSCManager(
		NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
		NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
		SC_MANAGER_ALL_ACCESS   // 所有权限
	);
	if (schSCManager == NULL) {

	
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	// 创建服务对象，添加至服务控制管理器数据库
	SC_HANDLE schService = CreateServiceW(
		schSCManager,               // 服务控件管理器数据库的句柄
		serviceName,                // 要安装的服务的名称
		serviceName,                // 用户界面程序用来标识服务的显示名称
		SERVICE_ALL_ACCESS,         // 对服务的访问权限：所有全权限
		SERVICE_KERNEL_DRIVER,      // 服务类型：驱动服务
		SERVICE_DEMAND_START,       // 服务启动选项：进程调用 StartService 时启动
		SERVICE_ERROR_IGNORE,       // 如果无法启动：忽略错误继续运行
		szFullPathDriverName,                    // 驱动文件绝对路径，如果包含空格需要多加双引号
		NULL,                       // 服务所属的负载订购组：服务不属于某个组
		NULL,                       // 接收订购组唯一标记值：不接收
		NULL,                       // 服务加载顺序数组：服务没有依赖项
		NULL,                       // 运行服务的账户名：使用 LocalSystem 账户
		NULL                        // LocalSystem 账户密码
	);
	if (schService == NULL) {

		//MessageBox(0, 0, 0, 0);
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return TRUE;
}

// 启动服务
BOOL startDvr(CONST WCHAR serviceName[20]) {

	// 打开服务控制管理器数据库
	SC_HANDLE schSCManager = OpenSCManager(
		NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
		NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
		SC_MANAGER_ALL_ACCESS   // 所有权限
	);
	if (schSCManager == NULL) {
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	// 打开服务
	SC_HANDLE hs = OpenServiceW(
		schSCManager,           // 服务控件管理器数据库的句柄
		serviceName,            // 要打开的服务名
		SERVICE_ALL_ACCESS      // 服务访问权限：所有权限
	);
	if (hs == NULL) {
		CloseServiceHandle(hs);
		CloseServiceHandle(schSCManager);
		return FALSE;
	}
	if (StartService(hs, 0, 0) == 0) {
		CloseServiceHandle(hs);
		CloseServiceHandle(schSCManager);
		return FALSE;
	}


	CloseServiceHandle(hs);
	CloseServiceHandle(schSCManager);
	return TRUE;
}

void DetourUnHook(void** lpOldFunc, PVOID lpHookFunc) {

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourDetach(lpOldFunc, lpHookFunc);

	DetourTransactionCommit();
}

void DetourHook(void** lpOldFunc, PVOID lpHookFunc) {

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(lpOldFunc, lpHookFunc);

	DetourTransactionCommit();
}




BOOL MyWriteProcessMemory(
	HANDLE hProcess,               
	LPVOID lpBaseAddress,          
	LPVOID lpBuffer,               
	DWORD nSize,                   
	LPDWORD lpNumberOfBytesWritten){

	WRITEDATA data{ 0 };

	data.hProcess = hProcess;
	data.lpBaseAddress = lpBaseAddress;
	data.lpBuffer = lpBuffer;
	data.lpNumberOfBytesWritten = (PDWORD32)lpNumberOfBytesWritten;
	data.nSize = nSize;


	return DeviceIoControl(hFile, CTL_CODE_WRITE, &data, sizeof(WRITEDATA), 0, 0, 0, 0);

}


BOOL MyReadProcessMemory(
	HANDLE hProcess,
	LPVOID lpBaseAddress,
	LPVOID lpBuffer,
	DWORD nSize,
	LPDWORD lpNumberOfBytesRead) {


	READDATA data{ 0 };

	data.hProcess = hProcess;
	data.lpBaseAddress = lpBaseAddress;
	data.lpBuffer = lpBuffer;
	data.lpNumberOfBytesRead = (PDWORD32)lpNumberOfBytesRead;
	data.nSize = nSize;

	//ERR
	return DeviceIoControl(hFile, CTL_CODE_READ, &data, sizeof(WRITEDATA), 0, 0, 0, 0);


}


BOOL Hook() {

	UINT64 hkAddr = (UINT64)WriteProcessMemory;

	DetourHook((void**)&hkAddr, MyWriteProcessMemory);

	hkAddr = (UINT64)ReadProcessMemory;

	DetourHook((void**)&hkAddr, MyReadProcessMemory);

	return true;

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {

		init();//初始化劫持Version.dll的函数

		bool bOk = installDvr(DRIVER_PATH, DRIVER_NAME);
		if (!bOk) {
			MessageBox(0, "LoadDriverErr", 0, 0);
			return false;

		}
		bOk = startDvr(DRIVER_NAME);
		if (!bOk) {
			MessageBox(0, "StartDriverErr", 0, 0);
			return false;

		}

		hFile = CreateFile("\\\\.\\DriverCE", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

		if (hFile == INVALID_HANDLE_VALUE) {

			MessageBox(0, "failed to create file", "err", MB_OK | MB_ICONERROR);

			return false;
		}

		return Hook();


    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

