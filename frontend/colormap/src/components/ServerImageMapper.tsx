import { useEffect, useState } from "react";
import "../App.css";

type Props = {
  colors: string[];
};

export default function ServerImageMapper({
  colors,
}: Props) {
  const [image, setImage] = useState<string | null>(null);
  const [result, setResult] = useState<string | null>(null);
  const [processing, setProcessing] = useState(false);
  const [serverOnline, setServerOnline] = useState<boolean | null>(null);
  const [colorSelection, setColorSelection] = useState<Record<string, boolean>>(
    {}
  );
  const [maxSize, setMaxSize] = useState<number | 512>(512);
  const [blur, setBlur] = useState<boolean>(false);
  const [blurFactor, setBlurFactor] = useState<number>(2);
  const [method, setMethod] = useState<string>("rgb2lab")

  /* is server alive? */
  useEffect(() => {
    fetch("http://localhost:8080/api/health")
      .then((r) => r.ok ? setServerOnline(true) : setServerOnline(false))
      .catch(() => setServerOnline(false));
  }, []);

  useEffect(() => {
    const obj: Record<string, boolean> = {};
    colors.forEach((c) => (obj[c] = true));
    setColorSelection(obj);
  }, [colors]);

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

  const mapColorsServer = async () => {
    if (!image || !serverOnline) return;
    setProcessing(true);

    try {
      const response = await fetch("http://localhost:8080/api/color_map", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          image: image.split(",")[1],
          colors,
          blur, 
          blurFactor,
          maxSize,
          method
        }),
      });

      const data = await response.json();
      if (data.image) {
        const out = `data:image/jpeg;base64,${data.image}`;
        setResult(out);
      }
    } catch (err) {
      console.error("Server mapping failed:", err);
    } finally {
      setProcessing(false);
    }
  };

const exportSelected = async () => {
  if (!result) return;

  const selectedColors = Object.entries(colorSelection)
    .filter(([, v]) => v)
    .map(([k]) => k);

  if (selectedColors.length === 0) {
    alert("No colors selected");
    return;
  }

  try {
    const res = await fetch("http://localhost:8080/api/image_processing", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        image: result.split(",")[1],
        colors: selectedColors,
      }),
    });

    if (!res.ok) {
      throw new Error(await res.text());
    }

    const data = await res.json();

    // Expecting: { models: [{ color, stl }] }
    for (const model of data.models) {
      const binary = atob(model.stl);
      const bytes = new Uint8Array(binary.length);

      for (let i = 0; i < binary.length; i++) {
        bytes[i] = binary.charCodeAt(i);
      }

      const blob = new Blob([bytes], { type: "application/sla" });
      const url = URL.createObjectURL(blob);

      const a = document.createElement("a");
      a.href = url;
      a.download = `model-${model.color}.stl`;
      document.body.appendChild(a);
      a.click();
      document.body.removeChild(a);
      URL.revokeObjectURL(url);
    }

    console.log("Export completed");

  } catch (err) {
    console.error("Export failed:", err);
    alert("Export failed. See console.");
  }
};


  const toggleAll = (checked: boolean) => {
    const obj: Record<string, boolean> = {};
    colors.forEach((c) => (obj[c] = checked));
    setColorSelection(obj);
  };

  return (
    <div className="image-handler">
      <div className="control-panel">
        <h2>
          Input{" "}
          {serverOnline === false && (
            <span style={{ color: "red" }}>(server offline)</span>
          )}
          {serverOnline === true && (
            <span style={{ color: "limegreen" }}>(server online)</span>
          )}
        </h2>

        <div className="aligned-items">
          <input type="file" accept="image/*" onChange={handleImageChange} />
          <button
            onClick={mapColorsServer}
            disabled={!image || processing || !serverOnline}
          >
            {processing ? "Processing…" : "Map Colors (Server)"}
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
              max={1024*4}
              style={{width: 60}}
            />
            max size
          </label>
        </div>
        <div className="aligned-items">
          <label className="inline-check">
            distance method
            <select
              value={method}
              onChange={(e) => setMethod(e.target.value)}
            >
              <option value="HSL">hsl</option>
              <option value="Euclidian">euclidian</option>
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
              <div style={{ display: "flex", gap: 6, marginBottom: 8 }}>
                <button onClick={() => toggleAll(true)}>select all</button>
                <button onClick={() => toggleAll(false)}>deselect all</button>
              </div>

              {colors.map((c) => (
                <label className="color-container" key={c}>
                  <input
                    type="checkbox"
                    checked={colorSelection[c]}
                    onChange={(e) =>
                      setColorSelection({
                        ...colorSelection,
                        [c]: e.target.checked,
                      })
                    }
                  />
                  <div
                    style={{
                      width: 12,
                      height: 12,
                      background: c,
                      border: "1px solid #aaa",
                    }}
                  />
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
