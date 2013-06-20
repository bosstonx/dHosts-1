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
// hostsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dhosts.h"
#include "dhostsDlg.h"
#include "afxdialogex.h"
#include "shlobj.h"


#pragma comment(lib,"hostsdll.lib")
#pragma comment(lib,"shell32.lib")

#import "RegExp.tlb"
using namespace VBScript_RegExp_55;

int initHook(HWND hWnd,TCHAR* username);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CdhostsDlg::CdhostsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdhostsDlg::IDD, pParent)
{
	bVisible = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if(!AfxOleInit())MessageBox(_T("Ole Initial Failure"));
}

void CdhostsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdhostsDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_COPYDATA()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_NOTIFYICON,&CdhostsDlg::OnNotifyIcon)
	ON_WM_CLOSE()
	ON_COMMAND(ID_AppAbout, &CdhostsDlg::showDialog)
	ON_COMMAND(ID_AppExit, &CdhostsDlg::endDialog)
END_MESSAGE_MAP()


void CdhostsDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
    if(!bVisible)lpwndpos->flags &= ~SWP_SHOWWINDOW;

    CDialogEx::OnWindowPosChanging(lpwndpos);
}

BOOL CdhostsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ȡ����·��
	TCHAR szDeskPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szDeskPath, CSIDL_DESKTOP, FALSE);
	sHostpath.Format(_T("%s\\hosts.txt"),szDeskPath);

	//����IP��
	updateIpMap();

	//��ʼ��Hook
	const int nBufSize = 256;
	TCHAR tcUsername[nBufSize];
	ZeroMemory(tcUsername,nBufSize);
	DWORD dwRet = nBufSize;
	GetUserName(tcUsername,&dwRet);

	initHook(m_hWnd,tcUsername);

	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//���ϵͳ��ͼ��
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    m_NotifyIconData.hWnd = m_hWnd;
    m_NotifyIconData.uID = IDR_MAINFRAME;
    m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    m_NotifyIconData.uCallbackMessage = WM_NOTIFYICON;
	m_NotifyIconData.dwInfoFlags = NIIF_USER;

    m_NotifyIconData.hIcon = m_hIcon;
	wcscpy(m_NotifyIconData.szTip, _T("dHosts"));

    wcscpy(m_NotifyIconData.szInfoTitle, _T(""));
	wcscpy(m_NotifyIconData.szInfo, _T("�����ڹ����ڶ��û�dHostsģʽ�¡�\r\n���޸������ϵ�hosts.txt�ļ���"));
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);

	//������ʱ��
	SetTimer(0,1000,NULL);
	SetTimer(1,10000,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CdhostsDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{

	CString m_strCopyData=(LPCWSTR)pCopyDataStruct->lpData;
    m_strCopyData=m_strCopyData.Left(pCopyDataStruct->cbData/2);

	DWORD nValue = 0;
	ipMap.Lookup(m_strCopyData.MakeLower(), nValue);

	return nValue;
}

//��ʾ�Ի���
void CdhostsDlg::showDialog()
{
	bVisible = true;
	ShowWindow(SW_SHOW);
}

//�����Ի���
void CdhostsDlg::endDialog()
{
	PostQuitMessage(0);
}

//��ʱ��
void CdhostsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{//����ļ��޸�
		FILETIME fNewHostLastWriteTime;
		HANDLE hFile = CreateFile(sHostpath, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE,NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);
		GetFileTime(hFile,NULL,NULL,&fNewHostLastWriteTime);
		CloseHandle(hFile);
		if(CompareFileTime(&fNewHostLastWriteTime,&fHostLastWriteTime)!=0){
			updateIpMap();
		}
	}
	else if(nIDEvent ==1)
	{
		if(bHostsInterval)updateIpMap();
	}

}

//����IPӳ���
void CdhostsDlg::updateIpMap()
{
	//���ļ�
	CStdioFile file;
	file.Open(sHostpath,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	CString szLine;
	CArray<CString,CString&> arrFile;
	//��ȡ�ļ�����������
	while( file.ReadString( szLine ) )
	{
		arrFile.Add(szLine);
	}
	file.Close();
	
	HANDLE hFile = CreateFile(sHostpath, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE,NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);
	GetFileTime(hFile,NULL,NULL,&fHostLastWriteTime);
	CloseHandle(hFile);

	//���ipMap
	ipMap.RemoveAll();

	IRegExpPtr pRegExp(__uuidof(RegExp)),pRegExpIp(__uuidof(RegExp)),pRegExpHost(__uuidof(RegExp));
	pRegExp->PutGlobal(VARIANT_TRUE);
	pRegExp->PutPattern("^\\s*([^\\s#]+)\\s+([^#]+)");
	pRegExpIp->PutPattern("^(\\d{1,3}\\.){3}\\d{1,3}$");
	pRegExpHost->PutGlobal(VARIANT_TRUE);
	pRegExpHost->PutPattern("([^\\s]+)\\s*");
	IMatchCollection2Ptr pMatchs ;
	ISubMatchesPtr pMatch;

	//��ʼ��socket
	WSADATA WSAData;
	WSAStartup(MAKEWORD(1,1), &WSAData);

	//����ת���ַ���
	CString szHostname,szHostIp;
	PHOSTENT phe; 
	struct in_addr addr;
	bHostsInterval = false;
	int hostCount;
	int lineCount=arrFile.GetSize();
	for(int i=0;i<lineCount;i++) 
	{
		szLine=arrFile[i];
		pMatchs = pRegExp->Execute(szLine.AllocSysString());
		if(pMatchs->GetCount()>0){
			pMatch = ((IMatch2Ptr)pMatchs->Item[0])->GetSubMatches();
			szHostname=pMatch->GetItem(1).bstrVal;
			szHostIp = pMatch->GetItem(0).bstrVal;
			if(!pRegExpIp->Test((_bstr_t)szHostIp))
			{
				bHostsInterval = true;
				phe = gethostbyname((CStringA)szHostIp); 
				if(phe != NULL)
				{
					addr.s_addr = *(u_long *) phe->h_addr_list[0];
					szHostIp = inet_ntoa(addr);
				}
				else szHostIp="";
			}
			if(szHostIp.GetLength()>0)
			{
				pMatchs = pRegExpHost->Execute(szHostname.AllocSysString());
				hostCount = pMatchs->GetCount();
				for(int i=0;i<hostCount;i++)
				{
					pMatch = ((IMatch2Ptr)pMatchs->Item[i])->GetSubMatches();
					szHostname=pMatch->GetItem(0).bstrVal;
					ipMap.SetAt(szHostname.MakeLower(),inet_addr((CStringA)szHostIp));
				}				
			}
		}
	}

	WSACleanup();

}

HRESULT CdhostsDlg::OnNotifyIcon(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
		case WM_LBUTTONDOWN:
			{
				showDialog();
			}
			break;
		case WM_RBUTTONDOWN:
			{
				CMenu menu;
				menu.LoadMenu (IDR_MENU1);
				CMenu* pContextMenu = menu.GetSubMenu (0);
				POINT point;
				GetCursorPos(&point);
				pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd ());
			}
			break;
		default:
			break;
	}
	return 0L;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdhostsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdhostsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdhostsDlg::OnOK()
{
}

void CdhostsDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CdhostsDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

BOOL CdhostsDlg::DestroyWindow()
{
    // ��������ɾ��ͼ��
    Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
    return CDialogEx::DestroyWindow();
}
