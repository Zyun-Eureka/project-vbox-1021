#pragma once
#include <string>
#include <vector>
#include "YoloDefine.h"
#include <opencv2/opencv.hpp>
#include "Model.h"

class ISession
{
public:
	ISession() = default; //构造函数
	~ISession() = default; //析构函数

	//模型初始化
	virtual bool Initialize(const std::string& modelPath) = 0;

	//推理
	virtual std::vector<ResultNode> Detect(const cv::Mat& image) = 0;

	//获取模型参数
	virtual Model* GetModel() { return model_; };  //这里不明白 model_是什么

	//设置confidence阈值
	virtual void SetConfidence(float conf) { confidenceThreshold_ = conf; }

	//设置iou阈值
	virtual void SetIou(float iou) { iouThreshold_ = iou; };

	//protected:
	//	virtual bool WarmUpModel() = 0;


protected:
	virtual bool WarmUpModel() = 0;

	Model* model_;

	float confidenceThreshold_;
	float iouThreshold_;
};