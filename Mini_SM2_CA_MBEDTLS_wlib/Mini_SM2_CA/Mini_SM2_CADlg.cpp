// Mini_SM2_CADlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Mini_SM2_CA.h"
#include "Mini_SM2_CADlg.h"
#include "PassWordDlg.h"
#include <windows.h>
//#include <WinCrypt.h>

//#include "sm2.h"
//#include "sm3.h"
#include "IS_Base64.h"
//#include "sm2p10.h"
//#include "sm2cert.h"
//
#include "MSCUKeyAPI.h"
//
//#include "CVSKF.h"


#include "cvm_cert.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/x509.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

LPSTR MSACSP="DeCard CSP v2.0";
char Container[65] = {0};

int usep10 = 1;
int usegen = 1;
int usepin = 0;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMini_SM2_CADlg �Ի���




CMini_SM2_CADlg::CMini_SM2_CADlg(CWnd* pParent /*=NULL*/)
: CDialog(CMini_SM2_CADlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMini_SM2_CADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMini_SM2_CADlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GENROOTCER, &CMini_SM2_CADlg::OnBnClickedButtonGenrootcer)
	ON_BN_CLICKED(IDC_BUTTON_GENUSERCER, &CMini_SM2_CADlg::OnBnClickedButtonGenusercer)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTCERT, &CMini_SM2_CADlg::OnBnClickedButtonImportcert)
	ON_BN_CLICKED(IDC_BUTTON_VERIFYP10, &CMini_SM2_CADlg::OnBnClickedButtonVerifyp10)
	ON_BN_CLICKED(IDC_BUTTON_GENUSERP10, &CMini_SM2_CADlg::OnBnClickedButtonGenuserp10)
	ON_BN_CLICKED(IDC_BUTTON_GENROOTKEY, &CMini_SM2_CADlg::OnBnClickedButtonGenrootkey)
	ON_BN_CLICKED(IDC_BUTTON_GENROOTP10, &CMini_SM2_CADlg::OnBnClickedButtonGenrootp10)
	ON_BN_CLICKED(IDC_CHECK1, &CMini_SM2_CADlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMini_SM2_CADlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_GENUSERKEY, &CMini_SM2_CADlg::OnBnClickedButtonGenuserkey)
	ON_BN_CLICKED(IDC_CHECK3, &CMini_SM2_CADlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CMini_SM2_CADlg ��Ϣ�������

BOOL CMini_SM2_CADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//char *pTestP10 = "MIIBFTCBuwIBADBbMQ0wCwYDVQQGHgQAQwBOMSEwHwYDVQQKHhgAQwBGAEMAQQAgAFQARQBTAFQAIABDAEExJzAlBgNVBAMeHgBjAGUAcgB0AFIAZQBxAHUAaQBzAGkAdABpAG8AbjBZMBMGByqGSM49AgEGCCqBHM9VAYItA0IABOpzXUiLgVmob7cje/x7n3gW6GxmkE1BJQyvEsePcLhhYdlXCx/gSjhWsHISyqbkN3d9m/Tta8zf03eJpQVIDzQwDAYIKoEcz1UBg3UFAANHADBEAiB5AqZzCahcnJ0rtOvjNqrAoNR12kgEpKpLEtHfnKmBNgIgSD545I1iCD/Rrieod75W0ShDKQtKFq4zyTU1KXDGj80=";
	//((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(pTestP10);
	((CEdit*)GetDlgItem(IDC_EDIT_ND))->SetWindowText("CN=USER,O=TEST,C=CN");
	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->SetWindowText("FEDCBA9876543210");
	((CEdit*)GetDlgItem(IDC_EDIT_NOTBEFOR))->SetWindowText("20170101000000");
	((CEdit*)GetDlgItem(IDC_EDIT_NOTAFTER))->SetWindowText("20270101000000");


	((CButton*)GetDlgItem(IDC_BUTTON_GENROOTKEY))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_GENROOTP10))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_GENROOTCER))->EnableWindow(false);

	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(true);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMini_SM2_CADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMini_SM2_CADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CMini_SM2_CADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMini_SM2_CADlg::OnBnClickedButtonGenrootkey()
{
	int ret;
	CVM_ECP_GEN_OPT opt;

	unsigned char pkey[500];
	int len;
	char obuf[1000] = { 0 };
	int olen;

	opt.pri_file = "SM2Root.prikey";
	opt.pub_file = "SM2Root.pubkey";

	ret = cvm_ecp_gen_key(&opt, pkey, &len, NULL, NULL);
	if (ret)
		MessageBox("������Կ��ʧ��!");
	else
		MessageBox("������Կ�Գɹ�!\n˽Կ�ļ���SM2Root.prikey\n��Կ�ļ���SM2Root.pubkey");

	IS_Base64Encode((char*)pkey, len, (char*)obuf, &olen, true);
	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(obuf);
}


