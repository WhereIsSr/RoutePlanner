#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <unordered_map>
#include <assert.h>
#include <Algorithm>
#include "pugixml.hpp"

class Data {
private:
    // 从文件中读取的数据放在osm_data里
	std::vector<std::byte> osm_data;
    // 调整坐标
    void adjustCoordinates();

public:
    /* 点线面等数据结构 */

    struct Node {
        double x = 0.f;
        double y = 0.f;
    };

    struct Way {
        std::vector<int> nodes;
    };

    struct Road {
        enum Type {
            Invalid, Unclassified, Service, Residential,
            Tertiary, Secondary, Primary, Trunk, Motorway
        };
        int way;
        Type type;
    };
    Data::Road::Type StringToRoadType(std::string type);

    struct Railway {
        int way;
    };

    struct Multipolygon {
        std::vector<int> outer;
        std::vector<int> inner;
    };
    void BuildRings(Multipolygon& mp);
    std::vector<int> Track(std::vector<int>& open_ways, const Data::Way* ways);
    bool TrackRec(const std::vector<int>& open_ways, const Data::Way* ways,
        std::vector<bool>& used, std::vector<int>& nodes);

    struct Building : Multipolygon {};

    struct Leisure : Multipolygon {};

    struct Water : Multipolygon {};

    struct Landuse : Multipolygon {
        enum Type { Invalid, Commercial, Construction, Grass, Forest, Industrial, Railway, Residential };
        Type type;
    };
    Data::Landuse::Type StringToLanduseType(std::string type);

private:
    /* 地图数据 */
    std::vector<Node> m_Nodes;
    std::vector<Way> m_Ways;
    std::vector<Road> m_Roads;
    std::vector<Railway> m_Railways;
    std::vector<Building> m_Buildings;
    std::vector<Leisure> m_Leisures;
    std::vector<Water> m_Waters;
    std::vector<Landuse> m_Landuses;

    double m_MinLat = 0.;
    double m_MaxLat = 0.;
    double m_MinLon = 0.;
    double m_MaxLon = 0.;
    double m_MetricScale = 1.f;

public:
    auto MetricScale() const noexcept { return m_MetricScale; }

    auto& Nodes() const noexcept { return m_Nodes; }
    auto& Ways() const noexcept { return m_Ways; }
    auto& Roads() const noexcept { return m_Roads; }
    auto& Buildings() const noexcept { return m_Buildings; }
    auto& Leisures() const noexcept { return m_Leisures; }
    auto& Waters() const noexcept { return m_Waters; }
    auto& Landuses() const noexcept { return m_Landuses; }
    auto& Railways() const noexcept { return m_Railways; }

public:
    // 构造函数
	Data(std::string filePath);
    // 从文件中读取数据(由构造函数调用)
	std::optional<std::vector<std::byte>> readFile(const std::string& path);
    // 将读取到的数据解析出来(由构造函数调用)
    void loadData();
};
