// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifdef WIN64
#ifdef _DEBUG
#pragma comment(lib, "../x64/xDebug/taskUtils.lib")
#else
#pragma comment(lib, "../x64/Release/taskUtils.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../Release/taskUtils.lib")
#else
#pragma comment(lib, "../Debug/taskUtils.lib")
#endif
#endif

// TODO:  在此处引用程序需要的其他头文件
