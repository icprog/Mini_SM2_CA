#pragma once
#include "Mini_SM2_CA.h"


// CPassWordDlg �Ի���

class CPassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassWordDlg();

// �Ի�������
	enum { IDD = IDD_PW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassWord;
	afx_msg void OnBnClickedOk();
};
