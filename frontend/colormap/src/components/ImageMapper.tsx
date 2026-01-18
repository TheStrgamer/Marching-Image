import { useState, useMemo, useRef, useEffect } from "react";
import "../App.css";
import { ColorMap } from "../scripts/colorMapper";
import { ImageHandler } from "../scripts/imageHandeler";

type Props = {
  colors: string[];
};

function loadImage(src: string): Promise<HTMLImageElement> {
  return new Promise((res) => {
    const img = new Image();
    img.src = src;
    img.onload = () => res(img);
  });
}

function colorMatches(r: number, g: number, b: number, hex: string, tol = 10) {
  const tr = parseInt(hex.slice(1, 3), 16);
  const tg = parseInt(hex.slice(3, 5), 16);
  const tb = parseInt(hex.slice(5, 7), 16);
  return Math.abs(r - tr) <= tol && Math.abs(g - tg) <= tol && Math.abs(b - tb) <= tol;
}

function imageToMatrixForColor(img: HTMLImageElement, color: string, canvas: HTMLCanvasElement) {
  const ctx = canvas.getContext("2d")!;
  canvas.width = img.width + 2;
  canvas.height = img.height + 2;
  ctx.drawImage(img, 0, 0, img.width, img.height);

  const { data } = ctx.getImageData(0, 0, img.width, img.height);
  const matrix: number[][] = [];

  const tb_row: number[] = Array(img.width + 2).fill(0);
  matrix.push(tb_row);

  for (let y = 0; y < img.height; y++) {
    const row: number[] = [0];
    for (let x = 0; x < img.width; x++) {
      const i = (y * img.width + x) * 4;
      const r = data[i], g = data[i + 1], b = data[i + 2], a = data[i + 3];
      if (a < 10) {
        row.push(0);
        continue;
      }
      row.push(colorMatches(r, g, b, color) ? 1 : 0);
    }
    row.push(0);
    matrix.push(row);
  }

  matrix.push(tb_row);
  return matrix;
}

function ImageMapper({ colors }: Props) {
  const [image, setImage] = useState<string | null>(null);
  const [maxSize, setMaxSize] = useState<number | 512>(512);
  const [result, setResult] = useState<string | null>(null);
  const [blur, setBlur] = useState<boolean>(false);
  const [blurFactor, setBlurFactor] = useState<number>(2);
  const [blurMethod, setBlurMethod] = useState<string>("simple")
  const [distMethod, setDistMethod] = useState<string>("rgb2lab")

  const [processing, setProcessing] = useState(false);
  const [colorSelection, setColorSelection] = useState<Record<string, boolean>>({});
  const canvasRef = useRef<HTMLCanvasElement>(document.createElement("canvas"));
  const workerRef = useRef<Worker | null>(null);

  useEffect(() => {
    const worker = new Worker(
      new URL("../scripts/marchingWorker.ts", import.meta.url),
      { type: "module" }
    );
    worker.onmessage = (e) => {
      const { stl, error, filename } = e.data;
      if (error) {
        console.error(error);
        setProcessing(false);
        return;
      }
      setProcessing(false);
      if (stl) triggerDownload(stl, filename || "model.stl");
    };
    workerRef.current = worker;
    return () => worker.terminate();
  }, []);

  useMemo(() => {
    const obj: Record<string, boolean> = {};
    colors.forEach(c => (obj[c] = true));
    setColorSelection(obj);
  }, [colors]);

  const toggleAll = (checked: boolean) => {
    const obj: Record<string, boolean> = {};
    colors.forEach(c => (obj[c] = checked));
    setColorSelection(obj);
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

  const mapColors = async () => {
    if (!image) return;
    setProcessing(true);

    try {
      const img = await loadImage(image);
      const colorMap = new ColorMap(colors);
      const handler = new ImageHandler();
      handler.setImage(img);

      if (blur) {
        if (blurMethod == "simple") handler.blurImage(blurFactor);
        else handler.blurImageBilateral(blurFactor)
      }
      handler.resize(maxSize)

      handler.mapImage(colorMap, distMethod == "euclidian");

      const out = handler.getDataURL();
      setResult(out);

    } catch (e) {
      console.error(e);
    } finally {
      setProcessing(false);
    }
  };

  const handleImageChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onloadend = () => {
      const b64 = reader.result as string;
      setImage(b64);
    };
    reader.readAsDataURL(file);
  };

  const exportSelected = async () => {
    if (!result) return;
    setProcessing(true);
    const img = await loadImage(result);

    for (const [color, selected] of Object.entries(colorSelection)) {
      if (!selected) continue;
      const matrix = imageToMatrixForColor(img, color, canvasRef.current!);
      workerRef.current?.postMessage({ matrix, filename: `model_${color.slice(1)}.stl` });
    }
  };

  return (
    <div className="image-handler">
      <div className="control-panel">
        <h2>Input</h2>
        <div className="aligned-items">
          <input type="file" accept="image/*" onChange={handleImageChange} />
          <button onClick={mapColors} disabled={!image || processing}>
            {processing ? "Processing…" : "Map Colors"}
          </button>
        </div>
        <div className="aligned-items">
          <label className="inline-check">
            <input
              type="checkbox"
              checked={blur}
              onChange={(e) => setBlur(e.target.checked)}
              style={{width: 12}}
            />
            blur
          </label>
          <label className="inline-check">
            <input
              type="number"
              value={blurFactor}
              onChange={(e) => setBlurFactor(Number(e.target.value))}
              min={0}
              max={25}
              style={{width: 60}}
            />
            blur factor
          </label>
          <label className="inline-check">
            <input
              type="number"
              value={maxSize}
              onChange={(e) => setMaxSize(Number(e.target.value))}
              min={0}
              max={1024}
              style={{width: 60}}
            />
            max size
          </label>
        </div>
        <div className="aligned-items">
          <label className="inline-check">
            distance method
            <select
              value={distMethod}
              onChange={(e) => setDistMethod(e.target.value)}
            >
              <option value="rgb2lab">rgb2lab</option>
              <option value="euclidian">euclidian</option>
            </select>
          </label>
          <label className="inline-check">
            blur method
            <select
              value={blurMethod}
              onChange={(e) => setBlurMethod(e.target.value)}
            >
              <option value="simple">simple</option>
              <option value="advanced">advanced</option>
            </select>
          </label>
        </div>

        {image && <img src={image} className="preview" />}
      </div>

      <div className="control-panel">
        <h2>Result</h2>
        {result && (
          <>
            <img src={result} className="preview" />

            <div className="color-select-card">
              <div style={{display:"flex", gap:"6px", marginBottom:8}}>
                <button onClick={() => toggleAll(true)}>select all</button>
                <button onClick={() => toggleAll(false)}>deselect all</button>
              </div>

              {colors.map((c) => (
                <label className="color-container" key={c}>
                  <input
                    type="checkbox"
                    checked={colorSelection[c]}
                    onChange={(e) =>
                      setColorSelection({ ...colorSelection, [c]: e.target.checked })
                    }
                  />
                  <div style={{ width: 12, height: 12, background: c, border: "1px solid #aaa" }} />
                  {c}
                </label>
              ))}

              <button onClick={exportSelected}>export selected</button>
            </div>
          </>
        )}
      </div>
    </div>
  );
}

export default ImageMapper;
