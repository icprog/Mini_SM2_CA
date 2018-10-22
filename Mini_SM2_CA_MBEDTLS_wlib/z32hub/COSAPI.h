#ifndef __COSAPI_H__
#define __COSAPI_H__

#include <iostream>   
#include <windows.h>
#include <winioctl.h> 
#include <setupapi.h>
#include "xMutex.h"

#define MAX_READERS			10

//APDU �����շ��ṹ
typedef struct
{
		BYTE		cla;
		BYTE		ins;
		BYTE		p1;
		BYTE		p2;
		BYTE		lc;
		BYTE		sendbuf[288];
		BYTE		le;
		BYTE		recvbuf[288];
} APDUEx, *pAPDUEx;

// ������� ===========================================================

EXTERN_C int WINAPI API_Find();
EXTERN_C int WINAPI API_Open(int index);
EXTERN_C int WINAPI API_SendApdu(int index,pAPDUEx apdu);
EXTERN_C int WINAPI API_Close(int index);
EXTERN_C int WINAPI API_EnumDev(LPSTR szNameList, int *ReaderNum,ULONG *pulSize);
EXTERN_C int WINAPI API_ConnectDev(LPSTR szName);
EXTERN_C int WINAPI API_ConnectOneDev();


#define SCARD_F_NO_CARD                       0x80100003L
#define SCARD_F_BUFFER_TOO_SMALL              0x80100001L 

// ������� ===========================================================
#ifndef SCARD_S_SUCCESS

// �ɹ���û�д���
#define SCARD_S_SUCCESS 				((DWORD)0x00000000L)

// �ڲ����Ӽ��ʧ��
#define SCARD_F_INTERNAL_ERROR			((DWORD)0x80100001L)

// �������û���ֹ
#define SCARD_E_CANCELLED				((DWORD)0x80100002L)

// ����ȷ�Ĳ������
#define SCARD_E_INVALID_HANDLE			((DWORD)0x80100003L)

// ����ȷ�Ĳ���(p1, p2)
#define SCARD_E_INVALID_PARAMETER		((DWORD)0x80100004L)

// ע���������Ϣ��ʧ����Ч
#define SCARD_E_INVALID_TARGET			((DWORD)0x80100005L)

// û���㹻���ڴ������������
#define SCARD_E_NO_MEMORY				((DWORD)0x80100006L)

// �ڲ���ʱ
#define SCARD_F_WAITED_TOO_LONG			((DWORD)0x80100007L)

// �û������Ļ�����̫С�������Է���ȫ���ķ�������
#define SCARD_E_INSUFFICIENT_BUFFER		((DWORD)0x80100008L)

// δ֪�Ķ�����
#define SCARD_E_UNKNOWN_READER			((DWORD)0x80100009L)

// �û�ָ����ʱ�䳬ʱ
#define SCARD_E_TIMEOUT					((DWORD)0x8010000AL)

// �����ڱ���������ռ��
#define SCARD_E_SHARING_VIOLATION		((DWORD)0x8010000BL)

// �ڶ���������û�п�
#define SCARD_E_NO_SMARTCARD			((DWORD)0x8010000CL)

// δ֪�Ŀ�����
#define SCARD_E_UNKNOWN_CARD			((DWORD)0x8010000DL)

// �������޷�����˳�������
#define SCARD_E_CANT_DISPOSE			((DWORD)0x8010000EL)

// ��ǰ�Ŀ���֧���û�ָ����ͨѶЭ��
#define SCARD_E_PROTO_MISMATCH			((DWORD)0x8010000FL)

// ����û��׼���ý�������
#define SCARD_E_NOT_READY				((DWORD)0x80100010L)

// ĳЩ������ֵ������
#define SCARD_E_INVALID_VALUE			((DWORD)0x80100011L)

// ������ϵͳ��ֹ�����������µ�½��ػ�
#define SCARD_E_SYSTEM_CANCELLED		((DWORD)0x80100012L)

// �ڲ�ͨѶ����
#define SCARD_F_COMM_ERROR				((DWORD)0x80100013L)

// �ڲ�δ֪����
#define SCARD_F_UNKNOWN_ERROR			((DWORD)0x80100014L)

