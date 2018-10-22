#include <stdio.h>
#include "COSAPI.h"

#pragma comment(lib, "setupapi.lib")

//���ݽṹ����=============================================================== 

//MSC �豸�洢
typedef struct tagMASS_STORAGE_DEVICE
{
	BOOL		bOpened;				// ���Ƿ��Ѿ���ͨ
	HANDLE		hCard;					// ���ľ��
	xMutex	    mutex;
	char		strName[256];
	char		strPath[256];
}
MASS_STORAGE_DEVICE, *PMASS_STORAGE_DEVICE;

typedef struct tagScsiPassThroughDirect   
{   
	WORD    Length;             // [x00]    
	BYTE    ScsiStatus;         // [x01]    
	BYTE    PathId;             // [x02]    
	BYTE    TargetId;           // [x03]    
	BYTE    Lun;                // [x04]    
	BYTE    CdbLength;          // [x05]    
	BYTE    SenseInfoLength;    // [x06]    
	BYTE    DataIn;             // [x07]    
	DWORD   DataTransferLength; // [x0B:0A:09:08]    
	DWORD   TimeOutValue;       // [x0F:0E:0D:0C]    
	void *  DataBuffer;         // [x13:12:11:10]    
	DWORD   SenseInfoOffset;    // [x17:16:15:14]    
	BYTE    Cdb[16];            // [x18, ... x27]    
}   
SCSI_PASS_THROUGH_DIRECT;   

typedef struct tagSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER   
{   
	SCSI_PASS_THROUGH_DIRECT sptd;   
	DWORD   byFiller; // realign buffer to double word boundary    
	BYTE    bySenseBuf[32];   
}
SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;
typedef struct
{
	BYTE sw1;
	BYTE sw2;
	DWORD error_code;
	DWORD mode;
} s_Error;

//�궨��===============================================================  
//Windows����ϵͳAPI�����USBͨѶ�ṹ�壬����־������    
#define SCSI_IOCTL_DATA_OUT                 0    
#define SCSI_IOCTL_DATA_IN                  1    
#define CDB_GENERIC_LENGTH                  10    
#define IOCTL_SCSI_PASS_THROUGH_DIRECT      0x4D014    
#define LINK_OK_TMOT						100  // x1��  
#define ERROR_NUM							29
#define TRANSFORMABLE_SW2					1	// sw2 �е������Ǳ仯��

//ȫ�ֱ�������===============================================================  
MASS_STORAGE_DEVICE g_DevList[MAX_READERS];
int g_DevNum;
s_Error error_list[ERROR_NUM] = 
{
	{0x90, 0x00, SCARD_S_SUCCESS, 0},	// �ɹ�
	// ===========================================================
	{0x61, 0xFF, SCARD_W_RESPONSE, TRANSFORMABLE_SW2},	// ����������Ҫ����
	{0x62, 0x83, SCARD_E_NO_FILE, 0},
	{0x65, 0x81, SCARD_E_EPROM_ERROR, 0},
	{0x67, 0x00, SCARD_E_INVALID_VALUE, 0},
	{0x69, 0x82, SCARD_W_SECURITY_VIOLATION, 0},
	{0x69, 0x85, SCARD_E_FILE_EXISTS, 0},
	{0x6a, 0x82, SCARD_E_FILE_NOT_FOUND, 0},
	{0x6a, 0x83, SCARD_W_EOF, 0},
	{0x6a, 0x84, SCARD_E_WRITE_TOO_MANY, 0},
	{0x6b, 0x00, SCARD_E_INVALID_PARAMETER, 0},
	{0x6d, 0x00, SCARD_E_INVALID_INS, 0},
	{0x6d, 0x81, SCARD_E_INVALID_INSTRUCTION, 0},
	{0x6d, 0x82, SCARD_E_VM_ADDRESS_ERROR, 0},
	{0x6d, 0x8f, SCARD_E_ZERO_DIVIDE, 0},
	{0x6e, 0x00, SCARD_E_INVALID_CLA, 0},
	{0x6f, 0x81, SCARD_E_UNEXPECTED, 0},

};


