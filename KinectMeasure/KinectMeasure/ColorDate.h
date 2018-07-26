#pragma once

#include "include.h"

class CColorDate
{
public:
	CColorDate();
	~CColorDate();
private:
	//kinect设备
	IKinectSensor* kinect;
	//彩色数据
	int colorHeight, colorWidth;
	IColorFrameSource* myColorSource;
	IColorFrameReader* myColorReader;
	IFrameDescription* myDescription;
	UINT    colorDataSize;
	DepthSpacePoint* output;
	IColorFrame* myColorFrame;
	Mat colorData;

public:
	//开启彩色帧
	bool OpenColor(IKinectSensor* kinect);
	//得到彩色数据
	Mat GetColorDate();
};

