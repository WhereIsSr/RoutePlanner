#pragma once
#include <io2d.h>
#include "data.h"
#include <Windows.h>

using namespace std::experimental;

class Render {
public:
    int m_Width, m_Height;
	Render(Data* mapData, int width, int height);
    void drawRawMap(io2d::output_surface& surface);
private:
	Data* m_Data;

    // 设置每一种区域的绘制风格
    std::unordered_map<Data::Landuse::Type, io2d::brush> m_LanduseBrushes;
    void BuildLanduseBrushes();

    // 设置每一种道路的绘制的风格
    struct RoadRep {
        io2d::brush brush{ io2d::rgba_color::black };
        io2d::dashes dashes{};
        float metric_width = 1.f;
    };
    std::unordered_map<Data::Road::Type, RoadRep> m_RoadReps;
    void BuildRoadReps();
    io2d::rgba_color RoadColor(Data::Road::Type type);
    float RoadMetricWidth(Data::Road::Type type);
    io2d::dashes RoadDashes(Data::Road::Type type);

    // 绘制参数
    float m_Scale = 1.f;
    float m_PixelsInMeter = 1.f;
    io2d::matrix_2d m_Matrix;

    io2d::brush m_BackgroundFillBrush{ io2d::rgba_color{238, 235, 227} };

    io2d::brush m_BuildingFillBrush{ io2d::rgba_color{208, 197, 190} };
    io2d::brush m_BuildingOutlineBrush{ io2d::rgba_color{181, 167, 154} };
    io2d::stroke_props m_BuildingOutlineStrokeProps{ 1.f };

    io2d::brush m_LeisureFillBrush{ io2d::rgba_color{189, 252, 193} };
    io2d::brush m_LeisureOutlineBrush{ io2d::rgba_color{160, 248, 162} };
    io2d::stroke_props m_LeisureOutlineStrokeProps{ 1.f };

    io2d::brush m_WaterFillBrush{ io2d::rgba_color{155, 201, 215} };

    io2d::brush m_RailwayStrokeBrush{ io2d::rgba_color{93,93,93} };
    io2d::brush m_RailwayDashBrush{ io2d::rgba_color::white };
    io2d::dashes m_RailwayDashes{ 0.f, {3.f, 3.f} };
    float m_RailwayOuterWidth = 3.f;
    float m_RailwayInnerWidth = 2.f;

public:
    // 绘制函数
    void DrawBuildings(io2d::output_surface& surface);
    void DrawHighways(io2d::output_surface& surface);
    void DrawRailways(io2d::output_surface& surface);
    void DrawLeisure(io2d::output_surface& surface);
    void DrawWater(io2d::output_surface& surface);
    void DrawLanduses(io2d::output_surface& surface);
    void DrawBEPoint(io2d::output_surface& surface, int beginPoint, int endPoint);
    void DrawPath(io2d::output_surface& surface, std::vector<Data::Node> path);
    io2d::interpreted_path PathLine(std::vector<Data::Node> path);
    io2d::interpreted_path PathFromWay(const Data::Way& way);
    io2d::interpreted_path PathFromMP(const Data::Multipolygon& mp);
    io2d::point_2d ToPoint2D(const Data::Node& node) noexcept;
};