// PassWordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PassWordDlg.h"
#include "MSCUKeyAPI.h"

//#include "CVSKF.h"

// CPassWordDlg �Ի���

IMPLEMENT_DYNAMIC(CPassWordDlg, CDialog)

CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWordDlg::IDD, pParent)
	, m_strPassWord(_T(""))
{

}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPassWord);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPassWordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassWordDlg ��Ϣ�������

void CPassWordDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPTSTR 		UserPin;
	int			UserPinLen;
	ULONG			trytime;
	CString		str;
	DWORD		dwRet = 0;

	UpdateData(TRUE);
	CStatic *Lable=(CStatic*)GetDlgItem(IDC_STATIC_INFO);
	UserPin=(LPTSTR)m_strPassWord.GetBuffer(0);
	UserPinLen=(int)m_strPassWord.GetLength();

	dwRet = MSC_VerifyUserPIN((BYTE *)UserPin,UserPinLen);
	if(dwRet== 0x9000)
	{
		OnOK();
	}	
	else
	{
		if(dwRet == 0x6983)
		{
			Lable->SetWindowText("�û�PIN���Ѿ�������������ϵ����Ա");
		}
		else if((dwRet & 0xFF00) == 0x6300)
		{
					
			if(dwRet == 0x63C0)
			{
				Lable->SetWindowText("�û�PIN��֤ʧ�ܣ��ɳ��Դ����Ѵ����ޣ�����ϵ����Ա");
				((CButton*)GetDlgItem(IDOK))->EnableWindow(0);
			}
			else
			{
				trytime = (dwRet & 0xF);
				str.Format("%d",trytime);
				Lable->SetWindowText("��֤�û�PINʧ�ܣ�ʣ��"+str+"�γ��Ի���!");
			}
		}
		else
		{
					
			Lable->SetWindowText("��֤�û�PINִ�в��ɹ�!");		
		}
	}

    ((CEdit*)GetDlgItem(IDC_EDIT_PW))->SetWindowText("");

	return ;
}
