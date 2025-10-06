// Will turn a matrix of 0s and 1s into a 3d model by running
// a customized verion of marching squares
import {Verticie, Face, vertLookup, bottomFaceLookup, topFaceLookup, sideFaceLookup} from "./lookups.ts";

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
                this.addVertsFromSquare(matrix,j,i)
            }
        } 
    }

    indexFromMatrix(matrix:number[][],startX:number,startY:number):number {
        let index = +(matrix[startX][startY] === 1);
        index += +(matrix[startX + 1][startY] === 1) * 2;
        index += +(matrix[startX + 1][startY + 1] === 1) * 4;
        index += +(matrix[startX][startY + 1] === 1) * 8;
        return index;
    }
    
    addVertsFromSquare(matrix:number[][],startX:number,startY:number) {
        let index = this.indexFromMatrix(matrix,startX,startY);
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

    marchSquare(matrix:number[][],startX:number,startY:number) {
        //Todo lookup edges based on verticies in square
        let index = this.indexFromMatrix(matrix,startX,startY);
        let topOffsets = topFaceLookup[index];
        for (let i = 0; i<topOffsets.length; i++) {
            let offsets = topOffsets[i];
            let vl = vertLookup[index];
            let verts = offsets.map(offset => {
                const [dx, dy] = vl[offset];
                return this.verticies[startX + dx][startY + dy][1]!;
            });
            this.faces.push(new Face(verts));
        }
        let bottomOffsets = bottomFaceLookup[index];
        for (let i = 0; i<bottomOffsets.length; i++) {
            let offsets = bottomOffsets[i];
            let vl = vertLookup[index];
            let verts = offsets.map(offset => {
                const [dx, dy] = vl[offset];
                return this.verticies[startX + dx][startY + dy][0]!;
            });
            this.faces.push(new Face(verts));
        }

        let sideOffsets = sideFaceLookup[index];
        for (let i = 0; i<sideOffsets.length; i+=3) {
            let offsets = sideOffsets[i];
            let verts = offsets.map(offset => {
                const [dx, dy, dz] = offsets;
                return this.verticies[startX + dx][startY + dy][dz]!;
            });
            this.faces.push(new Face(verts));
        }
    }

    marchingSquares(matrix:number[][],startX:number,startY:number) {
        for (let i = 0; i < matrix.length; i++) {
            for (let j = 0; j< matrix[0].length; j++) {
                this.marchSquare(matrix,j,i)
            }
        }
    }
}