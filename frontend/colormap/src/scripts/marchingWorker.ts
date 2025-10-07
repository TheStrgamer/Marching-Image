import { marchingSquares } from "./marchingSquares";

onmessage = (e) => {
  const { matrix } = e.data;
  try {
    const ms = new marchingSquares(matrix, matrix[0].length, matrix.length);
    ms.marchingSquares(matrix);
    const stl = ms.export();
    postMessage({ stl });
  } catch (err) {
    postMessage({ error: (err as Error).message });
  }
};