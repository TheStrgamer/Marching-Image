import "./App.css";
import ImageMapper from "./components/ImageMapper";
import ServerImageMapper from "./components/ServerImageMapper";
import ColorpickerList from "./components/ColorpickerList";
import { use, useState } from "react";
function App() {
  const [image, setImage] = useState<string | null>(null);
  const [colors, setColors] = useState(["#FF0000", "#FFFFFF"]);
  const [resultImage, setResultImage] = useState<string | null>(null);

  const [clientOnly, setClientOnly] = useState<boolean>(true);

  return (
    <>
      <h1>
        Upload image to map{" "}
        <span>
          ({clientOnly ? "client-side" : "server-side"})
        </span>
      </h1>

      <div className="side-switch">
        <span className={!clientOnly ? "inactive" : "active"}>Client side</span>

        <button
          className={`switch ${clientOnly ? "left" : "right"}`}
          onClick={() => setClientOnly(!clientOnly)}
          aria-label="Toggle client or server side"
        >
          <span className="thumb" />
        </button>

        <span className={clientOnly ? "inactive" : "active"}>Server side</span>
      </div>   
      <div className="content">
        <div>
          <h2>Colors</h2>
          <ColorpickerList setColorList={setColors} />
        </div>
        {!clientOnly && (
          <ServerImageMapper
            setImageParent={setImage}
            colors={colors}
            setResultImage={setResultImage}
        />)}
        {clientOnly && (
          <ImageMapper 
            setImageParent={setImage} 
            colors={colors} 
            setResultImage={setResultImage}
        />)}
        </div>
    </>
  );
}

export default App;
