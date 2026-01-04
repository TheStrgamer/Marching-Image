import "./App.css";
import ImageMapper from "./components/ImageMapper";
import ColorpickerList from "./components/ColorpickerList";
import { use, useState } from "react";
function App() {
  const [image, setImage] = useState<string | null>(null);
  const [colors, setColors] = useState(["#FF0000", "#FFFFFF"]);
  const [resultImage, setResultImage] = useState<string | null>(null);

  const [clientOnly, setClientOnly] = useState<boolean>(true);

  return (
    <>
      <h1>Upload image to map it {clientOnly && <span> (client-side)</span>}{!clientOnly && <span> (server-side)</span>}</h1>
      <label className="inline-check">
        <input
        type="checkbox"
            checked={clientOnly}
            onChange={(e) => setClientOnly(e.target.checked)}
            style={{width: 12}}
          />
          client side
      </label>      
        <div className="content">
        <div>
          <h2>Colors</h2>
          <ColorpickerList setColorList={setColors} />
        </div>
        {!clientOnly && <h2>Not fully finished yet</h2>}
        {clientOnly && <ImageMapper setImageParent={setImage} colors={colors} setResultImage={setResultImage}/>}
        </div>
    </>
  );
}

export default App;
