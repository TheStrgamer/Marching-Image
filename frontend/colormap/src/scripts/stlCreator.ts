// This will be responsible for taking a list of faces and verticies
// and turning it into an exportable stl mesh
import { Verticie, Face } from "./lookups";

function cross(a: number[], b: number[]): number[] {
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ];
}

function normalize(v: number[]): number[] {
    const len = Math.sqrt(v[0] ** 2 + v[1] ** 2 + v[2] ** 2);
    return len > 0 ? [v[0] / len, v[1] / len, v[2] / len] : [0, 0, 0];
}

function computeNormal(verts: Verticie[]): number[] {
    const [x1, y1, z1] = verts[0].pos;
    const [x2, y2, z2] = verts[1].pos;
    const [x3, y3, z3] = verts[2].pos;

    const u = [x2 - x1, y2 - y1, z2 - z1];
    const v = [x3 - x1, y3 - y1, z3 - z1];

    return normalize(cross(u, v));
}

export function facesToSTL(faces: Face[], solidName: string = "model"): string {
    let stl = `solid ${solidName}\n`;

    for (const face of faces) {
        if (face.verts.length !== 3) {
            throw new Error("STL requires triangular faces only.");
        }
        const normal = computeNormal(face.verts);
        stl += `  facet normal ${normal[0]} ${normal[1]} ${normal[2]}\n`;
        stl += `    outer loop\n`;
        for (const vert of face.verts) {
            stl += `      vertex ${vert.pos[0]} ${vert.pos[1]} ${vert.pos[2]}\n`;
        }
        stl += `    endloop\n`;
        stl += `  endfacet\n`;
    }

    stl += `endsolid ${solidName}\n`;
    return stl;
}