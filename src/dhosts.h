
// hosts.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdhostsApp:
// �йش����ʵ�֣������ hosts.cpp
//

class CdhostsApp : public CWinApp
{
public:
	CdhostsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdhostsApp theApp;