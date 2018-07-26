#pragma once

#include "include.h"

class CMeasure
{
public:
	CMeasure();
	~CMeasure();

private:
	//kinect设备
	IKinectSensor* kinect;
	// 获取源  
	IBodyFrameSource* bady;
	//身体框架读取
	IBodyFrameReader* pBodyReader;
	//身体框架
	IBodyFrame* pBodyFrame;

	// 默认的是 6 个骨骼 ，初始化所有的骨骼信息 
	//	IBody* pBody[BODY_COUNT];
	//单个人物
	IBody* pSingleBody;
	Joint joints[JointType_Count];

public:
	//计时器
	static int count;

private:
	//检测左手是否握拳
	bool LeftHandClose();
	//检测右手是否握拳
	bool RightHandClose();
	//检测手臂是否水平展开
	bool IsArmSpread(int hand, int shoulder);

public:
	//打开骨骼帧
	bool Open(IKinectSensor* kinect);
	//动作识别
	float GestureRecognition();
public:
	//
	bool isOK;
	//身高
	float height;
	//袖长
	float xiuChang;
	//摆围
	float baiWei;
	//腰围
	float yaoWei;
	//胸围
	float xiongWei;
	//肩宽
	float jiankuan;
	//开领
	float kaiLing;
	//衣长
	float yiChang;
private:
	//计算袖长
	void MeasureXiuChang();
	//计算摆围
	void MeasureBaiWei();
	//计算腰围
	void MeasureYaoWei();
	//计算胸围
	void MeasureXiongWei();
	//计算肩宽
	void MeasureJianKuan();
	//计算开领
	void MeasureKaiLing();
	//计算衣长
	void MeasureYiChang();
	//计算身高
	void MeasureHeight();
};

