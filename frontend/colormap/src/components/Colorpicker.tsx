
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
        style={{backgroundColor:"#00000000"}}
      />
      <button className="removeButton" onClick={onRemove}>
        X
      </button>
    </div>
  );
}

export default Colorpicker;
