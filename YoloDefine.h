#pragma once
//��������ṹ��

typedef struct _DetectionResultNode
{
	float x, y, w, h;  //bounding box λ��
	int classIdx;   //�����
	float confidence;  //���Ŷ�
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

//ռ�ö��ٸ���1��boxռ��5��Ԫ��
#define YOLOV5_OUTBOX_ELEMENT_COUNT 5