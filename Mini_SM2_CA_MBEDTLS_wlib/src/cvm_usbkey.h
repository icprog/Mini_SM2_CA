/** ****************************************************************************
* @copyright						CVM
*               Copyright (c) 2017 - 2018 All Rights Reserved
********************************************************************************
* @file     cvm_usbkey.h
* @author   cong.peng <cong.peng@cvchip.com>
* @date     2017-5-15 17:09:24
* @version  v1.0
* @brief    CVM UsbKey SDK Header File
* @defgroup
* @{
*******************************************************************************/
#ifndef	_CVM_USBKEY_H_
#define	_CVM_USBKEY_H_

// ===========================Para Code Define==================================
#define CVM_ALG_SM1		0x11	//SM1�㷨��ʶ
#define	CVM_ALG_SM4		0x14	//SM4�㷨��ʶ

#define CVM_KEY_SM1		0x01	//SM1��Կ��ʶ
#define	CVM_KEY_SM4		0x00	//SM4��Կ��ʶ

#define CVM_SBC_ECB_E	0x00	//ECB����ģʽ
#define CVM_SBC_ECB_D	0x01	//ECB����ģʽ
#define CVM_SBC_CBC_E	0x02	//CBC����ģʽ
#define CVM_SBC_CBC_D	0x03	//CBC����ģʽ
// =============================================================================

// ===========================Error Code Define=================================
#define	CVM_OK							0			//����
#define	CVM_FAIL						0xFFFFFFFF	//ʧ��
#define	CVM_ERR							0xFFFF0000	//ʧ��
#define CVM_ALLOC_FAILED				0xFFFF1000	//���ٿռ�ʧ��
#define	CVM_ERR_CERT_PARSE_FAILED		0xFFFF2100	//֤�����ʧ��
#define	CVM_ERR_CERT_VERIFY_FAILED		0xFFFF2200	//֤�����ʧ��
#define CVM_ERR_BAD_INPUT_DATA			0xFFFF2500	//�����������
#define	CVM_ERR_BUFFER_TOO_SMALL		0xFFFF2400	//������̫С
#define	CVM_ERR_ECP_INVALID_KEY			0xFFFF3100	//��Կ��Ч
#define CVM_ERR_VERIFY_ADMINPIN			0xFFFF4100	//����ԱPIN��֤ʧ��
#define CVM_ERR_VERIFY_USERPIN			0xFFFF4200	//�û�PIN��֤ʧ��
#define CVM_ERR_PIN_STATUS				0xFFFF4300	//PIN��֤״̬��Ч
#define CVM_ERR_READ_PUBKEY				0xFFFF5101	//��ȡ��Կʧ��
#define CVM_ERR_READ_CERT				0xFFFF5102	//��ȡ֤��ʧ��
#define CVM_ERR_WRITE_CERT				0xFFFF5202	//д��֤��ʧ��

#define CVM_ERR_SIGN					0xFFFF6F0C	//˽Կǩ��ʧ��
#define CVM_ERR_VERIFY					0xFFFF6F0D	//��Կ��ǩʧ��
#define CVM_ERR_ENCRYPT					0xFFFF6F0E	//��Կ����ʧ��
#define CVM_ERR_DECRYPT					0xFFFF6F0F	//˽Կ����ʧ��

#define CVM_ERR_UNSUPPORT				0xFFFF6A81	//��֧�ִ˹���
#define CVM_ERR_P1_P2					0xFFFF6A86	//����P1��P2����ȷ
#define CVM_ERR_CANNOT_USE				0xFFFF6985	//������ʹ������
#define CVM_ERR_APPLOCKED_FOREVER		0xFFFF9303	//Ӧ�ñ���������
// =============================================================================

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// =============================================================================
// ���ƣ�CVM_OpenDevice
// ���ܣ����豸
// ��������
// ���أ�CVM_OK		����
//		 CVM_FAIL	ʧ��
// =============================================================================
int CVM_OpenDevice();

// =============================================================================
// ���ƣ�CVM_CloseDevice
// ���ܣ��ر��豸
// ������
// ���أ�CVM_OK		����
//       CVM_FAIL	ʧ��
// =============================================================================
int CVM_CloseDevice();

