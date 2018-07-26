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
	//获取手部状态
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
//检测右手是否握拳
bool CMeasure::RightHandClose()
{
	HandState rightHandState = HandState_Unknown;
	//获取手部状态
	pSingleBody->get_HandRightState(&rightHandState);
	if (HandState_Closed == rightHandState)
	{
		//cout << "右手握拳\n";
		return true;
	}
	else
	{
		//cout << "右手张开\n";
		return false;
	}
}
//检测手臂是否水平展开
bool CMeasure::IsArmSpread(int hand, int shoulder)
{
	//手臂伸展开的距离
	const double ArmStretched = 0.45;
	// 获取骨骼信息节点  
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

//打开骨骼摄像头
bool CMeasure::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	HRESULT hr = S_OK;
	kinect->get_BodyFrameSource(&this->bady);
	hr = bady->OpenReader(&this->pBodyReader); // 打开获取骨骼信息的  Reader  
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
//动作检测
float CMeasure::GestureRecognition()
{
	this->isOK = false;
	HRESULT hr = S_OK;
	while(pBodyReader->AcquireLatestFrame(&this->pBodyFrame) != S_OK);
	
	IBody* pBody[BODY_COUNT] = { 0 };
	//更新骨骼数据    
	while (pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody) != S_OK); // 刷新骨骼信息（6个）  
	
	BOOLEAN bTracked = false;

	for (int i = 0; i < BODY_COUNT; i++)
	{
		hr = pBody[i]->get_IsTracked(&bTracked); // 检查是否被追踪  
	}

	for (int i = 0; i < BODY_COUNT; i++)
	{
		this->pSingleBody = pBody[i];
		if (this->pSingleBody)
		{
			//获取骨骼信息节点  
			this->pSingleBody->GetJoints(JointType::JointType_Count, this->joints);
			
			if (this->RightHandClose())
			{
				this->MeasureXiuChang();
				this->MeasureHeight();
				this->MeasureJianKuan();
				this->MeasureYiChang();
				//以胸围为基础
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
	SafeRelease(this->pBodyFrame);  // 别忘了释放 
	

	return this->isOK;
}
//初始化计数器
int CMeasure::count = 0;
//计算袖长
void CMeasure::MeasureXiuChang()
{
	CameraSpacePoint wristRight = joints[JointType_WristRight].Position;
	CameraSpacePoint shoulderRight = joints[JointType_ShoulderRight].Position;
	this->xiuChang += sqrt(pow(abs(wristRight.X - shoulderRight.X), 2) + pow(abs(wristRight.Y
		- shoulderRight.Y), 2) + pow(abs(wristRight.Z - shoulderRight.Z), 2));
}
//计算摆围
void CMeasure::MeasureBaiWei()
{
	this->baiWei = this->xiongWei / 2;
}
//计算身高
void CMeasure::MeasureHeight()
{
	CameraSpacePoint head = joints[JointType::JointType_Head].Position;
	CameraSpacePoint footRight = joints[JointType::JointType_FootRight].Position;
	this->height += sqrt(pow(abs(head.X - footRight.X), 2) + pow(abs(head.Y
		- footRight.Y), 2) + pow(abs(head.Z - footRight.Z), 2));
}
//计算肩宽
void CMeasure::MeasureJianKuan()
{
	CameraSpacePoint shoulderLeft = joints[JointType::JointType_ShoulderLeft].Position;
	CameraSpacePoint shoulderRight = joints[JointType_ShoulderRight].Position;
	this->jiankuan += sqrt(pow(abs(shoulderLeft.X - shoulderRight.X), 2) + pow(abs(shoulderLeft.Y
		- shoulderRight.Y), 2) + pow(abs(shoulderLeft.Z - shoulderRight.Z), 2));
}
//计算开领
void CMeasure::MeasureKaiLing()
{
	CameraSpacePoint neck = joints[JointType::JointType_Neck].Position;
	CameraSpacePoint spineShoulder = joints[JointType::JointType_SpineShoulder].Position;
	this->kaiLing += sqrt(pow(abs(neck.X - spineShoulder.X), 2) + pow(abs(neck.Y
		- spineShoulder.Y), 2) + pow(abs(neck.Z - spineShoulder.Z), 2));
}
//计算胸围
void CMeasure::MeasureXiongWei()
{
	this->xiongWei = 0.8677 + 1.9561 * 17 + 0.8408*(this->jiankuan - 10) + 0.3962 * this->jiankuan;
}
//计算腰围
void CMeasure::MeasureYaoWei()
{
	this->yaoWei = this->xiongWei;
}
//计算衣长
void CMeasure::MeasureYiChang()
{
	CameraSpacePoint spineShoulder = joints[JointType::JointType_SpineShoulder].Position;
	CameraSpacePoint spineBase = joints[JointType::JointType_SpineBase].Position;
	this->yiChang += sqrt(pow(abs(spineShoulder.X - spineBase.X), 2) + pow(abs(spineShoulder.Y
		- spineBase.Y), 2) + pow(abs(spineShoulder.Z - spineBase.Z), 2));
}
