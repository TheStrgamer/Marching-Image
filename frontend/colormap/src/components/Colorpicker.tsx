
type ColorpickerProps = {
  color: string;
  onChange: (newColor: string) => void;
  onRemove: () => void;
};

function Colorpicker({ color, onChange, onRemove }: ColorpickerProps) {
  return (
    <div className="colorpicker">
      <input
        type="color"
        value={color}
        onChange={(e) => onChange(e.target.value)}
      />
      <div className="color-display" style={{ backgroundColor: color }}></div>
      <button className="removeButton" onClick={onRemove}>
        X
      </button>
    </div>
  );
}

export default Colorpicker;
