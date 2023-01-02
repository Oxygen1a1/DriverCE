.code
;这个汇编文件用于中转，将假的dll文件导出函数jmp到真正的versiondll
;win10 x64版本
 extern   pGetFileVersionInfoA:far
 extern   pGetFileVersionInfoByHandle:far
 extern   pGetFileVersionInfoExA:far;win10 专有
 extern   pGetFileVersionInfoExW:far
 extern   pGetFileVersionInfoSizeA:far
 extern   pGetFileVersionInfoSizeExA:far ;win10专有？
 extern   pGetFileVersionInfoSizeExW:far
 extern   pGetFileVersionInfoSizeW:far
 extern   pGetFileVersionInfoW:far
 extern   pVerFindFileA:far
 extern   pVerFindFileW:far
 extern   pVerInstallFileA:far
 extern   pVerInstallFileW:far
 extern   pVerLanguageNameA:far
 extern   pVerLanguageNameW:far
 extern   pVerQueryValueA:far
 extern   pVerQueryValueW:far
 

GetFileVersionInfoA proc
jmp  qword ptr[pGetFileVersionInfoA]
ret
GetFileVersionInfoA endp

 
GetFileVersionInfoByHandle proc
jmp qword ptr[pGetFileVersionInfoByHandle]
ret
GetFileVersionInfoByHandle endp
 

 
GetFileVersionInfoExA proc
jmp  qword ptr[pGetFileVersionInfoExA]
ret
GetFileVersionInfoExA endp
 

GetFileVersionInfoExW proc
jmp  qword ptr[pGetFileVersionInfoExW]
ret
GetFileVersionInfoExW endp

GetFileVersionInfoSizeA proc
jmp  qword ptr[pGetFileVersionInfoSizeA]
ret
GetFileVersionInfoSizeA endp


GetFileVersionInfoSizeExA proc
jmp  qword ptr[pGetFileVersionInfoSizeExA]
ret
GetFileVersionInfoSizeExA endp

GetFileVersionInfoSizeExW proc
jmp  qword ptr[pGetFileVersionInfoSizeExW]
ret
GetFileVersionInfoSizeExW endp

GetFileVersionInfoSizeW proc
jmp  qword ptr[pGetFileVersionInfoSizeW]
ret
GetFileVersionInfoSizeW endp

GetFileVersionInfoW proc
jmp  qword ptr[pGetFileVersionInfoW]
ret
GetFileVersionInfoW endp


VerFindFileA proc
jmp  qword ptr[pVerFindFileA]
ret
VerFindFileA endp


VerFindFileW proc
jmp  qword ptr[pVerFindFileW]
ret
VerFindFileW endp

VerInstallFileA proc
jmp  qword ptr[pVerInstallFileA]
ret
VerInstallFileA endp


VerInstallFileW proc
jmp  qword ptr[pVerInstallFileW]
ret
VerInstallFileW endp

VerLanguageNameA proc
jmp   qword ptr[pVerLanguageNameA]
ret
VerLanguageNameA endp

VerLanguageNameW proc
jmp  qword ptr[pVerLanguageNameW]
ret
VerLanguageNameW endp


VerQueryValueA proc
jmp  qword ptr[pVerQueryValueA]
ret
VerQueryValueA endp

VerQueryValueW proc
jmp  qword ptr[pVerQueryValueW]
ret
VerQueryValueW endp


end