// =============================================================================
// ���ƣ�CVM_GetChallenge
// ���ܣ���ȡ�����
// ������rand		[OUT]	�����������
//		 rand_len	[IN]	���������
// ���أ�CVM_OK		����
//		 CVM_ERR_XXXX	UKEY���ش���ָ��
// =============================================================================
int CVM_GetChallenge(unsigned char *rand, int rand_len);

// =============================================================================
// ���ƣ�CVM_VerifyAdminPin
// ���ܣ���֤����ԱPIN
// ������pin		[IN]	PIN����
//		 pin_len	[IN]	PIN���ݳ���
// ���أ�CVM_OK		����
//		 CVM_ERR_VERIFY_ADMINPIN	�û�������֤ʧ��
// =============================================================================
int CVM_VerifyAdminPin(unsigned char *pin, int pin_len);

// =============================================================================
// ���ƣ�CVM_VerifyUserPin
// ���ܣ���֤�û�PIN
// ������pin		[IN]	PIN����
//		 pin_len	[IN]	PIN���ݻ���������
// ���أ�CVM_OK		����
//		 CVM_ERR_VERIFY_USERPIN	�û�������֤ʧ��
// =============================================================================
int CVM_VerifyUserPin(unsigned char *pin, int pin_len);

// =============================================================================
// ���ƣ�CVM_ChangeAdminPIN
// ���ܣ���������ԱPIN
// ������oldpin		[IN]	��PIN����
//		 opin_len	[IN]	��PIN���ݳ���
//		 newpin		[IN]	��PIN����
//		 npin_len	[IN]	��PIN���ݳ���
// ���أ�CVM_OK				����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤����ԱPIN
// =============================================================================
int CVM_ChangeAdminPIN(unsigned char *oldpin, int opin_len, unsigned char *newpin, int npin_len);

// =============================================================================
// ���ƣ�CVM_ChangeUserPIN
// ���ܣ������û�PIN
// ������oldpin		[IN]	��PIN����
//		 opin_len	[IN]	��PIN���ݳ���
//		 newpin		[IN]	��PIN����
//		 npin_len	[IN]	��PIN���ݳ���
// ���أ�CVM_OK				����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤����ԱPIN
// =============================================================================
int CVM_ChangeUserPIN(unsigned char *oldpin, int opin_len, unsigned char *newpin, int npin_len);

// =============================================================================
// ���ƣ�CVM_ResetUserPIN
// ���ܣ������û�PIN
// ������pin		[IN]	PIN����
//		 pin_len	[IN]	PIN���ݻ���������
// ���أ�CVM_OK		����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤����ԱPIN
// =============================================================================
int CVM_ResetUserPIN(unsigned char *pin, int pin_len);

// =============================================================================
// ���ƣ�CVM_ReadCert
// ���ܣ����豸�ж�ȡ�û�֤�飬���������ݳ���
// ������cert		[OUT]	���֤�����ݵĻ���������ΪNULL������ȡ��֤�����ݳ���
//		 cert_len	[IN,OUT]	����ʱ��ʾ���ݻ��������ȣ����ʱ��ʾ֤�����ݳ���
// ���أ�CVM_OK		����
//		 CVM_ERR_READ_CERT	֤���ȡʧ��
//		 CVM_ERR_BUFFER_TOO_SMALL	���ݻ��������Ȳ���
// =============================================================================
int CVM_ReadCert(unsigned char *cert, int *cert_len);

// =============================================================================
// ���ƣ�CVM_WriteCert
// ���ܣ�д���û�֤�鵽�豸
// ������cert		[IN]	֤������
//		 cert_len	[IN]	֤�����ݳ���
// ���أ�CVM_OK		����
//		 CVM_ERR_WRITE_CERT	֤��д��ʧ��
// ��ע��������֤�û�PIN
// =============================================================================
int CVM_WriteCert(unsigned char *cert, int cert_len);

