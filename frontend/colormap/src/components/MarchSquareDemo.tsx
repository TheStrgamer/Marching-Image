import React, { useRef, useState } from "react";
import { marchingSquares } from "../scripts/marchingSquares";

function MarchSquareDemo({}) {
  const fileInputRef = useRef<HTMLInputElement | null>(null);
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const [stlData, setStlData] = useState<string>("");

  // Convert image -> matrix of 0/1
  const imageToMatrix = (img: HTMLImageElement, threshold = 240): number[][] => {
    const canvas = canvasRef.current!;
    const ctx = canvas.getContext("2d")!;
    canvas.width = img.width;
    canvas.height = img.height;
    ctx.drawImage(img, 0, 0, img.width, img.height);

    const imageData = ctx.getImageData(0, 0, img.width, img.height);
    const data = imageData.data;

    const matrix: number[][] = [];
    for (let y = 0; y < img.height; y++) {
      const row: number[] = [];
      for (let x = 0; x < img.width; x++) {
        const idx = (y * img.width + x) * 4;
        const r = data[idx];
        const g = data[idx + 1];
        const b = data[idx + 2];
        // Treat "almost white" as 0
        const value = (r > threshold && g > threshold && b > threshold) ? 0 : 1;
        row.push(value);
      }
      matrix.push(row);
    }
    return matrix;
  };

  // Handle file input
  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (!file) return;
    const img = new Image();
    img.onload = () => {
      const matrix = imageToMatrix(img);
      const ms = new marchingSquares(matrix, matrix[0].length, matrix.length);
      ms.marchingSquares(matrix);
      const stl = ms.export();
      setStlData(stl);
      triggerDownload(stl, "model.stl");
    };
    img.src = URL.createObjectURL(file);
  };

  // Trigger file download
  const triggerDownload = (data: string, filename: string) => {
    const blob = new Blob([data], { type: "application/sla" });
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");
    link.href = url;
    link.download = filename;
    link.click();
    URL.revokeObjectURL(url);
  };

  return (
    <div className="marchSquareDemo">
      <input
        type="file"
        accept="image/*"
        ref={fileInputRef}
        onChange={handleFileChange}
      />
      <canvas ref={canvasRef} style={{ display: "none" }} />
      {stlData && <p>STL file generated and downloaded!</p>}
    </div>
  );
}

export default MarchSquareDemo;
