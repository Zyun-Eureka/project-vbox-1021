#pragma once
#include <string>
#include <vector>
#include "YoloDefine.h"
#include <opencv2/opencv.hpp>
#include "Model.h"

class ISession
{
public:
	ISession() = default; //���캯��
	~ISession() = default; //��������

	//ģ�ͳ�ʼ��
	virtual bool Initialize(const std::string& modelPath) = 0;

	//����
	virtual std::vector<ResultNode> Detect(const cv::Mat& image) = 0;

	//��ȡģ�Ͳ���
	virtual Model* GetModel() { return model_; };  //���ﲻ���� model_��ʲô

	//����confidence��ֵ
	virtual void SetConfidence(float conf) { confidenceThreshold_ = conf; }

	//����iou��ֵ
	virtual void SetIou(float iou) { iouThreshold_ = iou; };

	//protected:
	//	virtual bool WarmUpModel() = 0;


protected:
	virtual bool WarmUpModel() = 0;

	Model* model_;

	float confidenceThreshold_;
	float iouThreshold_;
};