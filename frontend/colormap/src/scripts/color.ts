/**
 * Convert a number (0–255) to a 2-digit hexadecimal string.
 */
function toHex(num: number): string {
  if (num < 0) return "00";
  if (num > 255) return "FF";
  const hex = "0123456789ABCDEF";
  return hex[Math.floor(num / 16)] + hex[num % 16];
}

/**
 * Convert a hexadecimal string to a number.
 */
function toNum(hex: string): number {
  let num = 0;
  for (const c of hex.toUpperCase()) {
    let val: number;
    if (c >= "0" && c <= "9") val = c.charCodeAt(0) - "0".charCodeAt(0);
    else if (c >= "A" && c <= "F") val = c.charCodeAt(0) - "A".charCodeAt(0) + 10;
    else throw new Error(`Invalid hex character: ${c}`);
    num = num * 16 + val;
  }
  return num;
}

/**
 * Clamp a number between min and max.
 */
function setInLimits(num: number, min: number, max: number): number {
  return Math.max(min, Math.min(num, max));
}

export class Color {
  private red: number;
  private green: number;
  private blue: number;
  private hex: string;

  /**
   * Default constructor (black).
   */
  constructor();
  /**
   * Constructor with RGB values.
   */
  constructor(red: number, green: number, blue: number);
  /**
   * Constructor with HEX string (#RRGGBB).
   */
  constructor(hex: string);
  constructor(a?: number | string, b?: number, c?: number) {
    if (typeof a === "string") {
      // Construct from HEX
      if (a.length !== 7 || a[0] !== "#") {
        throw new Error("Invalid hex color format. Expected #RRGGBB");
      }
      this.hex = a.toUpperCase();
      this.red = toNum(a.slice(1, 3));
      this.green = toNum(a.slice(3, 5));
      this.blue = toNum(a.slice(5, 7));
    } else if (typeof a === "number" && typeof b === "number" && typeof c === "number") {
      // Construct from RGB
      this.red = setInLimits(a, 0, 255);
      this.green = setInLimits(b, 0, 255);
      this.blue = setInLimits(c, 0, 255);
      this.hex = `#${toHex(this.red)}${toHex(this.green)}${toHex(this.blue)}`;
    } else {
      // Default (black)
      this.red = 0;
      this.green = 0;
      this.blue = 0;
      this.hex = "#000000";
    }
  }

  getRed(): number {
    return this.red;
  }
  getGreen(): number {
    return this.green;
  }
  getBlue(): number {
    return this.blue;
  }
  getHex(): string {
    return this.hex;
  }

  setRed(red: number): void {
    this.red = setInLimits(red, 0, 255);
    this.updateHex();
  }
  setGreen(green: number): void {
    this.green = setInLimits(green, 0, 255);
    this.updateHex();
  }
  setBlue(blue: number): void {
    this.blue = setInLimits(blue, 0, 255);
    this.updateHex();
  }

  private updateHex(): void {
    this.hex = `#${toHex(this.red)}${toHex(this.green)}${toHex(this.blue)}`;
  }

  addColor(color: Color | string): void {
    const c = typeof color === "string" ? new Color(color) : color;
    this.setRed(this.red + c.red);
    this.setGreen(this.green + c.green);
    this.setBlue(this.blue + c.blue);
  }

  subtractColor(color: Color | string): void {
    const c = typeof color === "string" ? new Color(color) : color;
    this.setRed(this.red - c.red);
    this.setGreen(this.green - c.green);
    this.setBlue(this.blue - c.blue);
  }

  getDistance(color: Color): number {
    return Math.sqrt(
      Math.pow(this.red - color.red, 2) +
      Math.pow(this.green - color.green, 2) +
      Math.pow(this.blue - color.blue, 2)
    );
  }
}