// =============================================================================
// ���ƣ�CVM_ParseCert_Name
// ���ܣ���֤���ж�ȡ����
// ������cert		[IN]	֤������
//		 cert_len	[IN]	֤�����ݻ���������
//		 type		[IN]	��ȡ���ͣ�0��ʾ��ȡsubject��1��ʾ��ȡissuer
//		 attr		[IN]	���ȡ���Ƶ�����������"CN"��"O"��"C"��
//		 name		[OUT]	����ȡ������
//		 name_len	[IN]	���ƻ���������
// ���أ�CVM_OK		����
//		 CVM_ERR_CERT_PARSE_FAILED	֤�����ʧ��
//		 CVM_ERR_BUFFER_TOO_SMALL	���ݻ��������Ȳ���
// =============================================================================
int CVM_ParseCert_Name(unsigned char *cert, int cert_len, int type, 
	const char *attr,	char *name, int name_len);

// =============================================================================
// ���ƣ�CVM_ParseCert_PubKey
// ���ܣ���֤���ж�ȡ��Կ
// ������cert		[IN]	֤������
//		 cert_len	[IN]	֤�����ݻ���������
//		 pubkey		[OUT]	��Կ���ݻ�����
//		 pk_len		[IN,OUT]	��Կ����������
// ���أ�CVM_OK		����
//		 CVM_ERR_CERT_PARSE_FAILED	֤�����ʧ��
//		 CVM_ERR_BUFFER_TOO_SMALL	���ݻ��������Ȳ���
// =============================================================================
int CVM_ParseCert_PubKey(unsigned char *cert, int cert_len, unsigned char *pubkey, int *pk_len);

// =============================================================================
// ���ƣ�CVM_ParseCert_Validity
// ���ܣ���֤���ж�ȡ��Чʱ��
// ������cert		[IN]	֤������
//		 cert_len	[IN]	֤�����ݻ���������
//		 not_befor	[OUT]	��Ч����ʼʱ�䣬��ʽyyyy-mm-dd hh:mm:ss
//		 not_after	[OUT]	��Ч�ڽ�ֹʱ�䣬��ʽyyyy-mm-dd hh:mm:ss
// ���أ�CVM_OK		����
//		 CVM_ERR_CERT_PARSE_FAILED	֤�����ʧ��
// ��ע��������������ȴ���20����
// =============================================================================
int CVM_ParseCert_Validity(unsigned char *cert, int cert_len, char *not_befor, char *not_after);

// =============================================================================
// ���ƣ�CVM_VerifyCert
// ���ܣ����ϼ�֤����֤֤����Ч��
// ������cert		[IN]	֤������
//		 cert_len	[IN]	֤�����ݻ���������
//		 trust_ca	[IN]	�ϼ�֤������
//		 tca_len	[IN]	�ϼ�֤�����ݻ���������
// ���أ�CVM_OK		����
//		 CVM_ERR_CERT_PARSE_FAILED	֤�����ʧ��
//		 CVM_ERR_CERT_VERIFY_FAILED	֤����֤ʧ��
// =============================================================================
int CVM_VerifyCert(unsigned char *cert, int cert_len, unsigned char *trust_ca, int tca_len);

// =============================================================================
// ���ƣ�CVM_SM2GenKey
// ���ܣ������豸�ڵĹ�˽Կ��
// ��������
// ���أ�CVM_OK				����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤�û�PIN
// =============================================================================
int CVM_SM2GenKey();

// =============================================================================
// ���ƣ�CVM_SM2ImportKeyPairToFile
// ���ܣ����빫˽Կ�Ե��豸
// ������pubkey		[IN]	��Կ����
//		 pubk_len	[IN]	��Կ���ݳ���
//		 prikey		[IN]	˽Կ����
//		 prik_len	[IN]	˽Կ���ݳ���
// ���أ�CVM_OK				����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤�û�PIN
// =============================================================================
int CVM_SM2ImportKeyPairToFile(unsigned char *pubkey, int pubk_len, unsigned char *prikey, int prik_len);

