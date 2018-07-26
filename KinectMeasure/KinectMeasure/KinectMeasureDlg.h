
// KinectMeasureDlg.h : 头文件
//

#pragma once

#include "resource.h"
#include "ColorDate.h"
#include "include.h"
#include "Measure.h"


// CKinectMeasureDlg 对话框
class CKinectMeasureDlg : public CDialogEx
{
// 构造
public:
	CKinectMeasureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KINECTMEASURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//以胸围为基础
	void ShowKaiLing();
	void ShowXiongWei();
	void ShowYaoWei();
	void ShowBaiWei();
	//重新开始计算
	void Recount();
   //全屏
	void FllScreen();
};
