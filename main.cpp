#include <iostream>
#include "algorithm.h"

const int screen_width = 600;
const int screen_height = 600;

int main() {
	// OSM�ļ�·��
	std::string filePath = "map.osm";

	// ʵ����Data��,��ȡ����
	std::cout << "��[" << filePath << "]�ж�ȡ����\n";
	Data* mapData = new Data(filePath);

	// ��ȡ��������
	float x1{};
	float y1{};
	float x2{};
	float y2{};
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	std::cout << "||****************************************************|\n";
	std::cout << "||* ������Ҫ�������·���������ʼ����յ����꣺     *|\n";
	std::cout << "||* �ٷ���,���귶Χ��0��100,���½�Ϊԭ��             *|\n";
	std::cout << "||* ����Ϊx��,����Ϊy��                              *|\n";   
	std::cout << "||* ��ʽ:x1 y1 x2 y2                                 *|\n";
	std::cout << "||* ʾ����20 20 90 90                                *|\n";
	std::cout << "||****************************************************|\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "����:"; 
	std::cin >> x1 >> y1 >> x2 >> y2;
	x1 /= 100;
	y1 /= 100;
	x2 /= 100;
	y2 /= 100;

	// ʵ����Render��,ʵ�ֿ��ӻ�
	Render* render = new Render(mapData, screen_width, screen_height);
	auto display = io2d::output_surface{ render->m_Width, render->m_Height, io2d::format::argb32, 
		io2d::scaling::none, io2d::refresh_style::fixed, 30 };
	display.size_change_callback([](io2d::output_surface& surface) {
		surface.dimensions(surface.display_dimensions());
		});

	// ʵ�����㷨��
	Algorithm* algorithm = new Algorithm(render, mapData, x1, y1, x2, y2);
	// ִ���㷨
	algorithm->dijkstra();
	/*****************************************************/
	/* �ڱ�д���Լ�����ĺ���֮��,�ڴ˴���dijkstraע�͵� */
	/* ��ͨ��algorithm->yourAlgorithm();��������㷨     */
	/*****************************************************/

	// ���ӻ�
	if (algorithm->path.empty()) {
		std::cout << "pathΪ��!�Ҳ���·�����Ե���!\n";
	}
	display.draw_callback([&](io2d::output_surface& surface) {
		render->drawRawMap(surface);
		render->DrawBEPoint(surface, algorithm->beginPoint, algorithm->endPoint);
		render->DrawPath(surface, algorithm->path);
		});
	display.begin_show();
	
	return 0;
}
