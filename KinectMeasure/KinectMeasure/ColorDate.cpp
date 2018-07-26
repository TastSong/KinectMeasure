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
	//�ǵø����ͷ�
	delete[] output;

	this->myColorReader->Release();
}

bool CColorDate::OpenColor(IKinectSensor* kinect)
{
	this->kinect = kinect;
	//׼���ò�ɫͼ���Reader����ȡ�ߴ�
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
	//Ϊͼ��׼��buffer
	this->colorDataSize = this->colorHeight * this->colorWidth;
	this->output = new DepthSpacePoint[this->colorDataSize];
	//׼��buffer
	Mat colorData(colorHeight, colorWidth, CV_8UC4);
	this->colorData = colorData;

	return true;
}

Mat CColorDate::GetColorDate()
{	
	while (this->myColorReader->AcquireLatestFrame(&this->myColorFrame) != S_OK);   //��ȡcolorͼ
	this->myColorFrame->CopyConvertedFrameDataToArray(this->colorDataSize * 4, this->colorData.data, ColorImageFormat_Bgra);
	this->myColorFrame->Release();

	return this->colorData;
}