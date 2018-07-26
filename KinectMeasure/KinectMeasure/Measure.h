#pragma once

#include "include.h"

class CMeasure
{
public:
	CMeasure();
	~CMeasure();

private:
	//kinect�豸
	IKinectSensor* kinect;
	// ��ȡԴ  
	IBodyFrameSource* bady;
	//�����ܶ�ȡ
	IBodyFrameReader* pBodyReader;
	//������
	IBodyFrame* pBodyFrame;

	// Ĭ�ϵ��� 6 ������ ����ʼ�����еĹ�����Ϣ 
	//	IBody* pBody[BODY_COUNT];
	//��������
	IBody* pSingleBody;
	Joint joints[JointType_Count];

public:
	//��ʱ��
	static int count;

private:
	//��������Ƿ���ȭ
	bool LeftHandClose();
	//��������Ƿ���ȭ
	bool RightHandClose();
	//����ֱ��Ƿ�ˮƽչ��
	bool IsArmSpread(int hand, int shoulder);

public:
	//�򿪹���֡
	bool Open(IKinectSensor* kinect);
	//����ʶ��
	float GestureRecognition();
public:
	//
	bool isOK;
	//���
	float height;
	//�䳤
	float xiuChang;
	//��Χ
	float baiWei;
	//��Χ
	float yaoWei;
	//��Χ
	float xiongWei;
	//���
	float jiankuan;
	//����
	float kaiLing;
	//�³�
	float yiChang;
private:
	//�����䳤
	void MeasureXiuChang();
	//�����Χ
	void MeasureBaiWei();
	//������Χ
	void MeasureYaoWei();
	//������Χ
	void MeasureXiongWei();
	//������
	void MeasureJianKuan();
	//���㿪��
	void MeasureKaiLing();
	//�����³�
	void MeasureYiChang();
	//�������
	void MeasureHeight();
};

