#pragma once
#include "ISession.h"
#include "YoloDefine.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "onnxruntime_cxx_api.h"
#include "ModelProcessor.h"
#include "ModelParser.h"


class Yolov5Session : public ISession  //public 在这里是什么意思
{
public:
	Yolov5Session();
	~Yolov5Session();

	bool Initialize(const std::string& modelPath);
	
	std::vector<ResultNode> Detect(const cv::Mat& image);

private:
	bool CreateSession(const std::filesystem::path& modelPath);

	bool ParasModel();

	OrtCUDAProviderOptions CreateCudaOptions();

	bool IsGPUAvailable();

protected:
	bool WarmUpModel() override;  //override是什么意思


private:
	Ort::Session session_{ nullptr };      //这里都不知道什么意思
	Ort::SessionOptions sessionOpt{ nullptr };
	Ort::Env env_{ nullptr };
	std::string envName_;

	ModelProcessor* processor_ = nullptr;
	float confidence = 0.5;
	float iou = 0.6;
	
	bool useGpu = true;
	bool warmup = true;
};

