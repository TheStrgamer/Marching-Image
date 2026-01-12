#include "Mesh.hpp"
#include <cmath>
#include <iostream>

/**
 * The constructor of the mesh class
 */
Mesh::Mesh() {
    vertices.clear();
    faces.clear();
}

/**
 * @brief Add vertex to mesh
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @return The index of the vertex
 */
int Mesh::addVertex(float x, float y, float z) {
    vertices.emplace_back(x, y, z);
    return static_cast<int>(vertices.size() - 1);
}

/**
 * @brief Add face to mesh
 * @param v1 the index of the first vertex
 * @param v2 the index of the second vertex
 * @param v3 the index of the third vertex
 */
void Mesh::addFace(int v1, int v2, int v3) {
    faces.emplace_back(v1, v2, v3);
}

/**
 * @brief Clears faces and verticies
*/
void Mesh::clear() {
    vertices.clear();
    faces.clear();
}

/**
 * @brief Computes the normals of the face based on vertex coordinates
*/
Vertex Mesh::computeNormal(const Face& f) {
    const Vertex& a = vertices[f.v1];
    const Vertex& b = vertices[f.v2];
    const Vertex& c = vertices[f.v3];

    float ux = b.x - a.x;
    float uy = b.y - a.y;
    float uz = b.z - a.z;

    float vx = c.x - a.x;
    float vy = c.y - a.y;
    float vz = c.z - a.z;

    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;

    float length = std::sqrt(nx*nx + ny*ny + nz*nz);
    if (length == 0) return Vertex(0, 0, 0);

    return Vertex(nx/length, ny/length, nz/length);
}

/**
 * @brief Exports the mesh as an stl file
 * @param filename the name of the exported file
 * @return true if saved succesfully, false otherwise
*/
bool Mesh::exportSTL(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return false;
    }

    file << "solid mesh\n";

    for (const auto& f : faces) {
        Vertex normal = computeNormal(f);
        file << "  facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n";
        file << "    outer loop\n";
        const Vertex& v1 = vertices[f.v1];
        const Vertex& v2 = vertices[f.v2];
        const Vertex& v3 = vertices[f.v3];
        file << "      vertex " << v1.x << " " << v1.y << " " << v1.z << "\n";
        file << "      vertex " << v2.x << " " << v2.y << " " << v2.z << "\n";
        file << "      vertex " << v3.x << " " << v3.y << " " << v3.z << "\n";
        file << "    endloop\n";
        file << "  endfacet\n";
    }

    file << "endsolid mesh\n";
    file.close();
    return true;
}