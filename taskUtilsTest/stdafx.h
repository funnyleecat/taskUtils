// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
