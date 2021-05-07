#include "algorithm.h"

Algorithm::Algorithm(Render* render, Data* data, float x1, float y1, float x2, float y2):
	a_render(render), a_data(data){
	create_node_to_road();
	be_findClosestPoint(x1, y1, x2, y2);
}

void Algorithm::create_node_to_road() {
	for (auto road : a_data->Roads()) {
		for (int index_node : a_data->Ways()[road.way].nodes) {
			index_node_to_road[index_node].push_back(road);
		}
	}
}

void Algorithm::be_findClosestPoint(float x1, float y1, float x2, float y2) {
	float begin_disMin = std::numeric_limits<float>::max();
	float end_disMin = std::numeric_limits<float>::max();
	for (auto it = index_node_to_road.begin(); it != index_node_to_road.end();it++) {
		float x_cur = a_data->Nodes()[it->first].x;
		float y_cur = a_data->Nodes()[it->first].y;
		if (x_cur < 0 || y_cur < 0) continue;
		if (distance(x_cur, y_cur, x1, y1) < begin_disMin) {
			begin_disMin = distance(x_cur, y_cur, x1, y1);
			beginPoint = it->first;
		}
		if (distance(x_cur, y_cur, x2, y2) < end_disMin) {
			end_disMin = distance(x_cur, y_cur, x2, y2);
			endPoint = it->first;
		}
	}
}

bool myIntersection(std::vector<Data::Road> A, std::vector<Data::Road> B) {
	for (auto RA : A) {
		for (auto RB : B) {
			if (RA.way == RB.way && RA.type == RB.type) {
				return true;
			}
		}
	}
	return false;
}

void Algorithm::dijkstra() {
	// ����ʼ�����·����
	std::unordered_map<int, std::vector<int>> everyPath;
	everyPath[beginPoint].push_back(beginPoint);
	// dist  keyΪ�õ������,valueΪ�õ㵽��ʼ��ľ���
	std::unordered_map<int, float> dist;
	// ��ʼ��dist
	for (auto it = index_node_to_road.begin(); it != index_node_to_road.end(); it++) {
		/* �˴�Ҫ�ر�ע��, ������������һ��·�ϣ���ô���ǵľ��벻����ֱ�߾��붨��Ӧ��Ϊ */
		/* �����ֻ����������һ��·�ϣ����ǿ��Խ�����Ϊ���ǵľ������ֱ�߾���         */
		/* myIntersection�������������ж��������Ƿ���ͬһ��·��                         */
		if (myIntersection(index_node_to_road[beginPoint], index_node_to_road[it->first])) {
			dist[it->first] = distance(a_data->Nodes()[beginPoint].x, a_data->Nodes()[beginPoint].y,
				a_data->Nodes()[it->first].x, a_data->Nodes()[it->first].y);
			everyPath[it->first].push_back(beginPoint);
			everyPath[it->first].push_back(it->first);
		}
		else {
			dist[it->first] = std::numeric_limits<float>::max();
		}
	}
	dist[beginPoint] = 0;
	int num = 1;
	while (num < index_node_to_road.size()) {
		int k = beginPoint;
		float minTem = std::numeric_limits<float>::max();
		// Ѱ�Ҿ�����С��k��
		for (auto it = index_node_to_road.begin(); it != index_node_to_road.end(); it++) {
			if (dist[it->first] != 0 && dist[it->first] < minTem) {
				k = it->first;
				minTem = dist[k];
			}
		}
		num++;
		// ����dist
		for (auto it = index_node_to_road.begin(); it != index_node_to_road.end(); it++) {
			if (dist[it->first] == 0)continue;
			float disTem = 0;
			if (myIntersection(index_node_to_road[k], index_node_to_road[it->first])) {
				disTem = distance(a_data->Nodes()[k].x, a_data->Nodes()[k].y,
					a_data->Nodes()[it->first].x, a_data->Nodes()[it->first].y);
			}
			else continue;
			if (dist[it->first] > dist[k] + disTem) {
				dist[it->first] = dist[k] + disTem;
				everyPath[it->first] = everyPath[k];
				everyPath[it->first].push_back(it->first);
			}
		}
		if (k == endPoint)break;
		dist[k] = 0;
	}
	// �����ת�Ƶ�path�ϣ������ͼ
	for (int index:everyPath[endPoint]) {
		path.push_back(a_data->Nodes()[index]);
	}
}

/*************** �Զ����㷨ʵ�� *****************/
/*
void yourAlgorithm(){

}
*/
/* �Ѵ���õĵ����ݵĽ�����鿴�ĵ�,Ҫ��ȷ���ҳ� */
/* ���·��������Ļ����ʾ����,Ҫ�����Ѹ��������� */
/* ��������㷨������,�ҳ���ʼ�㵽�յ�·�������� */
/* �������,����algorithm.h�ж����vector<Node>  */
/* path�С��� �Ѹ��������� -> ����㷨 -> path   */
/*************************************************/

