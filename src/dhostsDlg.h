
// hostsDlg.h : ͷ�ļ�
//

#pragma once
#define WM_NOTIFYICON (WM_USER+1001)

// CdhostsDlg �Ի���
class CdhostsDlg : public CDialogEx
{
// ����
public:
	CdhostsDlg(CWnd* pParent = NULL);	// ��׼���캯��

	void showDialog();
	void endDialog();
	void updateIpMap();

// �Ի�������
	enum { IDD = IDD_DHOSTS_DIALOG };
	BOOL DestroyWindow();

protected:
	BOOL bVisible;
	CMap<CString, LPCTSTR, DWORD, DWORD> ipMap;
	BOOL bHostsInterval;//��ʱˢ��HOSTS

	CString sHostpath;//����hosts�ļ�·������OnInitDialog�г�ʼ��
	FILETIME   fHostLastWriteTime;//�ϴ��޸�ʱ��

	void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	HICON m_hIcon;

	NOTIFYICONDATA m_NotifyIconData;//����ͼ��

	HRESULT OnNotifyIcon(WPARAM wParam,LPARAM lParam);//����ͼ��ص�����

	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	void OnClose();
	BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
