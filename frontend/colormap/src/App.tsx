import "./App.css";
import ImageMapper from "./components/ImageMapper";
import ColorpickerList from "./components/ColorpickerList";
import { useState } from "react";
function App() {
  const [image, setImage] = useState<string | null>(null);
  const [colors, setColors] = useState(["#FF0000", "#FFFFFF"]);
  const [resultImage, setResultImage] = useState<string | null>(null);

  return (
    <>
      <h1>Upload image to map it (client-side)</h1>
      <div className="content">
        <div>
          <h2>Colors</h2>
          <ColorpickerList setColorList={setColors} />
        </div>
        <ImageMapper setImageParent={setImage} colors={colors} setResultImage={setResultImage}/>
        </div>
    </>
  );
}

export default App;
