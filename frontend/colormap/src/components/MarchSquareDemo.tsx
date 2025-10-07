import React, { useRef, useState, useEffect } from "react";

function MarchSquareDemo() {
  const fileInputRef = useRef<HTMLInputElement | null>(null);
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const [stlData, setStlData] = useState<string>("");
  const [isProcessing, setIsProcessing] = useState(false);
  const workerRef = useRef<Worker | null>(null);

  useEffect(() => {
    const worker = new Worker(
      new URL("../scripts/marchingWorker.ts", import.meta.url),
      { type: "module" }
    );

    worker.onmessage = (e) => {
      const { stl, error } = e.data;
      if (error) {
        console.error(error);
        setIsProcessing(false);
        return;
      }
      setStlData(stl);
      setIsProcessing(false);
      triggerDownload(stl, "model.stl");
    };

    workerRef.current = worker;
    return () => worker.terminate();
  }, []);


  const imageToMatrix = (img: HTMLImageElement, threshold = 240): number[][] => {
    const canvas = canvasRef.current!;
    const ctx = canvas.getContext("2d")!;
    canvas.width = img.width;
    canvas.height = img.height;
    ctx.drawImage(img, 0, 0, img.width, img.height);

    const { data } = ctx.getImageData(0, 0, img.width, img.height);

    const matrix: number[][] = [];
    for (let y = 0; y < img.height; y++) {
      const row: number[] = [];
      for (let x = 0; x < img.width; x++) {
        const i = (y * img.width + x) * 4;
        const r = data[i];
        const g = data[i + 1];
        const b = data[i + 2];
        const a = data[i + 3];

        // transparent pixel → treat as background
        if (a < 10) {
          row.push(0);
          continue;
        }

        // brightness test for solid area
        const brightness = 0.299 * r + 0.587 * g + 0.114 * b;
        row.push(brightness < threshold ? 1 : 0);
      }
      matrix.push(row);
    }
    return matrix;
  };


  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (!file) return;

    const img = new Image();
    img.onload = () => {
      const matrix = imageToMatrix(img);
      setIsProcessing(true);
      setStlData("");
      workerRef.current?.postMessage({ matrix });
    };
    img.src = URL.createObjectURL(file);
  };

  const triggerDownload = (data: string, filename: string) => {
    const blob = new Blob([data], { type: "application/sla" });
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");
    link.href = url;
    link.download = filename;
    document.body.appendChild(link);
    link.click();
    link.remove();
    URL.revokeObjectURL(url);
  };

  return (
    <div className="marchSquareDemo" style={{ textAlign: "center", marginTop: "2rem" }}>
      <input
        type="file"
        accept="image/*"
        ref={fileInputRef}
        onChange={handleFileChange}
      />
      <canvas ref={canvasRef} style={{ display: "none" }} />

      {isProcessing && <p>Processing image into 3D model</p>}
      {stlData && <p>STL file generated and downloaded!</p>}
    </div>
  );
}

export default MarchSquareDemo;
