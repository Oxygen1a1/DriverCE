#pragma once

#define CTL_CODE_WRITE CTL_CODE(0x8000,0x801,0,0)
#define CTL_CODE_READ CTL_CODE(0x8000,0x802,0,0)


EXTERN_C NTSTATUS  MmCopyVirtualMemory(PEPROCESS FromProcess, PVOID FromAddress,
	PEPROCESS ToProcess, PVOID ToAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode,
	PSIZE_T NumberOfBytesCopied);


typedef struct _WRITEDATA
{
	HANDLE hProcess;              // handle to process
	PVOID lpBaseAddress;          // base of memory area
	PVOID lpBuffer;               // data buffer
	DWORD32 nSize;                  // number of bytes to write
	PDWORD32 lpNumberOfBytesWritten; // number of bytes writtenI
}WRITEDATA,*PWRITEDTA;


typedef struct _READDATA {
	HANDLE hProcess;              // handle to process
	PVOID lpBaseAddress;          // base of memory area
	PVOID lpBuffer;               // data buffer
	DWORD32 nSize;                  // number of bytes to write
	PDWORD32 lpNumberOfBytesRead; // number of bytes writtenI

}READDATA,*PREADDATA;