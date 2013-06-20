/*
dHosts is released under the MIT license:

Copyright (c) 2012 Yanis Wang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
// hosts.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "dhosts.h"
#include "dhostsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdhostsApp

BEGIN_MESSAGE_MAP(CdhostsApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CdhostsApp ����

CdhostsApp::CdhostsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CdhostsApp ����

CdhostsApp theApp;


// CdhostsApp ��ʼ��

BOOL CdhostsApp::InitInstance()
{
	CreateMutex(NULL,true,m_pszAppName); 
	if(GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		//AfxMessageBox(_T("Ӧ�ó��򲻿����ظ�����"),MB_OK | MB_APPLMODAL | MB_ICONSTOP); 
		return(false); 
	}
	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	CdhostsDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