// ��Ч�� ATR ��
#define SCARD_E_INVALID_ATR				((DWORD)0x80100015L)

// �û����Խ���ĳ�������ڵĴ���
#define SCARD_E_NOT_TRANSACTED			((DWORD)0x80100016L)

// ָ���Ķ�������ǰ�޷�ʹ��
#define SCARD_E_READER_UNAVAILABLE		((DWORD)0x80100017L)

// ��������ֹ�������������˳�
#define SCARD_P_SHUTDOWN				((DWORD)0x80100018L)

// PCI �Ľ��ջ�����̫С
#define SCARD_E_PCI_TOO_SMALL			((DWORD)0x80100019L)

// �������������޷�֧�ֵ�ǰ�Ķ�����
#define SCARD_E_READER_UNSUPPORTED		((DWORD)0x8010001AL)

// �����������������޷�����Ψһ�����֣��Ѿ�����ͬ���ֵĶ���������
#define SCARD_E_DUPLICATE_READER		((DWORD)0x8010001BL)

// ���޷�����ǰ�Ķ�����֧��
#define SCARD_E_CARD_UNSUPPORTED		((DWORD)0x8010001CL)

// ���ܿ�����û�п���
#define SCARD_E_NO_SERVICE				((DWORD)0x8010001DL)

// ���ܿ������Ѿ�����ֹ
#define SCARD_E_SERVICE_STOPPED			((DWORD)0x8010001EL)

// ĳ����������ܿ��������
#define SCARD_E_UNEXPECTED				((DWORD)0x8010001FL)

// �޷���֪���ܿ����ṩ����Ϣ
#define SCARD_E_ICC_INSTALLATION		((DWORD)0x80100020L)

// �޷���֪���ܿ�����������Ϣ
#define SCARD_E_ICC_CREATEORDER			((DWORD)0x80100021L)

// ��ǰ�����ܿ��޷�֧���û�Ҫ��Ĺ���
#define SCARD_E_UNSUPPORTED_FEATURE		((DWORD)0x80100022L)

// ָ����Ŀ¼������
#define SCARD_E_DIR_NOT_FOUND			((DWORD)0x80100023L)

// ָ�����ļ�������
#define SCARD_E_FILE_NOT_FOUND			((DWORD)0x80100024L)

// ָ����Ŀ¼��������Ч��Ŀ¼
#define SCARD_E_NO_DIR					((DWORD)0x80100025L)

// ָ�����ļ���������Ч���ļ���û��ѡ���ļ�
#define SCARD_E_NO_FILE					((DWORD)0x80100026L)

// ���ļ��ܾ�����
#define SCARD_E_NO_ACCESS				((DWORD)0x80100027L)

// ���Ŀռ��������޷���д����Ϣ
#define SCARD_E_WRITE_TOO_MANY			((DWORD)0x80100028L)

// �����ļ�ָ�����
#define SCARD_E_BAD_SEEK				((DWORD)0x80100029L)

// PIN �����
#define SCARD_E_INVALID_CHV				((DWORD)0x8010002AL)

// һ���޷�ʶ��Ĵ���������ܿ����񷵻�
#define SCARD_E_UNKNOWN_RES_MNG			((DWORD)0x8010002BL)

// �����֤�鲻����
#define SCARD_E_NO_SUCH_CERTIFICATE		((DWORD)0x8010002CL)

// �����֤�鲻������
#define SCARD_E_CERTIFICATE_UNAVAILABLE	((DWORD)0x8010002DL)

// �Ҳ����κ�һ��������
#define SCARD_E_NO_READERS_AVAILABLE	((DWORD)0x8010002EL)

// ���ܿ�ͨѶ�����з������ݶ�ʧ�����ٴγ���
#define SCARD_E_COMM_DATA_LOST			((DWORD)0x8010002FL)

// �������Կ�ļ�������
#define SCARD_E_NO_KEY_CONTAINER		((DWORD)0x80100030L)

// ���� ATR ���ó�ͻ���������޷�����ͨѶ
#define SCARD_W_UNSUPPORTED_CARD		((DWORD)0x80100065L)

