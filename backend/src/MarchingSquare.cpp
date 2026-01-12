#include "../header/MarchingSquare.hpp"


MarchingSquare::MarchingSquare(Matrix matrix, int w, int h) {
    m = matrix;
    width = w;
    height = h;
    vertsFromMatrix();
}

int MarchingSquare::indexFromMatrix(int startX, int startY){
    int index = m[startY][startX] == 1;
    index += (m[startY][startX+1] == 1)*2;
    index += (m[startY+1][startX+1] == 1)*4;
    index += (m[startY+1][startX] == 1)*8;
    return index;
}

void MarchingSquare::addVertsFromSquare(int startX, int startY, float size){
    const int vx = startX * 2;
    const int vy = startY * 2;
    const int index = indexFromMatrix(startX, startY);
    const int l = vertLookup[index].size();

    for (int i = 0; i < l; i++) {
        const Vert2 d = vertLookup[index][i];
        const int x = vx + d[0];
        const int y = vy + d[1];

        if (vertRef[x][y][0] == -1) {
            vertRef[x][y][0] = mesh.addVertex(x - width + 1, y - height + 1, -size/2);
            vertRef[x][y][1] = mesh.addVertex(x - width + 1, y - height + 1, size/2);
        }
    }
}

void MarchingSquare::vertsFromMatrix(){
    float size = (sqrt(width*height))/5;
    int gridWidth  = width * 2;
    int gridHeight = height * 2;   

    vector<vector<vector<int>>> Ref(
        gridHeight,
        vector<vector<int>>(
            gridWidth,
            vector<int>(2, -1)
        )
    ); 
    vertRef = Ref;

    for (int i = 0; i<height-1; i++) {
        for (int j = 0; j<width-1; j++) {
            addVertsFromSquare(j,i, size);
        }
    } 
}

void MarchingSquare::marchSquare(int startX, int startY){
    int index = indexFromMatrix(startX,startY);
    const int baseX = startX * 2;
    const int baseY = startY * 2;

    const auto& vl = vertLookup[index];

    for (const Tri& offsets : topFaceLookup[index]) {
        int v[3];

        for (int i = 0; i < 3; i++) {
            const auto& [dx, dy] = vl[offsets[i]];
            const int x = baseX + dx;
            const int y = baseY + dy;
            const int z = 1;

            int& ref = vertRef[y][x][z];
            if (ref == -1) {
                float size = (sqrt(width*height))/5;
                ref = mesh.addVertex(
                    x - width + 1,
                    y - height + 1,
                    +size * 0.5f
                );
            }
            v[i] = ref;
        }

        mesh.addFace(v[0], v[1], v[2]);
    }

    for (const Tri& offsets : bottomFaceLookup[index]) {
        int v[3];

        for (int i = 0; i < 3; i++) {
            const auto& [dx, dy] = vl[offsets[i]];
            const int x = baseX + dx;
            const int y = baseY + dy;
            const int z = 0;

            int& ref = vertRef[y][x][z];
            if (ref == -1) {
                float size = (sqrt(width*height))/5;
                ref = mesh.addVertex(
                    x - width + 1,
                    y - height + 1,
                    -size * 0.5f
                );
            }
            v[i] = ref;
        }

        mesh.addFace(v[0], v[1], v[2]);
    }

    for (const auto& tri : sideFaceLookup[index]) {
        int v[3];

        for (int i = 0; i < 3; i++) {
            const auto& [dx, dy, dz] = tri[i];
            const int x = baseX + dx;
            const int y = baseY + dy;
            const int z = dz;

            int& ref = vertRef[y][x][z];
            if (ref == -1) {
                float size = (sqrt(width*height))/5;
                ref = mesh.addVertex(
                    x - width + 1,
                    y - height + 1,
                    dz ? +size * 0.5f : -size * 0.5f
                );
            }
            v[i] = ref;
        }

        mesh.addFace(v[0], v[1], v[2]);
    }
}

void MarchingSquare::marchSquares() {
    for (int i = 0; i < m.size()-1; i++) {
        for (int j = 0; i < m[0].size()-1; j++) {
            marchSquare(j,i);
        }
    }

}

void MarchingSquare::exportMesh(string &filename){
    mesh.exportSTL(filename);

}