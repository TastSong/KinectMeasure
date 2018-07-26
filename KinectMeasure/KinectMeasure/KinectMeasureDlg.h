
// KinectMeasureDlg.h : ͷ�ļ�
//

#pragma once

#include "resource.h"
#include "ColorDate.h"
#include "include.h"
#include "Measure.h"


// CKinectMeasureDlg �Ի���
class CKinectMeasureDlg : public CDialogEx
{
// ����
public:
	CKinectMeasureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KINECTMEASURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	IKinectSensor *kinect = NULL;
	CColorDate cd;
	CMeasure measure;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
private:
	float m_BaiWei;
	float m_Height;
	float m_JianKuan;
	float m_KaiLing;
	float m_XiongWei;
	float m_XiuChang;
	float m_YaoWei;
	float m_YiChang;
private:
	void ShowColorDate();
	void MeasureAndShow();

	void ShowHeight();
	void ShowXiuChang();
	void ShowYiChang();
	void ShowJianKuan();
	//����ΧΪ����
	void ShowKaiLing();
	void ShowXiongWei();
	void ShowYaoWei();
	void ShowBaiWei();
	//���¿�ʼ����
	void Recount();
   //ȫ��
	void FllScreen();
};
