import React, { useState } from 'react';
import Colorpicker from './Colorpicker';

function ColorpickerList() {
  const [colors, setColors] = useState(['#FF0000', '#FFFFFF']);

  const updateColor = (index: number, newColor: string) => {
    const updated = [...colors];
    updated[index] = newColor;
    setColors(updated);
    console.log(`Color at index ${index} updated to ${newColor}`);
  };

  const removeColor = (index: number) => {
    const updated = colors.filter((_, i) => i !== index);
    setColors(updated);
  };

  const addColor = () => {
    setColors([...colors, '#FFFFFF']);
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
