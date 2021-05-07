#pragma once
#include "render.h"
#include "data.h"

class Algorithm {
public:
	Algorithm(Render* render, Data* data, float x1, float y1, float x2, float y2);
	Render* a_render;
	Data* a_data;
	float distance(float x1, float y1, float x2, float y2) const {
		return std::sqrt(std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2));
	}
	// 最短路径起始点和终点
	int beginPoint, endPoint;
	void be_findClosestPoint(float x1, float y1, float x2, float y2);
	// 建立点到路的映射,即点的索引到路的索引,一个点可能属于多条路
	std::unordered_map<int, std::vector<Data::Road>> index_node_to_road;
	void create_node_to_road();


	/***************** 算法 *****************/
	/*               最短路径               */
	std::vector<Data::Node> path;
	/*说明：在此处添加自己实现的最短路径算法*/
	/*          示例算法：迪杰斯特拉        */
	void dijkstra();
	/************ 示例：你自己的算法 ********/
	/*                                      */
	// void yourAlgorithm();
	/*                                      */
	/* 在添加自己的算法时把dijkstra示例算法 */
	/* 注释掉,在此处声明你的算法函数,且到cpp*/
	/* 文件中去实现这个函数,最后在main函数里*/
	/* 调用你自己的算法函数                 */
	/****************************************/

};