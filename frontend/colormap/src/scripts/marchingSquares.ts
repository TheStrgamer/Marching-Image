// Will turn a matrix of 0s and 1s into a 3d model by running
// a customized verion of marching squares
import {Verticie, Face, vertLookup, bottomFaceLookup, topFaceLookup, sideFaceLookup} from "./lookups.ts";
import { facesToSTL } from "./stlCreator.ts";

export class marchingSquares {
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
        let size = Math.sqrt(width*height)/5;
        for (let i = 0; i<height-1; i++) {
            for (let j = 0; j<width-1; j++) {
                this.addVertsFromSquare(matrix,j,i, size)
            }
        } 
    }

    indexFromMatrix(matrix:number[][],startX:number,startY:number):number {
        let index = +(matrix[startY][startX] === 1);
        index += +(matrix[startY][startX + 1] === 1) * 2;
        index += +(matrix[startY + 1][startX + 1] === 1) * 4;
        index += +(matrix[startY + 1][startX] === 1) * 8;
        return index;
    }
    
    addVertsFromSquare(matrix: number[][], startX: number, startY: number, size: number) {
    const vx = startX * 2;
    const vy = startY * 2;
    const index = this.indexFromMatrix(matrix, startX, startY);

    const length = vertLookup[index].length;
    for (let i = 0; i < length; i++) {
        const [dx, dy] = vertLookup[index][i];
        const x = vx + dx;
        const y = vy + dy;
        if (!this.verticies[y][x][0]) {
            this.verticies[y][x][0] = new Verticie(x, y, 0);
            this.verticies[y][x][1] = new Verticie(x, y, size);
        }
    }
}


    marchSquare(matrix:number[][],startX:number,startY:number) {
        let index = this.indexFromMatrix(matrix,startX,startY);
        let topOffsets = topFaceLookup[index];
        for (let i = 0; i<topOffsets.length; i++) {
            try{
            let offsets = topOffsets[i];
            let vl = vertLookup[index];
            let verts = offsets.map(offset => {
                const [dx, dy] = vl[offset];
                if (dx > 2 || dy > 2) {
                    console.log("dx: " + dx + "dy: "+ dy);
                }
                return this.verticies[startY*2 + dy][startX*2 + dx][1]!;
            });
            this.faces.push(new Face(verts));
            } catch {
                console.log("topOffsets: " + topOffsets);
                console.log("index: " + index);
                console.log("i: " + i);
                console.log("offsets: " + topOffsets[i]);
                console.log("vl: " + vertLookup[index]);

            }
        }
        let bottomOffsets = bottomFaceLookup[index];
        for (let i = 0; i<bottomOffsets.length; i++) {
            let offsets = bottomOffsets[i];
            let vl = vertLookup[index];
            let verts = offsets.map(offset => {
                const [dx, dy] = vl[offset];
                return this.verticies[startY*2 + dy][startX*2 + dx][0]!;
            });
            this.faces.push(new Face(verts));
        }

        let sideOffsets = sideFaceLookup[index];
        for (let triIndex = 0; triIndex < sideOffsets.length; triIndex++) {
            const tri = sideOffsets[triIndex];
            let verts: Verticie[] = [];

            for (let vertIndex = 0; vertIndex < tri.length; vertIndex++) {
                const [dx, dy, dz] = tri[vertIndex];
                const x = startX*2 + dx;
                const y = startY*2 + dy;
                const z = dz;

                // Check if vert exists
                const v = this.verticies[y]?.[x]?.[z];
                if (!v) {
                    console.error(
                        `Missing vertex! triIndex: ${triIndex}, vertIndex: ${vertIndex}, index: ${index}, ` +
                        `dx: ${dx}, dy: ${dy}, dz: ${dz}, ` +
                        `x: ${x}, y: ${y}, z: ${z}`
                    );
                }

                verts.push(v!);
            }

            this.faces.push(new Face(verts));
        }

    }

    marchingSquares(matrix:number[][]) {
        for (let i = 0; i < matrix.length-1; i++) {
            for (let j = 0; j< matrix[0].length-1; j++) {
                this.marchSquare(matrix,j,i)
            }
        }
    }
    
    export() : string{
        if (this.faces.length == 0) {
            return "";
        }
        return facesToSTL(this.faces);

    }
}