DWORD ErrorCodeConvert(BYTE sw1, BYTE sw2)
{
	for (int i=0;i<ERROR_NUM;i++)
	{
		if (sw1 == error_list[i].sw1)
		{
			if (error_list[i].mode & TRANSFORMABLE_SW2) 
				return(error_list[i].error_code);
			if (sw2 == error_list[i].sw2) 
				return(error_list[i].error_code);
		}
	}
	return(SCARD_F_UNKNOWN_ERROR);
}


//////////////////////////////////////////////////////////////////////////
// EXTERN_C int WINAPI API_Find();
// ���ܣ��ڵ�ǰ�����²����Ѿ����ӵģ����ҿ��������򿪵�����ָ���豸��
// �������ҵ����豸������       
// ��������
// ���أ�
//		һ������0��ֵ	��������,
//		-1	ִ��ʧ��
//////////////////////////////////////////////////////////////////////////
EXTERN_C int WINAPI API_Find()
{
	int i;
	GUID Guid =	
	{ 0x53f56308, 0xb6bf, 0x11d0,{ 0x94, 0xf2, 0x00,  0xa0, 0xc9, 0x1e, 0xfb, 0x8b } };

	BOOL bDeviceFinded = FALSE,bLastDevice = FALSE;
	HDEVINFO hDevInfoSet = NULL;
	char cDevicePath[256];
	char cHardwareId[256];
	strcpy(cDevicePath,"");
	//strcpy(cHardwareId,"USBSTOR\\CdRomCIDC____CD-ROM__________2.02");
	//strcpy(cHardwareId1,"USBSTOR\\CdRomCIDC____CD-ROM__________3.03");
	//strcpy(cHardwareId2,"USBSTOR\\CdRomCIDC____CD-ROM__________3.04");
	//strcpy(cHardwareId,"USBSTOR\\CdRomCD-ROM__INFOSMART_______1.01");
	strcpy(cHardwareId,"USBSTOR\\CdRomCVMChip_Key_____________1.01");
	
	int	retcode =0;
	for (i=0;i<g_DevNum;i++)
	{
		if(g_DevList[i].bOpened)
		{
			retcode = API_Close(i);
			if (SCARD_S_SUCCESS != retcode)
				return(retcode);
		}
	}
	memset(g_DevList,0,sizeof(g_DevList));
	g_DevNum = 0;

	hDevInfoSet = SetupDiGetClassDevs(&Guid,	//	All Device
		0,
		0,
		DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	int result = 0;
	int devIndex = 0;
	DWORD dwRequired = 0;
	PSP_DEVICE_INTERFACE_DATA pDevInterfaceData = NULL;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDevInterfaceDetailData = NULL;
	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	while (1)
	{
		if (!pDevInterfaceData)
		{
			pDevInterfaceData = new SP_DEVICE_INTERFACE_DATA;
			pDevInterfaceData->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		}
		result = SetupDiEnumDeviceInterfaces(hDevInfoSet,
			0,
			&Guid,
			devIndex,
			pDevInterfaceData);
		if ( !result )
		{
			if ( GetLastError() ==  ERROR_NO_MORE_ITEMS)//�豸�������
			{
				break;
			}
			else
			{
				devIndex++;
				continue;
			}
		}
		if (result)
		{
			result = SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
				pDevInterfaceData,
				NULL,
				0,
				&dwRequired,
				NULL);

			pDevInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwRequired);
			pDevInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			result = SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
				pDevInterfaceData,
				pDevInterfaceDetailData,
				dwRequired,
				NULL,
				NULL);

			SetupDiEnumDeviceInfo(hDevInfoSet,devIndex,&DeviceInfoData);			

			DWORD dwDataType,dwRequireSize=0;
			LPTSTR propertyBuffer = NULL;
			while (!SetupDiGetDeviceRegistryProperty(hDevInfoSet,
				&DeviceInfoData,
				SPDRP_HARDWAREID,
				&dwDataType,
				(PBYTE)propertyBuffer,
				dwRequireSize,
				&dwRequireSize))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					//	Change the buffer size.	
					if (propertyBuffer)
						LocalFree(propertyBuffer);

					propertyBuffer = (LPTSTR)LocalAlloc(LPTR,dwRequireSize);
				}
				else
				{
					//	Unknown Failure
					break;
				}            
			}	//	end while

			if (0 == strcmp(cHardwareId,propertyBuffer))
			{

				strcpy(g_DevList[g_DevNum].strPath,pDevInterfaceDetailData->DevicePath);
				strcpy(g_DevList[g_DevNum].strName,pDevInterfaceDetailData->DevicePath+47);
				g_DevNum++;

			}	//	end if

			devIndex++;
		}	//	end if
	}

	SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return g_DevNum;
}



