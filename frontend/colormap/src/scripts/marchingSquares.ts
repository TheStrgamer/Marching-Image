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
        this.verticies = Array.from({ length: height }, () =>
            Array.from({ length: width }, () =>
                Array(2).fill(null) as (Verticie | null)[]
            )
        );
        this.vertsFromMatrix(matrix, width, height);
        this.faces = [];
    }

    vertsFromMatrix(matrix:number[][], width:number, height:number): void {
        for (let i = 0; i<=height; i++) {
            for (let j = 0; j<=width; j++) {
                if (matrix[i][i] == 1) {
                    this.verticies[i][j][0] = new Verticie(i,j,0);
                    this.verticies[i][j][1] = new Verticie(i,j,1);
                }
            }
        } 
    }

    marchSquare(startX:number,startY:number,startZ:number) {
        //Todo lookup edges based on verticies in square
    }
}