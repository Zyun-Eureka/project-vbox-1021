#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "YoloDefine.h"

inline cv::Mat RenderBoundingBoxes(const cv::Mat& image, const std::vector<ResultNode>& boxes,
    const std::vector<std::string>& labels)
{
    cv::Mat out = image.clone();
    for (const auto& box : boxes) {
        cv::rectangle(out, cv::Rect(box.x, box.y, box.w, box.h), cv::Scalar(0, 255, 0), 2); // ������ɫ�߽���߿�Ϊ2

        cv::Point labelPosition(box.x, box.y - 10); // ������ǩλ�ã�ʹ��λ�ڱ߽���Ϸ�
        cv::putText(out, labels[box.classIdx], labelPosition, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    }
    return out;
}