// ���Ը�λû����Ӧ
#define SCARD_W_UNRESPONSIVE_CARD		((DWORD)0x80100066L)

// ��û�е�
#define SCARD_W_UNPOWERED_CARD			((DWORD)0x80100067L)

// ������λ�ˣ���˹������Ϣ��Ч��
#define SCARD_W_RESET_CARD				((DWORD)0x80100068L)

// ���Ѿ����Ƴ���
#define SCARD_W_REMOVED_CARD			((DWORD)0x80100069L)

// ��Ϊ��ȫ���򣬷��ʱ��ܾ���
#define SCARD_W_SECURITY_VIOLATION		((DWORD)0x8010006AL)

// PIN ��û�б���֤�����ʱ��ܾ�
#define SCARD_W_WRONG_CHV				((DWORD)0x8010006BL)

// �Ѿ�������� PIN ����֤���������ʱ��ܾ�
#define SCARD_W_CHV_BLOCKED				((DWORD)0x8010006CL)

// �Ѿ������������ܿ��ļ���û�и�����ļ����Է�����
#define SCARD_W_EOF						((DWORD)0x8010006DL)

// �������û���ֹ
#define SCARD_W_CANCELLED_BY_USER		((DWORD)0x8010006EL)

// ���ܿ� PIN û������
#define SCARD_W_CARD_NOT_AUTHENTICATED	((DWORD)0x8010006FL)

#endif // SCARD_S_SUCCESS


// �ļ��Ѿ�����
#define SCARD_E_FILE_EXISTS				((DWORD)0xA0100001L)

// ���ڴ洢����������
#define SCARD_E_EPROM_ERROR				((DWORD)0xA0100002L)

// �û���������Ч�� CLA
#define SCARD_E_INVALID_CLA				((DWORD)0xA0100003L)

// �û���������Ч�� INS
#define SCARD_E_INVALID_INS				((DWORD)0xA0100004L)

// VM ��ַ����/�쳣
#define SCARD_E_VM_ADDRESS_ERROR		((DWORD)0xA0100005L)

// �� 0 ��
#define SCARD_E_ZERO_DIVIDE				((DWORD)0xA0100006L)

// ��û�б����뵽��ȷ��λ��
#define SCARD_E_WRONG_POSITION			((DWORD)0xA0100007L)

// ����ǰ����ĳ��δ֪��״̬
#define SCARD_E_UNKNOWN_STATE			((DWORD)0xA0100008L)

// ����û�б���
#define SCARD_E_CARD_NOT_OPENED			((DWORD)0xA0100009L)

// δ֪������
#define SCARD_E_UNKNOWN_COMMAND			((DWORD)0xA010000AL)

// �����趨����������������ô����� 0
#define SCARD_E_ZERO_TRYTIME			((DWORD)0xA010000BL)

// ����̫����豸
#define SCARD_E_TOO_MANY_DEVICE			((DWORD)0xA010000CL)

// �Ƿ�ָ���
#define	SCARD_E_INVALID_INSTRUCTION		((DWORD)0xA010000DL)

// ����������Ҫ����
#define SCARD_W_RESPONSE				((DWORD)0xA01000FFL)

// �ڲ�����(Windows ����)
#define SCARD_ERR_SETUP_DI_CLASS_DEVS			0xA0100010

// �ڲ�����(Windows ����)
#define SCARD_TOO_LONG_DEVICE_DETAIL			0xA0100013

// �ڲ�����(Windows ����)
#define SCARD_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL		0xA010000A

// ���豸��(Windows ����)
#define SCARD_ERR_OPEN_DEVICE								0xA0100007

// û���ҵ�����Ҫ����豸(��������)
#define SCARD_ERR_NO_SUCH_DEVICE							0xA0100001

// �豸��(��������)
#define DEVICE_IN_USE							0xA0100046

// д��¼��(Windows ����)
#define SCARD_ERR_WRITE_REPORT								0xA0100009

// �ڲ�����(Windows ����)
#define SCARD_ERR_FLUSH_QUEUE								0xA010000F

// ����¼��(Windows ����)
#define SCARD_ERR_READ_REPORT								0xA0100008

#endif