#include <iostream>
#include "algorithm.h"

const int screen_width = 600;
const int screen_height = 600;

int main() {
	// OSM文件路径
	std::string filePath = "map.osm";

	// 实例化Data类,读取数据
	std::cout << "从[" << filePath << "]中读取数据\n";
	Data* mapData = new Data(filePath);

	// 获取两点坐标
	float x1{};
	float y1{};
	float x2{};
	float y2{};
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	std::cout << "||****************************************************|\n";
	std::cout << "||* 请输入要进行最短路径计算的起始点和终点坐标：     *|\n";
	std::cout << "||* 百分制,坐标范围从0到100,左下角为原点             *|\n";
	std::cout << "||* 横轴为x轴,纵轴为y轴                              *|\n";   
	std::cout << "||* 格式:x1 y1 x2 y2                                 *|\n";
	std::cout << "||* 示例：20 20 90 90                                *|\n";
	std::cout << "||****************************************************|\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "坐标:"; 
	std::cin >> x1 >> y1 >> x2 >> y2;
	x1 /= 100;
	y1 /= 100;
	x2 /= 100;
	y2 /= 100;

	// 实例化Render类,实现可视化
	Render* render = new Render(mapData, screen_width, screen_height);
	auto display = io2d::output_surface{ render->m_Width, render->m_Height, io2d::format::argb32, 
		io2d::scaling::none, io2d::refresh_style::fixed, 30 };
	display.size_change_callback([](io2d::output_surface& surface) {
		surface.dimensions(surface.display_dimensions());
		});

	// 实例化算法类
	Algorithm* algorithm = new Algorithm(render, mapData, x1, y1, x2, y2);
	// 执行算法
	algorithm->dijkstra();
	/*****************************************************/
	/* 在编写好自己定义的函数之后,在此处将dijkstra注释掉 */
	/* 且通过algorithm->yourAlgorithm();调用你的算法     */
	/*****************************************************/

	// 可视化
	if (algorithm->path.empty()) {
		std::cout << "path为空!找不到路径可以到达!\n";
	}
	display.draw_callback([&](io2d::output_surface& surface) {
		render->drawRawMap(surface);
		render->DrawBEPoint(surface, algorithm->beginPoint, algorithm->endPoint);
		render->DrawPath(surface, algorithm->path);
		});
	display.begin_show();
	
	return 0;
}