// ====================================================================
// EXTERN_C int WINAPI API_Open(int index);
// ���ܣ�����index������ָ�����豸����������0��ʼ��
// �����豸index
// ������
// ������	����	In/out	����ѡ��
// index	�豸��ȫ���豸�б��е�������	In	�������ɡ�0����ʼ
// ���أ�
//			һ����С��0��ֵ	�ɹ�
//			-1	ʧ��
// ====================================================================
EXTERN_C int WINAPI API_Open(int index)
{
	g_DevList[index].mutex.EnterSynCode();
	g_DevList[index].hCard = CreateFile(g_DevList[index].strPath,
											GENERIC_READ | GENERIC_WRITE,
											0,
											NULL,
											OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL,
											NULL
											);
	if(g_DevList[index].hCard == INVALID_HANDLE_VALUE)
	{
// 		LPVOID lpMsgBuf =NULL;
// 		TCHAR szBuf[128];
// 		DWORD dw = GetLastError(); 
// 		FormatMessage(   
// 			FORMAT_MESSAGE_ALLOCATE_BUFFER   |   
// 			FORMAT_MESSAGE_FROM_SYSTEM   |   
// 			FORMAT_MESSAGE_IGNORE_INSERTS, 
// 			NULL, 
// 			dw, 
// 			MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),   //   Default   language 
// 			(LPTSTR)   &lpMsgBuf, 
// 			0, 
// 			NULL   
// 			); 
// 		wsprintf(szBuf,("%s ������Ϣ (�����룽%d): %s"),("SCSI_IOCTL_DATA_OUT"), dw, lpMsgBuf);
// 		printf("%s\n",szBuf);
		g_DevList[index].bOpened=0;
		g_DevList[index].mutex.LeaveSynCode();
		return -1;
	}

	g_DevList[index].bOpened=1;
	g_DevList[index].mutex.LeaveSynCode();
	return index;	

}

// ====================================================================
// EXTERN_C int WINAPI API_Close(int index);
// ����: �ر�indexָ�����豸
// 	������
// 		index	�豸��ȫ���豸�б��е�������
// 	���أ�
// 		0	��������
// 		-1	ִ��ʧ��
// ====================================================================
EXTERN_C int WINAPI API_Close(int index)
{
	
	g_DevList[index].bOpened=0;
	g_DevList[index].mutex.LeaveSynCode();
	
	if (g_DevList[index].hCard)
	{
		BOOL ret = CloseHandle(g_DevList[index].hCard);
		g_DevList[index].hCard = NULL;
		if (!ret)
		{
			return -1;
		}	
	}
	return SCARD_S_SUCCESS;
}
//---------------------------------------------------------------------------
//	EXTERN_C int WINAPI API_Release()
//	ժҪ:
//	  �ڳ������ʱʱ����Ҫ���������Թ����������ڲ�����ʹ��
//	����ֵ:
//	  ����0��ʾ��ȷ�������ļ��������
//---------------------------------------------------------------------------

EXTERN_C int WINAPI API_Release()
{
	int i;
	int retcode =0;
	for (i=0;i<MAX_READERS;i++) 
	{
		retcode = API_Close(i);
		if(retcode!=SCARD_S_SUCCESS)
			return retcode;
	}
	return SCARD_S_SUCCESS;
}

