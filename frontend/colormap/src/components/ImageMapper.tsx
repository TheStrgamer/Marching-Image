import { useState, useMemo } from "react";
import "../App.css";
import { ColorMap } from "../scripts/colorMapper";
import { ImageHandler } from "../scripts/imageHandeler";

type Props = {
  setImageParent: (image: string | null) => void;
  setResultImage: (img: string | null) => void;
  colors: string[];
};

function loadImage(src: string): Promise<HTMLImageElement> {
  return new Promise((res) => {
    const img = new Image();
    img.src = src;
    img.onload = () => res(img);
  });
}

function ImageMapper({ setImageParent, setResultImage, colors }: Props) {
  const [image, setImage] = useState<string | null>(null);
  const [result, setResult] = useState<string | null>(null);

  const [blur, setBlur] = useState<boolean>(false);
  const [blurFactor, setBlurFactor] = useState<number>(2);
  const [processing, setProcessing] = useState(false);

  const [colorSelection, setColorSelection] = useState<Record<string, boolean>>({});

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

  const exportSelected = () => {
    //todo
    console.log("export selected not implemented");
  };

  const mapColors = async () => {
    if (!image) return;
    setProcessing(true);

    try {
      const img = await loadImage(image);

      const colorMap = new ColorMap(colors);
      const handler = new ImageHandler();
      handler.setImage(img);

      if (blur) handler.blurImage(blurFactor);

      handler.mapImage(colorMap);

      const out = handler.getDataURL();
      setResult(out);
      setResultImage(out);

      handler.cleanup();
    } catch (e) {
      console.error(e);
    } finally {
      setProcessing(false);
    }
  };

  const handleImageChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onloadend = () => {
      const b64 = reader.result as string;
      setImage(b64);
      setImageParent(b64);
    };
    reader.readAsDataURL(file);
  };

  return (
    <div className="image-handler">
      <div className="control-panel">
        <h2>Input</h2>
        <input type="file" accept="image/*" onChange={handleImageChange} />
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
          <button onClick={mapColors} disabled={!image || processing}>
            {processing ? "Processing…" : "Map Colors"}
          </button>
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
                <label className="color-container" key={c}
                >
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
