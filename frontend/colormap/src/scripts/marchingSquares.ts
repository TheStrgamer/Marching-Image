// Will turn a matrix of 0s and 1s into a 3d model by running
// a customized verion of marching squares

const corners: number[][] = [
    [0,0],
    [0,1],
    [1,1],
    [1,0]
]

const edges: number[][][] = [
  [],
  [[3,0]],
  [[0,1]],
  [[3,1]],
  [[1,2]],
  [[3,0],[1,2]],
  [[0,2]],
  [[3,2]],
  [[2,3]],
  [[0,2]],
  [[1,2],[0,3]],
  [[1,3]],
  [[1,3]],
  [[0,1]],
  [[3,0]],
  []
];

const vertLookup: number[][][] = [
  [],//0000
  [[0,0],[1,0],[0,1]],//0001
  [[2,0],[1,0],[2,1]],//0010
  [[0,0],[0,1],[2,0],[2,1]],//0011
  [[2,2],[2,1],[1,2]],//0100
  [[0,1],[1,0],[1,2],[2,1]],//0101
  [[1,0],[1,2],[2,0],[2,2]],//0110
  [[0,0],[0,1],[2,0],[2,2],[1,2]],//0111
  [[0,2],[0,1],[1,2]],//1000
  [[0,0],[1,0],[0,2],[1,2]],//1001
  [[1,2],[0,1],[2,1],[1,0]],//1010
  [[2,1],[0,0],[2,0],[0,2]],//1011
  [[0,2],[2,2],[2,1],[0,1]],//1100
  [[0,2],[2,2],[0,0],[1,0],[2,1]],//1101
  [[0,1],[0,2],[1,0],[2,0],[2,2]],//1110
  [[0,0],[0,2],[2,0],[2,2]]//1111
];

const topFaceLookup: number[][][] = [
  [], // 0000: empty
  [[0, 2, 1]],// 0001
  [[0, 2, 1]],// 0010
  [[0, 1, 3], [0, 3, 2]],  // 0011
  [[0, 2, 1]],  // 0100
  [[0, 1, 3], [1, 2, 3]],  // 0101
  [[0, 1, 3], [0, 3, 2]],  // 0110
  [[0, 1, 4], [0, 4, 2], [2,4,3]],  // 0111
  [[0, 2, 1]],  // 1000
  [[0, 1, 3], [0, 3, 2]],  // 1001
  [[0, 1, 3], [0, 3, 2]],  // 1010
  [[0, 1, 4], [0, 4, 3], [3,4,2]],  // 1011
  [[0, 1, 3], [0, 3, 2]],  // 1100
  [[0, 1, 4], [0, 4, 2], [2,4,3]],  // 1101
  [[0, 1, 3], [1,4,3], [1,2,4]],  // 1110
  [[0, 1, 3], [0, 3, 2]]  // 1111
];


//litterally just gives reverse result of previous
const faceLookupBottom: number[][][] = topFaceLookup.map(triangles =>
  triangles.map(tri => [tri[0], tri[2], tri[1]])
);

const sideFaceLookup: number[][][] = [
  [], // 0000: empty - no side faces
  [[1,0,0],[0,1,0],[1,0,1], [1,0,1],[0,1,0],[0,1,1]], // 0001 - 2 faces
  [[1,0,0],[2,1,0],[1,0,1], [1,0,1],[2,1,0],[2,1,1]], // 0010 - 2 faces
  [[0,1,0],[0,1,1],[2,1,1], [2,1,1],[2,1,0],[0,1,0]], // 0011 - 2 faces
  [[2,1,2],[1,2,2],[2,1,1], [2,1,1],[1,2,2],[1,2,1]], // 0100 - 2 faces
  [[1,0,0],[0,1,1],[1,0,1], [1,0,1],[0,1,1],[0,1,0], [0,1,1],[1,2,2],[1,2,1], [1,2,1],[2,1,1],[0,1,1]], // 0101 - diagonal (4 faces)
  [[1,0,0],[1,2,2],[2,0,0], [2,0,0],[1,2,2],[2,0,2]], // 0110 - 2 faces
  [[0,1,0],[0,1,1],[2,0,2], [0,1,0],[2,0,2],[2,0,0]], // 0111 - 2 faces
  [[0,0,2],[1,2,2],[0,0,1], [0,0,1],[1,2,2],[1,2,1]], // 1000 - 2 faces
  [[0,0,0],[1,0,0],[0,0,2], [0,0,2],[1,0,0],[1,2,2]], // 1001 - 2 faces
  [[1,2,2],[0,1,1],[1,0,0], [1,0,0],[0,1,1],[0,1,0], [0,1,1],[2,1,1],[2,1,0], [2,1,0],[1,0,0],[0,1,1]], // 1010 - diagonal (4 faces)
  [[2,1,1],[0,0,0],[0,0,2], [2,1,1],[0,0,2],[2,0,0]], // 1011 - 2 faces
  [[0,0,2],[2,0,2],[0,1,1], [0,0,2],[0,1,1],[2,1,1]], // 1100 - 2 faces
  [[0,0,2],[2,0,2],[0,0,0], [0,0,0],[2,0,2],[2,1,1]], // 1101 - 2 faces
  [[0,1,1],[0,0,2],[2,0,0], [0,1,1],[2,0,0],[2,0,2]], // 1110 - 2 faces
  []  // 1111: all solid - no side faces
];


class Verticie {
    pos: number[];
    // constructor(pos: number[]) {
    //     this.pos = pos;
    // }
    constructor(x: number, y: number, z: number) {
        this.pos = [x,y,z];
    }
}

class Face {
    verts: Verticie[];
    constructor(verts:Verticie[]) {
        this.verts = verts;
    }
}

class marchingSquares {
    verticies: (Verticie | null)[][][];
    faces: Face[];

    constructor(matrix:number[][], width:number, height:number) {
        this.verticies = Array.from({ length: height*2+1 }, () =>
            Array.from({ length: width*2+1 }, () =>
                Array(2).fill(null) as (Verticie | null)[]
            )
        );
        this.vertsFromMatrix(matrix, width, height);
        this.faces = [];
    }

    vertsFromMatrix(matrix:number[][], width:number, height:number): void {
        for (let i = 0; i<=height; i++) {
            for (let j = 0; j<=width; j++) {
                // if (matrix[i][i] == 1) {
                //     this.verticies[i][j][0] = new Verticie(i,j,0);
                //     this.verticies[i][j][1] = new Verticie(i,j,1);
                // }
            }
        } 
    }
    
    addVertsFromSquare(matrix:number[][],startX:number,startY:number) {
        let index = +(matrix[startX][startY] === 1);
        index += +(matrix[startX + 1][startY] === 1) * 2;
        index += +(matrix[startX + 1][startY + 1] === 1) * 4;
        index += +(matrix[startX][startY + 1] === 1) * 8;
        let length = vertLookup[index].length;
        for (let i = 0; i<length;i++) {
            let x = startX+vertLookup[index][i][0];
            let y = startY+vertLookup[index][i][1];
            if (!this.verticies[x][y][0]) {
                this.verticies[x][y][0] = new Verticie(x,y,0);
                this.verticies[x][y][1] = new Verticie(x,y,1);
            }

        }
    }

    marchSquare(matrix:number[][],startX:number,startY:number,startZ:number) {
        //Todo lookup edges based on verticies in square

    }
}