// =============================================================================
// ���ƣ�CVM_SM2ExportPubKey
// ���ܣ������豸�ڵĹ�Կ
// ������pubk		[OUT]	��Կ����
//		 plen		[OUT]	��Կ���ݻ���������
// ���أ�CVM_OK					����
//		 CVM_ERR_READ_PUBKEY	��Կ��ȡʧ��
// =============================================================================
int CVM_SM2ExportPubKey(unsigned char *pubk, int *plen);

// =============================================================================
// ���ƣ�CVM_SM2SignHash
// ���ܣ�ʹ��SM2�㷨���Ӵ�ֵ����ǩ��
// ������hash		[IN]	��ǩ���Ӵ�ֵ
//		 hash_len	[IN]	��ǩ���Ӵ�ֵ����
//		 sign		[OUT]	ǩ��ֵ������
//		 sign_len	[OUT]	ǩ��ֵ����������ָ��
// ���أ�CVM_OK				����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע��������֤�û�PIN
// =============================================================================
int CVM_SM2SignHash(unsigned char *hash, int hash_len, unsigned char *sign, int *sign_len);

// =============================================================================
// ���ƣ�CVM_SM2VerifyHash
// ���ܣ�ʹ��SM2�㷨���Ӵ�ֵ��ǩ�����ݽ�����֤
// ������hash		[IN]	��ǩ���Ӵ�ֵ
//		 hash_len	[IN]	��ǩ���Ӵ�ֵ����
//		 pubkey		[IN]	��Կ
//		 pubk_len	[IN]	��Կ���ݳ���
//		 sign		[IN]	ǩ��ֵ
//		 sign_len	[IN]	ǩ��ֵ����
// ���أ�CVM_OK				����
//		 CVM_ERR_PIN_STATUS	PIN״̬����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM2VerifyHash(unsigned char *hash, int hash_len, unsigned char *pubkey, int pubk_len, unsigned char *sign, unsigned int sign_len);

// =============================================================================
// ���ƣ�CVM_SM2SignData
// ���ܣ�ʹ��SM2�㷨�����ݽ���ǩ��
// ������data		[IN]	��ǩ������
//		 data_len	[IN]	��ǩ�����ݳ���
//		 sign		[OUT]	ǩ��ֵ������
//		 sign_len	[OUT]	ǩ��ֵ����������ָ��
// ���أ�CVM_OK		����
//		 CVM_ERR_SIGN			ǩ��ʧ��
//		 CVM_ERR_READ_PUBKEY	��Կ��ȡʧ��
//		 CVM_ERR_ECP_INVALID_KEY	��Ч��Կ����
// ��ע��������֤�û�PIN
// =============================================================================
int CVM_SM2SignData(unsigned char *data, int data_len, unsigned char *sign, unsigned int *sign_len);

// =============================================================================
// ���ƣ�CVM_SM2VerifyData
// ���ܣ�ʹ��SM2�㷨��ǩ�����ݽ�����֤
// ������data		[IN]	����ǩ����
//		 data_len	[IN]	����ǩ���ݳ���
//		 pubk		[IN]	��Կ
//		 klen		[IN]	��Կ���ݳ���
//		 sign		[IN]	ǩ��ֵ
//		 sign_len	[IN]	ǩ��ֵ����
// ���أ�CVM_OK		����
//		 CVM_ERR_VERIFY			��ǩʧ��
//		 CVM_ERR_ECP_INVALID_KEY	��Ч��Կ����
// =============================================================================
int CVM_SM2VerifyData(unsigned char *data, int dlen, unsigned char *pubk, int klen, unsigned char *sign, unsigned int slen);

// =============================================================================
// ���ƣ�CVM_SM2Encrypt
// ���ܣ�ʹ��SM2�㷨���ⲿ��Կ�����ݽ��м���
// ������plain		[IN]	��������
//		 plen		[IN]	�������ݳ���
//		 pubkey		[IN]	��Կ
//		 klen		[IN]	��Կ���ݳ���
//		 cipher		[IN]	��������
//		 clen		[IN,OUT]	�������ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM2Encrypt(unsigned char *plain, int plen, unsigned char *pubkey, int klen, unsigned char *cipher, int *clen);

