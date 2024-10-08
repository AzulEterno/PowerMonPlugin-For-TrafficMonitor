// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#define UNIT_STR_MAXLEN 12

#define GBS_HASBATTERY 0x1
#define GBS_ONBATTERY  0x2

// 添加要在此处预编译的标头
#include "framework.h"
#include <batclass.h>
#include "resource.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cwchar>
#include <vector>
#include <set>
#include <functional>
#include <map>
#include <iostream>
#include <iomanip>
#include "poclass.h"
#include "ioapiset.h"
#include "windows.h"
#include "WinBase.h"
#include "SetupAPI.h"
#pragma comment (lib, "SetupApi.lib")
#pragma comment(lib, "Version.lib")
//#include "BatClass.h"
#include "devguid.h"
#include "winioctl.h"
//#include "collection.h"
#include "PluginInterface.h"

#ifndef PrintValueStrBufferSize
#define PrintValueStrBufferSize 32
#endif

#define MAX_UINT32 4294967295U

#ifdef MAX_PATH
#undef MAX_PATH
#define MAX_PATH 32767
#endif



#define ENABLE_DEBUG_POP_WINDOWS false


#define WINRT_USE_FLAG (_M_CEE)

#ifdef _M_ARM64X
#define PLATFORM_STR L"ARM64X"
#elif defined(_M_ARM64EC)
#define PLATFORM_STR L"ARM64EC"
#elif defined(_M_ARM64)
#define PLATFORM_STR L"ARM64"
#elif defined(_M_X64) || defined(__amd64__)
#define PLATFORM_STR L"x64"
#elif defined(_M_IX86) || defined(__i386__)
#define PLATFORM_STR L"x86"
#else
#define PLATFORM_STR L"Unknown"
#endif


#define SAFE_DELETE(p) do \
{\
    if(p != nullptr) \
    { \
        delete p; \
        p = nullptr; \
    } \
} while (false)




#endif //PCH_H