void CMini_SM2_CADlg::OnBnClickedButtonGenrootp10()
{
	int ret;
	CVM_CSR_WRITE_OPT opt;
	char subject_name[200];
	
	unsigned char creq[500];
	int len;
	char obuf[1000] = { 0 };
	int olen;

	len = ((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowTextLength();
	if ( (len > sizeof(subject_name) ) || (len <= 0))
	{
		MessageBox("ʹ�����������벻��ȷ!");
		return;
	}
	memset(subject_name, 0, sizeof(subject_name));
	((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowText(subject_name, sizeof(subject_name));

	opt.ext_sign = 0;
	opt.subject_key = "SM2Root.prikey";
	opt.subject_name = subject_name;
	opt.subject_pwd = NULL;
	opt.key_usage = 0;
	opt.ns_cert_type = 0;
	opt.output_file = "SM2Root.req";

	ret = cvm_csr_write_from_opt(&opt, creq, &len);
	if (ret)
		MessageBox("��֤��P10����ʧ��!");
	else
		MessageBox("��֤��P10���ɳɹ�!\n�����ļ���SM2Root.req");

	IS_Base64Encode((char*)creq, len, (char*)obuf, &olen, true);

	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(obuf);
}

void CMini_SM2_CADlg::OnBnClickedButtonGenrootcer()
{
	int ret;
	CVM_CRT_WRITE_OPT opt;


	unsigned char cert[500];
	int len;
	char obuf[1000] = { 0 };;
	int olen;

	char serial[20];
	char not_befor[20];
	char not_after[20];
	char subject_name[200];
	int slen;

	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->GetWindowText(serial, sizeof(serial));
	((CEdit*)GetDlgItem(IDC_EDIT_NOTBEFOR))->GetWindowText(not_befor, sizeof(not_befor));
	((CEdit*)GetDlgItem(IDC_EDIT_NOTAFTER))->GetWindowText(not_after, sizeof(not_after));

	slen = ((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowTextLength();
	if ((slen > sizeof(subject_name)) || (slen <= 0))
	{
		MessageBox("ʹ�����������벻��ȷ!");
		return;
	}
	memset(subject_name, 0, sizeof(subject_name));
	((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowText(subject_name, sizeof(subject_name));


	opt.serial = serial;
	opt.not_befor = not_befor;
	opt.not_after = not_after;
	opt.issuer_key = "SM2Root.prikey";
	opt.issuer_pwd = NULL;
	opt.selfsign = 1;
	if (usep10)
		opt.request_file = "SM2Root.req";
	else
		opt.request_file = NULL;
	opt.subject_name = subject_name;
	opt.subject_pubkey = "SM2Root.pubkey";
	opt.is_ca = 1;
	opt.max_pathlen = -1;
	opt.key_usage = 0;
	opt.ns_cert_type = 0;
	opt.output_file = "SM2Root.cer";

	ret = cvm_crt_write_from_opt(&opt, cert, &len);
	if (ret)
		MessageBox("��֤������ʧ��!");
	else
		MessageBox("��֤�����ɳɹ�!\n֤���ļ���""SM2Root.cer");

	IS_Base64Encode((char*)cert, len, (char*)obuf, &olen, true);

	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(obuf);
}

void CMini_SM2_CADlg::OnBnClickedButtonVerifyp10()
{
	int ret;
	CString err;

	char ibuf[1024];
	int ilen;

	char creq[1024];
	int rlen;

	ilen = ((CEdit*)GetDlgItem(IDC_EDIT_CER))->GetWindowTextLength();
	if ( (ilen > sizeof(ibuf)) || (ilen <= 0))
	{
		MessageBox("�������ݳ��Ȳ���ȷ!");
		return;
	}
	((CEdit*)GetDlgItem(IDC_EDIT_CER))->GetWindowText(ibuf, sizeof(ibuf));

	IS_Base64Decode(ibuf, ilen, creq, &rlen);

	ret = cvm_csr_verify((unsigned char*)creq, rlen);
	if (ret)
	{
		err.Format("%08X", ret);
		MessageBox("֤�������ļ���֤ʧ�ܣ������룺" + err);
		return;
	}
	else
		MessageBox("֤�������ļ���֤�ɹ�!\n");
}

void CMini_SM2_CADlg::OnBnClickedButtonGenuserkey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	CString err;
	BYTE pubkey[65];
	UINT plen;

	if (ret = MSC_ConnectReader())
	{
		MessageBox("�����USBKey!");
		return;
	}

	if (usegen)
	{
		CPassWordDlg KeypwDlg;
		if (KeypwDlg.DoModal() != IDOK)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText("");
			MessageBox("�û�������֤ʧ�ܣ�");
			usepin = 0;
			return;
		}
		usepin = 1;

		ret = MSC_SM2GenKey();
		if (ret != 0x9000)
		{
			err.Format("%08X", ret);
			MessageBox("�û���Կ�Բ���ʧ�ܣ������룺" + err);
			return;
		}
	}

	pubkey[0] = 0x04;
	ret = MSC_SM2ExportPubKey(pubkey + 1, &plen);
	if (ret != 0x9000)
	{
		err.Format("%08X", ret);
		MessageBox("�û���Կ����ʧ�ܣ������룺" + err);
		return;
	}

	ret = cvm_pk_write_pubkey_file("SM2User.pubkey", pubkey, 65);
	if (ret)
	{
		err.Format("%08X", ret);
		MessageBox("�û���Կд���ļ�ʧ�ܣ������룺" + err);
		return;
	}

	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText("��Կ������SM2User.pubkey");

	MessageBox("�û���Կд���ļ��ɹ�\n��Կ�ļ���SM2User.pubkey");

	//MSC_DisConnectReader();
}

void CMini_SM2_CADlg::OnBnClickedButtonGenuserp10()
{
	int ret;
	CString err;
	CVM_CSR_WRITE_OPT opt;
	BYTE pubkey[65];
	UINT plen;

	char subject_name[200];
	int slen;
	unsigned char creq[500];
	int len;
	char obuf[1000] = { 0 };;
	int olen;

	slen = ((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowTextLength();
	if ((slen > sizeof(subject_name)) || (slen <= 0))
	{
		MessageBox("ʹ�����������벻��ȷ!");
		return;
	}
	memset(subject_name, 0, sizeof(subject_name));
	((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowText(subject_name, sizeof(subject_name));

	if (!usepin)
	{
		if (ret = MSC_ConnectReader())
		{
			MessageBox("�����USBKey!");
			return;
		}

		CPassWordDlg KeypwDlg;
		if (KeypwDlg.DoModal() != IDOK)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText("");
			MessageBox("�û�������֤ʧ�ܣ�");
			usepin = 0;
			return;
		}
		usepin = 1;
	}


	//ret = MSC_SM2GenKey();
	//if (ret != 0x9000)
	//{
	//	err.Format("%08X", ret);
	//	MessageBox("�û���Կ�Բ���ʧ�ܣ������룺"+err);
	//	return;
	//}
	//pubkey[0] = 0x04;
	//ret = MSC_SM2ExportPubKey(pubkey+1, &plen);
	//if(ret != 0x9000)
	//{
	//	err.Format("%08X", ret);
	//	MessageBox("�û���Կ����ʧ�ܣ������룺"+err);
	//	return;
	//}

	//ret = cvm_pk_write_pubkey_file("SM2User.pubkey", pubkey, 65);
	//if (ret)
	//{
	//	err.Format("%08X", ret);
	//	MessageBox("�û���Կд���ļ�ʧ�ܣ������룺" + err);
	//	return;
	//}

	opt.ext_sign = 1;
	opt.subject_key = "SM2User.pubkey";
	opt.subject_name = subject_name;
	opt.subject_pwd = NULL;
	opt.key_usage = 0;
	opt.ns_cert_type = 0;
	opt.output_file = "SM2User.req";

	//���к�����Ҫ��֤pin�ɹ�״̬
	ret = cvm_csr_write_from_opt(&opt, creq, &len);
	if (ret)
		MessageBox("�û�֤��P10����ʧ��!");
	else
		MessageBox("�û�֤��P10���ɳɹ�!\n�����ļ���SM2User.req");

	IS_Base64Encode((char*)creq, len, (char*)obuf, &olen, true);

	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(obuf);

	//MSC_DisConnectReader();
}

void CMini_SM2_CADlg::OnBnClickedButtonGenusercer()
{
	int ret;
	CVM_CRT_WRITE_OPT opt;
	CString err;

	unsigned char cert[500];
	int len;
	char obuf[1000] = { 0 };;
	int olen;

	char serial[20];
	char not_befor[20];
	char not_after[20];
	char subject_name[200];

	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->GetWindowText(serial, sizeof(serial));
	((CEdit*)GetDlgItem(IDC_EDIT_NOTBEFOR))->GetWindowText(not_befor, sizeof(not_befor));
	((CEdit*)GetDlgItem(IDC_EDIT_NOTAFTER))->GetWindowText(not_after, sizeof(not_after));
	((CEdit*)GetDlgItem(IDC_EDIT_ND))->GetWindowText(subject_name, sizeof(subject_name));


	opt.serial = serial;
	opt.not_befor = not_befor;
	opt.not_after = not_after;
	opt.issuer_key = "SM2Root.prikey";
	opt.issuer_pwd = NULL;
	opt.selfsign = 0;
	opt.issuer_crt = "SM2Root.cer";
	if (usep10)
		opt.request_file = "SM2User.req";
	else
		opt.request_file = NULL;
	opt.subject_name = subject_name;
	opt.subject_pubkey = "SM2User.pubkey";
	opt.is_ca = 1;
	opt.max_pathlen = -1;
	opt.key_usage = 0;
	opt.ns_cert_type = 0;
	opt.output_file = "SM2User.cer";

	ret = cvm_crt_write_from_opt(&opt, cert, &len);
	if (ret)
	{
		err.Format("%08X", ret);
		MessageBox("�û�֤������ʧ�ܣ������룺" + err);
		return;
	}
	else
		MessageBox("�û�֤�����ɳɹ�!\n֤���ļ���""SM2Root.cer");

	IS_Base64Encode((char*)cert, len, (char*)obuf, &olen, true);

	((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText(obuf);
}

void CMini_SM2_CADlg::OnBnClickedButtonImportcert()
{
	int ret;
	char ibuf[3000];
	int ilen;
	unsigned char cert[3000];
	int clen;

	if (ret = MSC_ConnectReader())
	{
		MessageBox("�����USBKey!");
		return;
	}

	CPassWordDlg KeypwDlg;
	if (KeypwDlg.DoModal() != IDOK)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_CER))->SetWindowText("");
		MessageBox("�û�������֤ʧ�ܣ�");
		return;
	}

	ilen = ((CEdit*)GetDlgItem(IDC_EDIT_CER))->GetWindowTextLength();
	((CEdit*)GetDlgItem(IDC_EDIT_CER))->GetWindowText(ibuf, sizeof(ibuf));

	IS_Base64Decode(ibuf, ilen, (char*)cert, &clen);

	ret = MSC_WriteCert(cert, clen);
	if (ret != 0x9000)
		MessageBox("д��֤��ʧ��!");
	else
		MessageBox("д��֤��ɹ�!");

	MSC_DisConnectReader();
}

bool CMini_SM2_CADlg::WriteRootKeyPair(BYTE *bPriKey, int nPriKeyLen, BYTE *bPubKey, int nPubKeyLen)
{
	BYTE bBuff[96];
	int i = 0;
	FILE *stream;
	errno_t err;

	if( (err  = fopen_s( &stream, "sm2root.key", "wb" )) !=0 )
	{
		return false;
	}

	for ( i = 0; i < 32; i++ )
	{
		bBuff[i] = bPriKey[i];
	}

	for ( i = 0; i < 64; i++ )
	{
		bBuff[i+32] = bPubKey[i];
	}

	fwrite(bBuff,1,96,stream);

	if( stream)
	{
		if ( fclose( stream ) )
		{
			return false;
		}
	}

	return true;
}

bool CMini_SM2_CADlg::ReadRootKeyPair(BYTE *bPriKey, int nPriKeyLen, BYTE *bPubKey, int nPubKeyLen)
{

	BYTE bBuff[96];
	int i = 0;
	FILE *stream;
	errno_t err;

	if( (err  = fopen_s( &stream, "sm2root.key", "rb" )) !=0 )
	{
		return false;
	}

	fread(bBuff,1,96,stream);
	if( stream)
	{
		if ( fclose( stream ) )
		{
			return false;
		}
	}

	for (i = 0; i < 32; i++)
	{
		bPriKey[i] = bBuff[i];
	}

	for ( i = 0; i < 64; i++ )
	{
		bPubKey[i] = bBuff[i+32];
	}

	return true;
}

bool CMini_SM2_CADlg::WriteRootCert(BYTE *bRootCert, int nRootCertLen)
{
	int i = 0;
	FILE *stream;
	errno_t err;

	if( (err  = fopen_s( &stream, "sm2root.cer", "wb" )) !=0 )
	{
		return false;
	}

	fwrite(bRootCert,1,nRootCertLen,stream);

	if( stream)
	{
		if ( fclose( stream ) )
		{
			return false;
		}
	}

	return true;
}

bool CMini_SM2_CADlg::ReadRootCert(BYTE *bRootCert, int *nRootCertLen)
{
	BYTE bBuff[3000];
	int i = 0;
	FILE *stream;
	errno_t err;
	int nRealLen = 0;

	if( (err  = fopen_s( &stream, "sm2root.cer", "rb" )) !=0 )
	{
		return false;
	}

	nRealLen = (int)fread(bBuff,1,3000,stream);

	memcpy(bRootCert, bBuff, nRealLen);
	*nRootCertLen = nRealLen;

	if( stream)
	{
		if ( fclose( stream ) )
		{
			return false;
		}
	}

	return true;
}

void CMini_SM2_CADlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == true)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_ND))->SetWindowText("CN=ROOT,O=TEST,C=CN");
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTKEY))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTP10))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTCER))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERP10))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERCER))->EnableWindow(false);
		//((CButton*)GetDlgItem(IDC_CHECK1))->SetWindowText("�û�֤��ģʽ");
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_ND))->SetWindowText("CN=USER,O=TEST,C=CN");
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTKEY))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTP10))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_GENROOTCER))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERP10))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERCER))->EnableWindow(true);
		//((CButton*)GetDlgItem(IDC_CHECK1))->SetWindowText("��֤��ģʽ");
	}
}


void CMini_SM2_CADlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == true)
	{
		usep10 = 1;
	}
	else
	{
		usep10 = 0;
	}
}





void CMini_SM2_CADlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() == true)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERKEY))->SetWindowText("�����û���Կ");
		usegen = 0;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_BUTTON_GENUSERKEY))->SetWindowText("�����û���Կ��");
		usegen = 1;
	}
}
