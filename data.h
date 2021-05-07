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
    // ���ļ��ж�ȡ�����ݷ���osm_data��
	std::vector<std::byte> osm_data;
    // ��������
    void adjustCoordinates();

public:
    /* ����������ݽṹ */

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
    /* ��ͼ���� */
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
    // ���캯��
	Data(std::string filePath);
    // ���ļ��ж�ȡ����(�ɹ��캯������)
	std::optional<std::vector<std::byte>> readFile(const std::string& path);
    // ����ȡ�������ݽ�������(�ɹ��캯������)
    void loadData();
};
