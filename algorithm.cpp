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
	// 将起始点加入路径中
	std::unordered_map<int, std::vector<int>> everyPath;
	everyPath[beginPoint].push_back(beginPoint);
	// dist  key为该点的索引,value为该点到起始点的距离
	std::unordered_map<int, float> dist;
	// 初始化dist
	for (auto it = index_node_to_road.begin(); it != index_node_to_road.end(); it++) {
		/* 此处要特别注意, 两个点若不在一条路上，那么他们的距离不能以直线距离定，应设为 */
		/* 无穷大，只有两个点在一条路上，我们可以近似认为他们的距离等于直线距离         */
		/* myIntersection函数就是用来判断两个点是否在同一条路上                         */
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
		// 寻找距离最小的k点
		for (auto it = index_node_to_road.begin(); it != index_node_to_road.end(); it++) {
			if (dist[it->first] != 0 && dist[it->first] < minTem) {
				k = it->first;
				minTem = dist[k];
			}
		}
		num++;
		// 调整dist
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
	// 将结果转移到path上，方便绘图
	for (int index:everyPath[endPoint]) {
		path.push_back(a_data->Nodes()[index]);
	}
}

/*************** 自定义算法实现 *****************/
/*
void yourAlgorithm(){

}
*/
/* 已处理好的的数据的解释请查看文档,要正确查找出 */
/* 最短路径并在屏幕上显示出来,要利用已给出的数据 */
/* 经过你的算法的运算,找出起始点到终点路径经过的 */
/* 点的索引,放在algorithm.h中定义的vector<Node>  */
/* path中。即 已给出的数据 -> 你的算法 -> path   */
/*************************************************/

