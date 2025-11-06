import { useState } from "react";
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
  const [blurFactor, setBlurFactor] = useState<number>(3);
  const [processing, setProcessing] = useState(false);

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

      setResultImage(handler.getDataURL());
      setResult(handler.getDataURL())
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
      <div className="image-upload">
        <input type="file" accept="image/*" onChange={handleImageChange} />

        <button onClick={mapColors} disabled={!image || processing}>
          {processing ? "Processing…" : "Map Colors"}
        </button>

        <label style={{ display: "flex", gap: "6px", alignItems: "center" }}>
          <input
            type="checkbox"
            checked={blur}
            onChange={(e) => setBlur(e.target.checked)}
          />
          blur
        </label>

        <input
          type="number"
          value={blurFactor}
          onChange={(e) => setBlurFactor(Number(e.target.value))}
          min={1}
          max={25}
          style={{ width: "60px" }}
        />

        {image && <img src={image} style={{ maxWidth: "100%" }} />}
      </div>
      <div className="result">
        {result && <img src={result} style={{ maxWidth: "100%" }} />}
      </div>
    </div>
  );
}

export default ImageMapper;
