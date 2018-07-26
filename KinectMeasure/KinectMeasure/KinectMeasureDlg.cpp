
// KinectMeasureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KinectMeasure.h"
#include "KinectMeasureDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKinectMeasureDlg �Ի���



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


// CKinectMeasureDlg ��Ϣ�������

BOOL CKinectMeasureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//ȫ��
	this->FllScreen();
	//picture�ؼ���ʼ��
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC_STATIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	//��ʼ��kinect
	kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  �õ�Ĭ�ϵ��豸  

	if (FAILED(hr) || kinect == NULL)
	{
		//cout << "���� sensor ʧ��\n";
		//return;
	}
	if (kinect->Open() != S_OK) // �Ƿ�򿪳ɹ�  
	{
		//cout << "Kinect sensor û׼����\n";
		//return;
	}

	this->cd.OpenColor(kinect);
	this->measure.Open(kinect);
	SetTimer(1, 33, NULL);
	SetTimer(2, 33, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKinectMeasureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CKinectMeasureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKinectMeasureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//���²�����ȡͼ�οؼ��ߴ粢�Դ˸ı�ͼƬ�ߴ�  
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
		//����ΧΪ����
		ShowXiongWei();
		ShowBaiWei();
		ShowYaoWei();
		ShowKaiLing();
		//ˢ�½�������
		UpdateData(FALSE);

		KillTimer(2);
		//30������¼���
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
	//��������
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
	//ˢ�½�������
	UpdateData(FALSE);
	//�������
	SetTimer(2, 33, NULL);
}
//ȫ��
void CKinectMeasureDlg::FllScreen()
{
	CRect rect;
	POINT old;
	GetClientRect(&rect);     //ȡ�÷Ŵ�ǰ�ͻ�����С 
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);   //ȫ��1
	//ShowWindow(SW_SHOWMAXIMIZED);  //ȫ��2
	CRect recttemp;
	CRect recta;
	POINT bili;
	float fsp[2];
	GetClientRect(&recta);  //���ȫ�����client��С 
	bili.x = recta.right - recta.left;       // ���ȫ�����xy�����
	bili.y = recta.bottom - recta.top;

	fsp[0] = (float)bili.x / old.x;    //�����ȫ��ǰ��ı�����ϵ
	fsp[1] = (float)bili.y / old.y;



	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�
	CPoint OldBRPoint, BRPoint; //���½�

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ� 
	while (hwndChild)          // ������ÿһ���ռ䶼���б�������
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID
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