// =============================================================================
// ���ƣ�CVM_SM2Decrypt
// ���ܣ�ʹ��SM2�㷨���ڲ�˽Կ�����ݽ��н���
// ������cipher		[IN]	��������
//		 clen		[IN]	�������ݳ���
//		 plain		[IN]	��������
//		 plen		[IN,OUT]	�������ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM2Decrypt(unsigned char *cipher, int clen, unsigned char *plain, int *plen);

// =============================================================================
// ���ƣ�CVM_SM3Hash
// ���ܣ�ʹ��SM3�㷨�����ݽ���ժҪ
// ������message	[IN]		����
//		 mlen		[IN]		���ݳ���
//		 hash		[IN]		�Ӵ�ֵ����������ΪNULL����ʾ��ȡ�Ӵ�ֵ����
//		 hlen		[IN,OUT]	�Ӵ�ֵ����������
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM3Hash(unsigned char *message, int mlen, unsigned char *hash, int *hlen);

// =============================================================================
// ���ƣ�CVM_SM3Init
// ���ܣ�SM3�㷨��ʼ��
// ������message	[IN]		����
//		 mlen		[IN]		���ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM3Init(unsigned char *message, int mlen);

// =============================================================================
// ���ƣ�CVM_SM3Update
// ���ܣ��������ݽ���SM3�㷨�Ӵռ���
// ������message	[IN]		����
//		 mlen		[IN]		���ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// ��ע�����ȵ���CVM_SM3Init
// =============================================================================
int CVM_SM3Update(unsigned char *message, int mlen);

// =============================================================================
// ���ƣ�CVM_SM3Final
// ���ܣ�����SM3�㷨�Ӵռ���
// ������message	[IN]		����
//		 mlen		[IN]		���ݳ���
//		 hash		[IN]		�Ӵ�ֵ����������ΪNULL����ʾ��ȡ�Ӵ�ֵ����
//		 hlen		[IN,OUT]	�Ӵ�ֵ����������
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX		UKEY���ش���ָ��
// =============================================================================
int CVM_SM3Final(unsigned char *message, int mlen, unsigned char *hash, int *hlen);

// =============================================================================
// ���ƣ�CVM_SBCWriteKeyToFile
// ���ܣ��Գ��㷨��Կ����
// ������KeyIndex	[IN]		��Կ��ʶ
//		 key		[IN]		��Կ����
//		 klen		[IN]		��Կ���ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX			UKEY���ش���ָ��
// =============================================================================
int CVM_SBCWriteKeyToFile(unsigned char KeyIndex, unsigned char *key, int klen);

// =============================================================================
// ���ƣ�CVM_SBCInit
// ���ܣ��Գ��㷨��ʼ��������㷨����Կ����
// ������AlgFlag	[IN]		�㷨��ʶ
//		 KeyIndex	[IN]		��Կ��ʶ��keyΪNULLʱ,��Ч��
//		 key		[IN]		��Կ���ݣ���ΪNULL����ʹ����Կ��ʶ��
//		 klen		[IN]		��Կ���ݳ���
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX			UKEY���ش���ָ��
// =============================================================================
int CVM_SBCInit(unsigned char AlgFlag, unsigned char KeyIndex, unsigned char *key, int klen);

// =============================================================================
// ���ƣ�CVM_SBCDone
// ���ܣ��Գ��㷨����
// ������type		[IN]		�Գ��㷨����ģʽ
//		 iv			[IN]		��ʼ������
//		 indata		[IN]		��������
//		 ilen		[IN]		�������ݳ���
//		 outdata	[OUT]		�������
//		 olen		[OUT]		������ݳ���ָ��
// ���أ�CVM_OK					����
//		 CVM_ERR_XXXX			UKEY���ش���ָ��
// ��ע����ivΪNULL����ʾ��������IV���м���
// =============================================================================
int CVM_SBCDone(unsigned char type, unsigned char *iv, unsigned char *indata, int ilen, unsigned char *outdata, int *olen);


int CVM_ParseCert_Signature(unsigned char *cert, int cert_len, unsigned char *sign, int *sign_len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CVM_USBKEY_H_ */