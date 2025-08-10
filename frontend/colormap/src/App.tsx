import './App.css'
import ImageUpload from './components/ImageUpload'
import ColorpickerList from './components/ColorpickerList'
import { useState } from 'react';




function App() {

    const [image, setImage] = useState<string | null>(null)
    const [colors, setColors] = useState(['#FF0000', '#FFFFFF']);
    const [resultImage, setResultImage] = useState<string | null>(null);

  const requestMapping = async () => {
      if (!image) {
        console.warn("No image provided.");
        return;
      }
      console.log("Requesting color mapping with image and colors:", colors);

      try {
        const response = await fetch("http://localhost:8080/api/color_map", {
          method: "POST",
          headers: {
            "Content-Type": "application/json"
          },
          body: JSON.stringify({
            image: image.split(',')[1],
            colors: colors
          })
        });

        const data = await response.json();

        if (data.image) {
          setResultImage(`data:image/jpeg;base64,${data.image}`);
        } else {
          console.log("Success:", data);
        }
      } catch (error) {
        console.error("Error:", error);
      }
    };
    

  return (
    <>
        <h1>Upload image to map it</h1>
        <div className="content">
          <div>
          <h2>Colors</h2>
          <ColorpickerList setColorList={setColors} />
          </div>
          <ImageUpload setImageParent={setImage} />
          <div>
            <button onClick={requestMapping} disabled={!image}>
              Map Colors
            </button>

            {resultImage && (
              <div className="result">
                <h2>Result</h2>
                <img src={resultImage} alt="Mapped result" />
              </div>
            )}
          </div>

        </div>        
    </>
  )
}

export default App
