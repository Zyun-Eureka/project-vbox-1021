#pragma once

#include "YoloDefine.h"
#include "Model.h"
#include <vector>
#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>

class ModelProcessor
{
public:
	ModelProcessor(Model* model);
	ModelProcessor() = delete;  //什么意思
	~ModelProcessor();

	//yolo5模型的预处理函数，归一化图像存储格式为RGB，图像大小为模型指定大小，生成OnnxRuntime需要的Ort::Value类型并且返回
	std::vector<Ort::Value> Preprecess(const cv::Mat& image);

	// yolov5后处理(主要是读取原始onnxruntime生成的数据并解析后经nms处理 的到符合阈值的结果集合并返回)
	std::vector<ResultNode> Postprocess(const std::vector<Ort::Value>& outTensor,
		const cv::Size& originalImageShape,
		float confThreshold, float iouThreshold);

private:
	//将图像归一化统一大小，符合模型输入尺寸
	cv::Mat Letterbox(const cv::Mat& image, const cv::Size& newShape = cv::Size(640, 640),
		const cv::Scalar& color = cv::Scalar(114, 114, 114), bool scaleFill = false, bool scaleUp = true,
		int stride = 32);

	//转换图像为rgb格式
	bool ConvertToRGB(const cv::Mat& image, cv::Mat& outImage);

	//计算原始图像中的坐标
	void GetOriCoords(const cv::Size& currentShape, const cv::Size& originalShape, cv::Rect& outCoords);

	//获取当前框中score最高的一个
	void GetBestClassInfo(std::vector<float>::iterator it, const int& numClasses, float& bestConf, int& bestClassId);

	void ParseRawOutput(const std::vector<Ort::Value>& tensor, float conf_threshold, std::vector<cv::Rect>& boxes, std::vector<float>& confs, std::vector<int>& classIds);

private:
	Model* model_ = nullptr;
	float* blob_ = nullptr;
	size_t blobSize_ = 0;

	Ort::MemoryInfo memInfo_{ nullptr };  //memInfo_{ nullptr }是什么语法

};

