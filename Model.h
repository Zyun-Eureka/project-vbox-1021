#pragma once
#include <vector>
#include <string>

struct Model
{
	Model() = default;
	~Model() = default;  //这里不明白，结构体为什么还有构造和西沟函数

	std::vector<std::string> labels;
	std::vector<std::vector<int64_t>> inputShapes;
	std::vector<std::vector<int64_t>> outpurShapes;

	std::vector<std::string> inputNames;
	std::vector<const char*> inputNamesPtr;

	std::vector<std::string> outputNames;
	std::vector<const char*> outputNamesPtr;

};