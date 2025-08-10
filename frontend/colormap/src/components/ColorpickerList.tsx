import { useState } from 'react';
import Colorpicker from './Colorpicker';

type form = {
      setColorList: (colors: string[]) => void;
};

function ColorpickerList({setColorList}: form) {
  const [colors, setColors] = useState(['#FF0000', '#FFFFFF']);

  const updateColor = (index: number, newColor: string) => {
    const updated = [...colors];
    updated[index] = newColor;
    setColors(updated);
    setColorList(updated);
    console.log(`Color at index ${index} updated to ${newColor}`);
  };

  const removeColor = (index: number) => {
    const updated = colors.filter((_, i) => i !== index);
    setColors(updated);
    setColorList(updated);
  };

  const addColor = () => {
    setColors([...colors, '#FFFFFF']);
    setColorList([...colors, '#FFFFFF']);
  };

  return (
    <div className="colorpicker-list">
      {colors.map((color, index) => (
        <Colorpicker
          key={index}
          color={color}
          onChange={(newColor) => updateColor(index, newColor)}
          onRemove={() => removeColor(index)}
        />
      ))}
      <button onClick={addColor} className="add-color-button">
        Add Color
      </button>
    </div>
  );
}

export default ColorpickerList;
