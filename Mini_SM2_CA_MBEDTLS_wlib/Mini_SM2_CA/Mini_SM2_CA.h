// Mini_SM2_CA.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMini_SM2_CAApp:
// �йش����ʵ�֣������ Mini_SM2_CA.cpp
//

class CMini_SM2_CAApp : public CWinApp
{
public:
	CMini_SM2_CAApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMini_SM2_CAApp theApp;