// ====================================================================
// ����APDU,�����ؽ��
// ������
//	   hDev	�豸�ľ�������룩
//     apdu:apdu���ݰ������������
// ����ֵ:ErrorCodeConvertȷ���ķ�����
// ====================================================================
int SendApdu(HANDLE hDev,pAPDUEx apdu)
{
	BYTE  bSendBuf[256];
	DWORD dwSendBufLen = 0;
	BYTE  bReceBuf[256];
	DWORD dwReceBufLen = 0;
	int   rlen = 0;
	WORD  off=0;
	SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;   
	DWORD dwSize;   
	int iRet;
	BYTE sw1, sw2;        //���ش���
	int retcode =-1;


	memset(bSendBuf,0,sizeof(bSendBuf));
	memcpy(bSendBuf, apdu, 4);
	dwSendBufLen = 4;
	if (apdu->lc != 0)
	{
		bSendBuf[dwSendBufLen] = (BYTE)apdu->lc;
		dwSendBufLen++;
		memcpy(bSendBuf+dwSendBufLen, apdu->sendbuf, apdu->lc);
		dwSendBufLen += apdu->lc;
	}
	if (apdu->le != 0)
	{
		bSendBuf[dwSendBufLen] = (BYTE)apdu->le;
		dwSendBufLen++;
	}
	else
	{
		bSendBuf[dwSendBufLen] = 0x00;
		dwSendBufLen++;
	}

	ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));
	sptdwb.sptd.Length                  = sizeof(SCSI_PASS_THROUGH_DIRECT);   
	sptdwb.sptd.PathId                  = 0;   
	sptdwb.sptd.TargetId                = 1;   
	sptdwb.sptd.Lun                     = 0;   
	sptdwb.sptd.CdbLength               = CDB_GENERIC_LENGTH;   
	sptdwb.sptd.SenseInfoLength         = 24;   
	sptdwb.sptd.SenseInfoOffset         = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, bySenseBuf);  
	sptdwb.sptd.TimeOutValue            = LINK_OK_TMOT;  
	sptdwb.sptd.Cdb[0] = 0xFF;
	sptdwb.sptd.Cdb[1] = 0x48;
	sptdwb.sptd.Cdb[2] = 0x53;
	sptdwb.sptd.Cdb[3] = 0x49;
	sptdwb.sptd.Cdb[4] = 0x43;
	sptdwb.sptd.Cdb[5] = 0x02;

	sptdwb.sptd.DataIn					= SCSI_IOCTL_DATA_OUT; 
	sptdwb.sptd.DataTransferLength      = dwSendBufLen;   
	sptdwb.sptd.DataBuffer              = bSendBuf;   

	dwSize = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);  
	iRet = DeviceIoControl(hDev, 
		IOCTL_SCSI_PASS_THROUGH_DIRECT,   
		&sptdwb, 
		dwSize, 
		&sptdwb, 
		dwSize, 
		&dwReceBufLen,
		NULL);  
	if (iRet == 0)
	{
// 		LPVOID lpMsgBuf =NULL;
// 		TCHAR szBuf[128];
// 		DWORD dw = GetLastError(); 
// 		FormatMessage(   
// 			FORMAT_MESSAGE_ALLOCATE_BUFFER   |   
// 			FORMAT_MESSAGE_FROM_SYSTEM   |   
// 			FORMAT_MESSAGE_IGNORE_INSERTS, 
// 			NULL, 
// 			dw, 
// 			MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),   //   Default   language 
// 			(LPTSTR)   &lpMsgBuf, 
// 			0, 
// 			NULL   
// 			); 
// 		wsprintf(szBuf,("%s ������Ϣ (�����룽%d): %s"),("SCSI_IOCTL_DATA_OUT"), dw, lpMsgBuf);
// 		printf("%s\n",szBuf);
		sw1 = 0x6F;
		sw2 = 0x81;
		goto END;
	}

	sptdwb.sptd.DataIn                  = SCSI_IOCTL_DATA_IN;  
	sptdwb.sptd.DataTransferLength      = sizeof(bReceBuf);    
	sptdwb.sptd.DataBuffer              = bReceBuf;  

	dwSize = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);  
	iRet = DeviceIoControl(hDev, 
		IOCTL_SCSI_PASS_THROUGH_DIRECT,   
		&sptdwb, 
		dwSize, 
		&sptdwb, 
		dwSize, 
		&dwReceBufLen,
		NULL);  
	if (iRet == 0)
	{
// 		LPVOID lpMsgBuf =NULL;
// 		TCHAR szBuf[128];
// 		DWORD dw = GetLastError(); 
// 		FormatMessage(   
// 			FORMAT_MESSAGE_ALLOCATE_BUFFER   |   
// 			FORMAT_MESSAGE_FROM_SYSTEM   |   
// 			FORMAT_MESSAGE_IGNORE_INSERTS, 
// 			NULL, 
// 			dw, 
// 			MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),   //   Default   language 
// 			(LPTSTR)   &lpMsgBuf, 
// 			0, 
// 			NULL   
// 			); 
// 		wsprintf(szBuf,("%s ������Ϣ (�����룽%d): %s"),("SCSI_IOCTL_DATA_IN"), dw, lpMsgBuf);
// 		printf("%s\n",szBuf);
		sw1 = 0x6F;
		sw2 = 0x81;
		goto END;
	}
	else
	{
		rlen = bReceBuf[1];
		if(rlen <2)
		{
// 			printf("receive buffer Len is wrong!\n");
			sw1 = 0x6F;
			sw2 = 0x81;
			goto END;
		}
		else
		{
			sw1 = bReceBuf[rlen];
			sw2 = bReceBuf[rlen+1];
			memcpy(apdu->recvbuf, bReceBuf + 2, rlen);
			apdu->le = rlen -2;
		}
	}
