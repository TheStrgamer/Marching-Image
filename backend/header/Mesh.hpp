#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief A structure to represent a vertex in 3D space
 */
struct Vertex {
    float x, y, z;
    Vertex(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

/**
 * @brief A structure to represent a face in a 3D mesh
 * A face is defined by three vertices.
 */
struct Face {
    int v1, v2, v3;
    Face(int a, int b, int c) : v1(a), v2(b), v3(c) {}
};

/**
 * @brief A class to represent a 3D mesh
 * A class to represent a 3D mesh with vertices and faces. 
 * It has functions for adding vertices, adding faces, clearing the mesh, 
 * and exporting to STL format.
 */
class Mesh {
public:
    Mesh();
    int addVertex(float x, float y, float z);
    void addFace(int v1, int v2, int v3);
    void clear();
    bool exportSTL(const std::string& filename);
    std::string toString();

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<Face>& getFaces() const { return faces; }

private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    Vertex computeNormal(const Face& f);
};
