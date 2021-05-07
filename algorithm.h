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
	// ���·����ʼ����յ�
	int beginPoint, endPoint;
	void be_findClosestPoint(float x1, float y1, float x2, float y2);
	// �����㵽·��ӳ��,�����������·������,һ����������ڶ���·
	std::unordered_map<int, std::vector<Data::Road>> index_node_to_road;
	void create_node_to_road();


	/***************** �㷨 *****************/
	/*               ���·��               */
	std::vector<Data::Node> path;
	/*˵�����ڴ˴�����Լ�ʵ�ֵ����·���㷨*/
	/*          ʾ���㷨���Ͻ�˹����        */
	void dijkstra();
	/************ ʾ�������Լ����㷨 ********/
	/*                                      */
	// void yourAlgorithm();
	/*                                      */
	/* ������Լ����㷨ʱ��dijkstraʾ���㷨 */
	/* ע�͵�,�ڴ˴���������㷨����,�ҵ�cpp*/
	/* �ļ���ȥʵ���������,�����main������*/
	/* �������Լ����㷨����                 */
	/****************************************/

};