END:
	retcode = ErrorCodeConvert(sw1,sw2);
	return retcode;

}
// ====================================================================
// EXTERN_C DWORD WINAPI API_SendApdu(int index,pAPDUEx apdu);
// ���ܣ�����APDU����ͨѶ��
// ������
// index	�豸��ȫ���豸�б��е�������
// apdu	    APDU���ݰ�
// ���أ�
// SCARD_S_SUCCESS	��������,
// ====================================================================
int WINAPI API_SendApdu(int index,pAPDUEx apdu)
{
	int retcode;
	BYTE sw1, sw2;        //���ش���

	g_DevList[index].mutex.EnterSynCode();

	retcode = SendApdu(g_DevList[index].hCard,apdu);
	sw1=apdu->recvbuf[apdu->le];
	sw2=apdu->recvbuf[apdu->le+1];

	g_DevList[index].mutex.LeaveSynCode();


	return retcode;
}



// =====================================================================================================
// EXTERN_C DWORD WINAPI API_EnumDev(LPSTR szNameList, int *ReaderNum,ULONG *pulSize);
// ���ܣ�ö�������Ѿ����ӵģ����ҿ��Գɹ��򿪵������豸�������豸��Ϣ�洢��һ���豸���б���
// ������
// szNameList	�洢�豸�����б�ռ�	Out	ÿ���豸�������Ե�����\0������,��˫��\0����ʾ�б�Ľ�����
// ReaderNum	�����ӿɴ򿪵��豸����	Out	��С��0��ֵ
// pulSize	�豸�����б�洢�ռ��С	In/Out	����ʱ��ʾ�����豸�����б�洢�ռ��С��
//                                              ���ʱ��ʾ����ʵ�ʵ��豸�����б�洢�ռ��С��
// ���أ�
// SCARD_S_SUCCESS	��������,
// ======================================================================================================
EXTERN_C int WINAPI API_EnumDev(LPSTR szNameList, int *ReaderNum,ULONG *pulSize)
{
	DWORD dwRet = SCARD_S_SUCCESS;
	int num = -1;
	int i = 0;
	DWORD NameLen =0,offset =0,listsize=0;

	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		*pulSize = 0;
		return SCARD_E_NO_SMARTCARD;	
	}
	//find the Application
	for(i=0;i<num;i++)
	{
		if(g_DevList[i].strName[0]!='\0')
		{	
			NameLen = (ULONG)strlen(g_DevList[i].strName)+ 1;
			offset+=NameLen;
		}
	}
	listsize = offset+1;
	if(szNameList==NULL||*pulSize<listsize)
	{
		*pulSize = listsize;	
		return SCARD_E_INSUFFICIENT_BUFFER;

	}
	offset =0;
	for(i=0;i<num;i++)
	{
		if(g_DevList[i].strName[0]!='\0')
		{	
			NameLen = (ULONG)strlen(g_DevList[i].strName)+ 1;
			memcpy(szNameList+offset,g_DevList[i].strName,NameLen);
			offset+=NameLen;
		}
	}
	*pulSize = listsize;
	szNameList[listsize-1]='\0';
	*ReaderNum = num;
	return SCARD_S_SUCCESS;
}
// =====================================================================================================
// EXTERN_C int WINAPI API_ConnectDev(LPSTR szName);
// ���ܣ�����ָ�����豸���ƴ��豸 
// �����豸��index
// ������
// szName	ָ�����豸����
// ���أ�
//			һ����С��0��ֵ	��������,
//			-1	����ʧ��
// ======================================================================================================
EXTERN_C int WINAPI API_ConnectDev(LPSTR szName)
{
	int i=0,index = -1;
	int num = -1;

	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return SCARD_E_NO_SMARTCARD;	
	}
	for(i=0;i<MAX_READERS;i++)
	{
		if(strcmp(g_DevList[i].strName,szName)==0)
		{
			index=i;
			break;
		}
	}
	if(i==MAX_READERS)
	{
		return SCARD_E_READER_UNAVAILABLE;
	}

	g_DevList[index].mutex.EnterSynCode();
	g_DevList[index].hCard = CreateFile(g_DevList[index].strPath,
										GENERIC_READ | GENERIC_WRITE,
										0,
										NULL,
										OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL,
										NULL
										);
	if(g_DevList[index].hCard == INVALID_HANDLE_VALUE)
	{
		g_DevList[index].bOpened=0;
		g_DevList[index].mutex.LeaveSynCode();
		DWORD dwRet = GetLastError();
		if(0x20 == dwRet)
		{
			return DEVICE_IN_USE ;
		}
		else
		{
			return SCARD_ERR_NO_SUCH_DEVICE;
		}
	}
	g_DevList[index].bOpened=1;
	g_DevList[index].mutex.LeaveSynCode();
	return index;	
}
// =====================================================================================================
// EXTERN_C int WINAPI API_ConnectOneDev();
// ���ܣ�Ĭ�ϴ򿪵�һ�������豸�������豸index
// ��������
// ���أ�
//		һ����С��0��ֵ	��������,
//		-1	ʧ��
// ======================================================================================================
EXTERN_C int WINAPI API_ConnectOneDev()
{
	int num = -1;

	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return SCARD_E_NO_SMARTCARD;	
	}
	if(g_DevList[0].strName[0]!='\0')
	{
		g_DevList[0].mutex.EnterSynCode();

		g_DevList[0].hCard = CreateFile(g_DevList[0].strPath,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);		
		if(g_DevList[0].hCard == INVALID_HANDLE_VALUE)
		{
			g_DevList[0].bOpened=0;
			g_DevList[0].mutex.LeaveSynCode();
			DWORD dwRet = GetLastError();
			if(0x20 == dwRet)
			{
				return DEVICE_IN_USE ;
			}
			else
			{
				return SCARD_ERR_NO_SUCH_DEVICE;
			}
			
		}
		g_DevList[0].bOpened=1;
		g_DevList[0].mutex.LeaveSynCode();
		return 0;	
	}
	return -1;
}