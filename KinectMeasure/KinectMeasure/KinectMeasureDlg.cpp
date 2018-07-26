
// KinectMeasureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KinectMeasure.h"
#include "KinectMeasureDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKinectMeasureDlg 对话框



CKinectMeasureDlg::CKinectMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKinectMeasureDlg::IDD, pParent)
	, m_Height(0)
	, m_BaiWei(0)
	, m_JianKuan(0)
	, m_KaiLing(0)
	, m_XiongWei(0)
	, m_XiuChang(0)
	, m_YaoWei(0)
	, m_YiChang(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKinectMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_EDIT_BAIWEI, m_BaiWei);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_EDIT_JIANKUAN, m_JianKuan);
	DDX_Text(pDX, IDC_EDIT_KAILING, m_KaiLing);
	DDX_Text(pDX, IDC_EDIT_XIONGWEI, m_XiongWei);
	DDX_Text(pDX, IDC_EDIT_XIUCHANG, m_XiuChang);
	DDX_Text(pDX, IDC_EDIT_YAOWEI, m_YaoWei);
	DDX_Text(pDX, IDC_EDIT_YICHANG, m_YiChang);
}

BEGIN_MESSAGE_MAP(CKinectMeasureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKinectMeasureDlg 消息处理程序

BOOL CKinectMeasureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//全屏
	this->FllScreen();
	//picture控件初始化
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC_STATIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	//初始化kinect
	kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  得到默认的设备  

	if (FAILED(hr) || kinect == NULL)
	{
		//cout << "创建 sensor 失败\n";
		//return;
	}
	if (kinect->Open() != S_OK) // 是否打开成功  
	{
		//cout << "Kinect sensor 没准备好\n";
		//return;
	}

	this->cd.OpenColor(kinect);
	this->measure.Open(kinect);
	SetTimer(1, 33, NULL);
	SetTimer(2, 33, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKinectMeasureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKinectMeasureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKinectMeasureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKinectMeasureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case 1:
			this->ShowColorDate();
			break;
		case 2:
			this->MeasureAndShow();
			break;
		case 3:
			this->Recount();
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CKinectMeasureDlg::ShowColorDate()
{
	Mat image =  this->cd.GetColorDate();
	Mat imagedst;
	//以下操作获取图形控件尺寸并以此改变图片尺寸  
	CRect rect;
	GetDlgItem(IDC_PIC_STATIC)->GetClientRect(&rect);
	Rect dst(rect.left, rect.top, rect.right, rect.bottom);
	resize(image, imagedst, cv::Size(rect.Width(), rect.Height()));
	imshow("view", imagedst);
}
void CKinectMeasureDlg::MeasureAndShow()
{
	this->measure.GestureRecognition();
	if (this->measure.count == 100)
	{
		ShowXiuChang();
		ShowHeight();
		ShowJianKuan();
		ShowYiChang();
		//以胸围为基础
		ShowXiongWei();
		ShowBaiWei();
		ShowYaoWei();
		ShowKaiLing();
		//刷新界面数据
		UpdateData(FALSE);

		KillTimer(2);
		//30秒后重新计算
		SetTimer(3, 30000, NULL);
	}	
}
void CKinectMeasureDlg::ShowXiuChang()
{
	this->measure.xiuChang = ((float)((int)((this->measure.xiuChang + 0.005) * 100))) / 100;
	this->m_XiuChang = this->measure.xiuChang / measure.count * 100;
	this->m_XiuChang += 5.1;
}
void CKinectMeasureDlg::ShowHeight()
{
	this->measure.height = ((float)((int)((this->measure.height + 0.005) * 100))) / 100;
	this->m_Height = this->measure.height / measure.count * 100;
	this->m_Height += 10.1;
}
void CKinectMeasureDlg::ShowJianKuan()
{
	this->measure.jiankuan = ((float)((int)((this->measure.jiankuan + 0.005) * 100))) / 100;
	this->m_JianKuan = this->measure.jiankuan / measure.count * 100;
	this->m_JianKuan += 12.1;
}
void CKinectMeasureDlg::ShowYiChang()
{
	this->measure.yiChang = ((float)((int)((this->measure.yiChang + 0.005) * 100))) / 100;
	this->m_YiChang = this->measure.yiChang  / measure.count * 100;
	this->m_YiChang -= 5.1;
}

void CKinectMeasureDlg::ShowXiongWei()
{
	this->measure.xiongWei = ((float)((int)((this->measure.xiongWei + 0.005) * 100))) / 100;
	this->m_XiongWei = this->measure.xiongWei / measure.count * 100;
	this->m_XiongWei += 17;
}
void CKinectMeasureDlg::ShowBaiWei()
{
	this->measure.baiWei = ((float)((int)((this->measure.baiWei + 0.005) * 100))) / 100;
	this->m_BaiWei = this->measure.baiWei / measure.count * 100;
	this->m_BaiWei += 8;
}
void CKinectMeasureDlg::ShowYaoWei()
{
	this->measure.yaoWei = ((float)((int)((this->measure.yaoWei + 0.005) * 100))) / 100;
	this->m_YaoWei = this->measure.yaoWei / measure.count * 100;
	this->m_YaoWei += 14.1;
}
void CKinectMeasureDlg::ShowKaiLing()
{
	this->measure.kaiLing = ((float)((int)((this->measure.kaiLing + 0.005) * 100))) / 100;
	this->m_KaiLing = this->measure.kaiLing / measure.count * 100;
	this->m_KaiLing -= 5;
}
void CKinectMeasureDlg::Recount()
{
	//重置数据
	this->measure.count = 0;

	this->m_BaiWei = 0;
	this->measure.baiWei = 0;
	this->m_Height = 0;
	this->measure.height = 0;
	this->m_KaiLing = 0;
	this->measure.kaiLing = 0;
	this->m_XiongWei = 0;
	this->measure.xiongWei = 0;
	this->m_XiuChang = 0;
	this->measure.xiuChang = 0;
	this->m_JianKuan = 0;
	this->measure.jiankuan = 0;
	this->m_YiChang = 0;
	this->measure.yiChang = 0;
	this->m_YaoWei = 0;
	this->measure.yaoWei = 0;
	//刷新界面数据
	UpdateData(FALSE);
	//激活计算
	SetTimer(2, 33, NULL);
}
//全屏
void CKinectMeasureDlg::FllScreen()
{
	CRect rect;
	POINT old;
	GetClientRect(&rect);     //取得放大前客户区大小 
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);   //全屏1
	//ShowWindow(SW_SHOWMAXIMIZED);  //全屏2
	CRect recttemp;
	CRect recta;
	POINT bili;
	float fsp[2];
	GetClientRect(&recta);  //获得全屏后的client大小 
	bili.x = recta.right - recta.left;       // 获得全屏后的xy轴距离
	bili.y = recta.bottom - recta.top;

	fsp[0] = (float)bili.x / old.x;    //计算出全屏前后的比例关系
	fsp[1] = (float)bili.y / old.y;



	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件 
	while (hwndChild)          // 将其中每一个空间都进行比例缩放
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = bili;

}

