#pragma once
//定义输出结构体

typedef struct _DetectionResultNode
{
	float x, y, w, h;  //bounding box 位置
	int classIdx;   //类别编号
	float confidence;  //置信度
}ResultNode, *pResultNode;


struct RawResult
{
	float cx;
	float cy;
	float w;
	float h;
	float cls_conf;
};

typedef RawResult* pRawResult;

//占用多少个，1个box占用5个元素
#define YOLOV5_OUTBOX_ELEMENT_COUNT 5