#include "stdafx.h"
#include "ColorDate.h"


CColorDate::CColorDate()
{
	this->kinect = nullptr;

	this->colorHeight = 0;
	this->colorWidth = 0;
	this->myColorSource = nullptr;
	this->myColorReader = nullptr;
	this->myDescription = nullptr;
	this->colorDataSize = 0;
	this->output = nullptr;
	this->myColorFrame = nullptr;
}


CColorDate::~CColorDate()
{
	//记得各种释放
	delete[] output;

	this->myColorReader->Release();
}

bool CColorDate::OpenColor(IKinectSensor* kinect)
{
	this->kinect = kinect;
	//准备好彩色图像的Reader并获取尺寸
	this->kinect->get_ColorFrameSource(&this->myColorSource);
	HRESULT hr = S_OK;
	hr = this->myColorSource->OpenReader(&this->myColorReader);
	if (FAILED(hr))
	{
		return false;
	}
	this->myColorSource->get_FrameDescription(&this->myDescription);
	this->myDescription->get_Height(&this->colorHeight);
	this->myDescription->get_Width(&this->colorWidth);

	this->myDescription->Release();
	this->myColorSource->Release();
	//为图像准备buffer
	this->colorDataSize = this->colorHeight * this->colorWidth;
	this->output = new DepthSpacePoint[this->colorDataSize];
	//准备buffer
	Mat colorData(colorHeight, colorWidth, CV_8UC4);
	this->colorData = colorData;

	return true;
}

Mat CColorDate::GetColorDate()
{	
	while (this->myColorReader->AcquireLatestFrame(&this->myColorFrame) != S_OK);   //读取color图
	this->myColorFrame->CopyConvertedFrameDataToArray(this->colorDataSize * 4, this->colorData.data, ColorImageFormat_Bgra);
	this->myColorFrame->Release();

	return this->colorData;
}