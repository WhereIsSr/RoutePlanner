#include "render.h"

Render::Render(Data* mapData, int width, int height):m_Data(mapData), m_Width(width), m_Height(height) {
    BuildRoadReps();
    BuildLanduseBrushes();
}

void Render::BuildRoadReps()
{
    using R = Data::Road;
    auto types = { R::Motorway, R::Trunk, R::Primary,  R::Secondary, R::Tertiary,
        R::Residential, R::Service, R::Unclassified,};
    for (auto type : types) {
        auto& rep = m_RoadReps[type];
        rep.brush = io2d::brush{ RoadColor(type) };
        rep.metric_width = RoadMetricWidth(type);
        rep.dashes = io2d::dashes{};
    }
}

io2d::rgba_color Render::RoadColor(Data::Road::Type type)
{
    switch (type) {
    case Data::Road::Motorway:     return io2d::rgba_color{ 226, 122, 143 };
    case Data::Road::Trunk:        return io2d::rgba_color{ 245, 161, 136 };
    case Data::Road::Primary:      return io2d::rgba_color{ 249, 207, 144 };
    case Data::Road::Secondary:    return io2d::rgba_color{ 244, 251, 173 };
    case Data::Road::Tertiary:     return io2d::rgba_color{ 244, 251, 173 };
    case Data::Road::Residential:  return io2d::rgba_color{ 254, 254, 254 };
    case Data::Road::Service:      return io2d::rgba_color{ 254, 254, 254 };
    case Data::Road::Unclassified: return io2d::rgba_color{ 254, 254, 254 };
    default:                        return io2d::rgba_color::grey;
    }
}

float Render::RoadMetricWidth(Data::Road::Type type)
{
    switch (type) {
    case Data::Road::Motorway:     return 6.f;
    case Data::Road::Trunk:        return 6.f;
    case Data::Road::Primary:      return 5.f;
    case Data::Road::Secondary:    return 5.f;
    case Data::Road::Tertiary:     return 4.f;
    case Data::Road::Residential:  return 2.5f;
    case Data::Road::Unclassified: return 2.5f;
    case Data::Road::Service:      return 1.f;
    default:                        return 1.f;
    }
}

io2d::point_2d Render::ToPoint2D(const Data::Node& node) noexcept
{
    return io2d::point_2d(static_cast<float>(node.x), static_cast<float>(node.y));
}

