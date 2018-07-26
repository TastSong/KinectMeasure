#include "stdafx.h"
#include "Measure.h"
#include "include.h"
#include <Kinect.h>
#include <time.h>



CMeasure::CMeasure()
{
	this->kinect = nullptr;
	this->bady = nullptr;
	this->pBodyReader = nullptr;
	this->pBodyFrame = nullptr;
	//	this->pBody[BODY_COUNT] = { 0 }; 
	this->pSingleBody = nullptr;
	//Joint joints[JointType_Count];

	this->height = 0;
	this->xiuChang = 0;
	this->baiWei = 0;
	this->jiankuan = 0;
	this->kaiLing = 0;
	this->xiongWei = 0;
	this->yaoWei = 0;
	this->yiChang = 0;
}

CMeasure::~CMeasure()
{
}
bool CMeasure::LeftHandClose()
{
	HandState leftHandState = HandState_Unknown;
	//��ȡ�ֲ�״̬
	pSingleBody->get_HandLeftState(&leftHandState);
	if (HandState_Closed == leftHandState)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//��������Ƿ���ȭ
bool CMeasure::RightHandClose()
{
	HandState rightHandState = HandState_Unknown;
	//��ȡ�ֲ�״̬
	pSingleBody->get_HandRightState(&rightHandState);
	if (HandState_Closed == rightHandState)
	{
		//cout << "������ȭ\n";
		return true;
	}
	else
	{
		//cout << "�����ſ�\n";
		return false;
	}
}
//����ֱ��Ƿ�ˮƽչ��
bool CMeasure::IsArmSpread(int hand, int shoulder)
{
	//�ֱ���չ���ľ���
	const double ArmStretched = 0.45;
	// ��ȡ������Ϣ�ڵ�  
	float handX = joints[hand].Position.X;
	float shoulderX = joints[shoulder].Position.X;

	if (fabs(handX - shoulderX) > ArmStretched)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�򿪹�������ͷ
bool CMeasure::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	HRESULT hr = S_OK;
	kinect->get_BodyFrameSource(&this->bady);
	hr = bady->OpenReader(&this->pBodyReader); // �򿪻�ȡ������Ϣ��  Reader  
	if (FAILED(hr))
	{
		//std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}
//�������
float CMeasure::GestureRecognition()
{
	this->isOK = false;
	HRESULT hr = S_OK;
	while(pBodyReader->AcquireLatestFrame(&this->pBodyFrame) != S_OK);
	
	IBody* pBody[BODY_COUNT] = { 0 };
	//���¹�������    
	while (pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody) != S_OK); // ˢ�¹�����Ϣ��6����  
	
	BOOLEAN bTracked = false;

	for (int i = 0; i < BODY_COUNT; i++)
	{
		hr = pBody[i]->get_IsTracked(&bTracked); // ����Ƿ�׷��  
	}

	for (int i = 0; i < BODY_COUNT; i++)
	{
		this->pSingleBody = pBody[i];
		if (this->pSingleBody)
		{
			//��ȡ������Ϣ�ڵ�  
			this->pSingleBody->GetJoints(JointType::JointType_Count, this->joints);
			
			if (this->RightHandClose())
			{
				this->MeasureXiuChang();
				this->MeasureHeight();
				this->MeasureJianKuan();
				this->MeasureYiChang();
				//����ΧΪ����
				this->MeasureKaiLing();
				this->MeasureXiongWei();
				this->MeasureBaiWei();
				this->MeasureYaoWei();

				this->count++;	
			}

		}
			
	}//for
	for (int count = 0; count < BODY_COUNT; count++)
	{
		SafeRelease(pBody[count]);
	}
	SafeRelease(this->pBodyFrame);  // �������ͷ� 
	

	return this->isOK;
}
//��ʼ��������
int CMeasure::count = 0;
//�����䳤
void CMeasure::MeasureXiuChang()
{
	CameraSpacePoint wristRight = joints[JointType_WristRight].Position;
	CameraSpacePoint shoulderRight = joints[JointType_ShoulderRight].Position;
	this->xiuChang += sqrt(pow(abs(wristRight.X - shoulderRight.X), 2) + pow(abs(wristRight.Y
		- shoulderRight.Y), 2) + pow(abs(wristRight.Z - shoulderRight.Z), 2));
}
//�����Χ
void CMeasure::MeasureBaiWei()
{
	this->baiWei = this->xiongWei / 2;
}
//�������
void CMeasure::MeasureHeight()
{
	CameraSpacePoint head = joints[JointType::JointType_Head].Position;
	CameraSpacePoint footRight = joints[JointType::JointType_FootRight].Position;
	this->height += sqrt(pow(abs(head.X - footRight.X), 2) + pow(abs(head.Y
		- footRight.Y), 2) + pow(abs(head.Z - footRight.Z), 2));
}
//������
void CMeasure::MeasureJianKuan()
{
	CameraSpacePoint shoulderLeft = joints[JointType::JointType_ShoulderLeft].Position;
	CameraSpacePoint shoulderRight = joints[JointType_ShoulderRight].Position;
	this->jiankuan += sqrt(pow(abs(shoulderLeft.X - shoulderRight.X), 2) + pow(abs(shoulderLeft.Y
		- shoulderRight.Y), 2) + pow(abs(shoulderLeft.Z - shoulderRight.Z), 2));
}
//���㿪��
void CMeasure::MeasureKaiLing()
{
	CameraSpacePoint neck = joints[JointType::JointType_Neck].Position;
	CameraSpacePoint spineShoulder = joints[JointType::JointType_SpineShoulder].Position;
	this->kaiLing += sqrt(pow(abs(neck.X - spineShoulder.X), 2) + pow(abs(neck.Y
		- spineShoulder.Y), 2) + pow(abs(neck.Z - spineShoulder.Z), 2));
}
//������Χ
void CMeasure::MeasureXiongWei()
{
	this->xiongWei = 0.8677 + 1.9561 * 17 + 0.8408*(this->jiankuan - 10) + 0.3962 * this->jiankuan;
}
//������Χ
void CMeasure::MeasureYaoWei()
{
	this->yaoWei = this->xiongWei;
}
//�����³�
void CMeasure::MeasureYiChang()
{
	CameraSpacePoint spineShoulder = joints[JointType::JointType_SpineShoulder].Position;
	CameraSpacePoint spineBase = joints[JointType::JointType_SpineBase].Position;
	this->yiChang += sqrt(pow(abs(spineShoulder.X - spineBase.X), 2) + pow(abs(spineShoulder.Y
		- spineBase.Y), 2) + pow(abs(spineShoulder.Z - spineBase.Z), 2));
}
