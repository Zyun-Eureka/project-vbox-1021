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
	ModelProcessor() = delete;  //ʲô��˼
	~ModelProcessor();

	//yolo5ģ�͵�Ԥ����������һ��ͼ��洢��ʽΪRGB��ͼ���СΪģ��ָ����С������OnnxRuntime��Ҫ��Ort::Value���Ͳ��ҷ���
	std::vector<Ort::Value> Preprecess(const cv::Mat& image);

	// yolov5����(��Ҫ�Ƕ�ȡԭʼonnxruntime���ɵ����ݲ�������nms���� �ĵ�������ֵ�Ľ�����ϲ�����)
	std::vector<ResultNode> Postprocess(const std::vector<Ort::Value>& outTensor,
		const cv::Size& originalImageShape,
		float confThreshold, float iouThreshold);

private:
	//��ͼ���һ��ͳһ��С������ģ������ߴ�
	cv::Mat Letterbox(const cv::Mat& image, const cv::Size& newShape = cv::Size(640, 640),
		const cv::Scalar& color = cv::Scalar(114, 114, 114), bool scaleFill = false, bool scaleUp = true,
		int stride = 32);

	//ת��ͼ��Ϊrgb��ʽ
	bool ConvertToRGB(const cv::Mat& image, cv::Mat& outImage);

	//����ԭʼͼ���е�����
	void GetOriCoords(const cv::Size& currentShape, const cv::Size& originalShape, cv::Rect& outCoords);

	//��ȡ��ǰ����score��ߵ�һ��
	void GetBestClassInfo(std::vector<float>::iterator it, const int& numClasses, float& bestConf, int& bestClassId);

	void ParseRawOutput(const std::vector<Ort::Value>& tensor, float conf_threshold, std::vector<cv::Rect>& boxes, std::vector<float>& confs, std::vector<int>& classIds);

private:
	Model* model_ = nullptr;
	float* blob_ = nullptr;
	size_t blobSize_ = 0;

	Ort::MemoryInfo memInfo_{ nullptr };  //memInfo_{ nullptr }��ʲô�﷨

};