void Render::BuildLanduseBrushes()
{
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Commercial, io2d::brush{ io2d::rgba_color{233, 195, 196} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Construction, io2d::brush{ io2d::rgba_color{187, 188, 165} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Grass, io2d::brush{ io2d::rgba_color{197, 236, 148} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Forest, io2d::brush{ io2d::rgba_color{158, 201, 141} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Industrial, io2d::brush{ io2d::rgba_color{223, 197, 220} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Railway, io2d::brush{ io2d::rgba_color{223, 197, 220} });
    m_LanduseBrushes.insert_or_assign(Data::Landuse::Residential, io2d::brush{ io2d::rgba_color{209, 209, 209} });
}

void Render::drawRawMap(io2d::output_surface& surface) {
    m_Scale = static_cast<float>(std::min(surface.dimensions().x(), surface.dimensions().y()));
    m_PixelsInMeter = static_cast<float>(m_Scale / m_Data->MetricScale());
    m_Matrix = io2d::matrix_2d::create_scale({ m_Scale, -m_Scale }) *
        io2d::matrix_2d::create_translate({ 0.f, static_cast<float>(surface.dimensions().y()) });

    surface.paint(m_BackgroundFillBrush);
    DrawLanduses(surface);
    DrawLeisure(surface);
    DrawWater(surface);
    DrawRailways(surface);
    DrawHighways(surface);
    DrawBuildings(surface);
}

void Render::DrawBuildings(io2d::output_surface& surface)
{
    for (auto& building : m_Data->Buildings()) {
        auto path = PathFromMP(building);
        surface.fill(m_BuildingFillBrush, path);
        surface.stroke(m_BuildingOutlineBrush, path, std::nullopt, m_BuildingOutlineStrokeProps);
    }
}

void Render::DrawLeisure(io2d::output_surface& surface)
{
    for (auto& leisure : m_Data->Leisures()) {
        auto path = PathFromMP(leisure);
        surface.fill(m_LeisureFillBrush, path);
        surface.stroke(m_LeisureOutlineBrush, path, std::nullopt, m_LeisureOutlineStrokeProps);
    }
}

void Render::DrawWater(io2d::output_surface& surface)
{
    for (auto& water : m_Data->Waters())
        surface.fill(m_WaterFillBrush, PathFromMP(water));
}

void Render::DrawLanduses(io2d::output_surface& surface)
{
    for (auto& landuse : m_Data->Landuses())
        if (auto br = m_LanduseBrushes.find(landuse.type); br != m_LanduseBrushes.end())
            surface.fill(br->second, PathFromMP(landuse));
}

void Render::DrawHighways(io2d::output_surface& surface)
{
    auto ways = m_Data->Ways().data();
    for (auto road : m_Data->Roads())
        if (auto rep_it = m_RoadReps.find(road.type); rep_it != m_RoadReps.end()) {
            auto& rep = rep_it->second;
            auto& way = ways[road.way];
            auto width = rep.metric_width > 0.f ? (rep.metric_width * m_PixelsInMeter) : 1.f;
            auto sp = io2d::stroke_props{ width, io2d::line_cap::round };
            surface.stroke(rep.brush, PathFromWay(way), std::nullopt, sp, rep.dashes);
        }
}

void Render::DrawRailways(io2d::output_surface& surface)
{
    auto ways = m_Data->Ways().data();
    for (auto& railway : m_Data->Railways()) {
        auto& way = ways[railway.way];
        auto path = PathFromWay(way);
        surface.stroke(m_RailwayStrokeBrush, path, std::nullopt, io2d::stroke_props{ m_RailwayOuterWidth * m_PixelsInMeter });
        surface.stroke(m_RailwayDashBrush, path, std::nullopt, io2d::stroke_props{ m_RailwayInnerWidth * m_PixelsInMeter }, m_RailwayDashes);
    }
}

io2d::interpreted_path Render::PathFromWay(const Data::Way& way)
{
    if (way.nodes.empty())
        return {};

    const auto nodes = m_Data->Nodes().data();

    auto pb = io2d::path_builder{};
    pb.matrix(m_Matrix);
    pb.new_figure(ToPoint2D(nodes[way.nodes.front()]));
    for (auto it = ++way.nodes.begin(); it != std::end(way.nodes); ++it)
        pb.line(ToPoint2D(nodes[*it]));
    return io2d::interpreted_path{ pb };
}

io2d::interpreted_path Render::PathFromMP(const Data::Multipolygon& mp)
{
    const auto nodes = m_Data->Nodes().data();
    const auto ways = m_Data->Ways().data();

    auto pb = io2d::path_builder{};
    pb.matrix(m_Matrix);

    auto commit = [&](const Data::Way& way) {
        if (way.nodes.empty())
            return;
        pb.new_figure(ToPoint2D(nodes[way.nodes.front()]));
        for (auto it = ++way.nodes.begin(); it != std::end(way.nodes); ++it)
            pb.line(ToPoint2D(nodes[*it]));
        pb.close_figure();
    };

    for (auto way_num : mp.outer)
        commit(ways[way_num]);
    for (auto way_num : mp.inner)
        commit(ways[way_num]);

    return io2d::interpreted_path{ pb };
}

void Render::DrawBEPoint(io2d::output_surface& surface, int beginPoint, int endPoint) {
    io2d::render_props aliased{ io2d::antialias::none };

    io2d::brush beginBrush{ io2d::rgba_color::green };
    io2d::brush endBrush{ io2d::rgba_color::red };

    float constexpr l_marker = 0.01f;

    // 起始点
    auto pb_begin = io2d::path_builder{};
    pb_begin.matrix(m_Matrix);

    pb_begin.new_figure({ (float)m_Data->Nodes()[beginPoint].x, (float)m_Data->Nodes()[beginPoint].y });
    pb_begin.rel_line({ l_marker, 0.f });
    pb_begin.rel_line({ 0.f, l_marker });
    pb_begin.rel_line({ -l_marker, 0.f });
    pb_begin.rel_line({ 0.f, -l_marker });
    pb_begin.close_figure();

    surface.fill(beginBrush, pb_begin);
    surface.stroke(beginBrush, io2d::interpreted_path{ pb_begin }, std::nullopt, std::nullopt, std::nullopt, aliased);

    //终点
    auto pb_end = io2d::path_builder{};
    pb_end.matrix(m_Matrix);

    pb_end.new_figure({ (float)m_Data->Nodes()[endPoint].x, (float)m_Data->Nodes()[endPoint].y });
    pb_end.rel_line({ l_marker, 0.f });
    pb_end.rel_line({ 0.f, l_marker });
    pb_end.rel_line({ -l_marker, 0.f });
    pb_end.rel_line({ 0.f, -l_marker });
    pb_end.close_figure();

    surface.fill(endBrush, pb_end);
    surface.stroke(endBrush, io2d::interpreted_path{ pb_end }, std::nullopt, std::nullopt, std::nullopt, aliased);
}

void Render::DrawPath(io2d::output_surface& surface, std::vector<Data::Node> path) {
    if (path.empty()) {
        return;
    }
    io2d::render_props aliased{ io2d::antialias::none };
    io2d::brush pathBrush{ io2d::rgba_color::orange };
    float width = 5.0f;
    surface.stroke(pathBrush, PathLine(path), std::nullopt, io2d::stroke_props{ width });
}

io2d::interpreted_path Render::PathLine(std::vector<Data::Node> path)
{
    auto pb = io2d::path_builder{};
    pb.matrix(m_Matrix);
    pb.new_figure(ToPoint2D(path[0]));
    for (int i = 1; i < path.size(); i++)
        pb.line(ToPoint2D(path[i]));
    return io2d::interpreted_path{ pb };
}