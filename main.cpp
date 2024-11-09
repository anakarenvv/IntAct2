#include <iostream>
#include <vector>
#include <boost/polygon/voronoi.hpp>

using namespace boost::polygon;
using namespace std;

// Definición de un punto en el plano
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

// Convierte los puntos a la estructura de Voronoi
struct VoronoiPoint {
    int x, y;
    VoronoiPoint(int x, int y) : x(x), y(y) {}
};

// Incluir los puntos en el modelo de Voronoi
namespace boost {
    namespace polygon {
        template <>
        struct geometry_concept<VoronoiPoint> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<VoronoiPoint> {
            typedef int coordinate_type;

            static inline coordinate_type get(const VoronoiPoint& point, orientation_2d orient) {
                return (orient == HORIZONTAL) ? point.x : point.y;
            }
        };
    }
}

// Función para imprimir polígonos del diagrama de Voronoi
void printVoronoiDiagram(const voronoi_diagram<double>& vd) {
    for (auto it = vd.cells().begin(); it != vd.cells().end(); ++it) {
        const voronoi_diagram<double>::cell_type& cell = *it;
        const voronoi_diagram<double>::edge_type* edge = cell.incident_edge();
        cout << "Polygon for point: ";
        if (cell.contains_point()) {
            cout << "(" << vd.vertices()[cell.source_index()].x() << ", " 
                 << vd.vertices()[cell.source_index()].y() << ")" << endl;
        }
        do {
            if (edge->is_primary()) {
                cout << "  Edge to vertex: (" 
                     << edge->vertex0()->x() << ", " << edge->vertex0()->y() << ") -> (" 
                     << edge->vertex1()->x() << ", " << edge->vertex1()->y() << ")" << endl;
            }
            edge = edge->next();
        } while (edge != cell.incident_edge());
    }
}

// Programa principal
int main() {
    // Entrada: Lista de coordenadas
    vector<Point> points = {
        Point(200, 500),
        Point(300, 100),
        Point(450, 150),
        Point(520, 480)
    };

    // Construir puntos para el diagrama de Voronoi
    vector<VoronoiPoint> voronoiPoints;
    for (const auto& point : points) {
        voronoiPoints.push_back(VoronoiPoint(point.x, point.y));
    }

    // Construir el diagrama de Voronoi
    voronoi_diagram<double> vd;
    construct_voronoi(voronoiPoints.begin(), voronoiPoints.end(), &vd);

    // Imprimir las regiones de Voronoi
    printVoronoiDiagram(vd);

    return 0;
}
