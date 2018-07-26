#pragma once

#include "include.h"

class CColorDate
{
public:
	CColorDate();
	~CColorDate();
private:
	//kinect�豸
	IKinectSensor* kinect;
	//��ɫ����
	int colorHeight, colorWidth;
	IColorFrameSource* myColorSource;
	IColorFrameReader* myColorReader;
	IFrameDescription* myDescription;
	UINT    colorDataSize;
	DepthSpacePoint* output;
	IColorFrame* myColorFrame;
	Mat colorData;

public:
	//������ɫ֡
	bool OpenColor(IKinectSensor* kinect);
	//�õ���ɫ����
	Mat GetColorDate();
};

