
// KinectMeasure.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKinectMeasureApp: 
// �йش����ʵ�֣������ KinectMeasure.cpp
//

class CKinectMeasureApp : public CWinApp
{
public:
	CKinectMeasureApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKinectMeasureApp theApp;