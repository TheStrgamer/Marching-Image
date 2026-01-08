// This will be responsible for turning an image into one or more matrices
// of 0s and 1s for use by marching squares

import { Color } from "./color";

/**
 * A class that stores and manipulates a collection of colors.
 */
export class ColorMap {
  private colors: Color[];

  /**
   * Default constructor – initializes an empty list of colors.
   */
  constructor();
  /**
   * Constructor that accepts an array of color hex strings.
   */
  constructor(colors: string[]);
  /**
   * Constructor that accepts an array of Color objects.
   */
  constructor(colors: Color[]);
  constructor(colors?: (Color | string)[]) {
    this.colors = [];

    if (colors) {
      for (const color of colors) {
        if (typeof color === "string") {
          this.addColor(color);
        } else {
          this.addColor(color);
        }
      }
    }
  }

  /**
   * Get all colors in the map.
   */
  getColors(): Color[] {
    return [...this.colors];
  }

  /**
   * Get a color by index.
   * @throws if index is out of range.
   */
  getColor(index: number): Color {
    if (index < 0 || index >= this.colors.length) {
      throw new Error("Index out of bounds");
    }
    return this.colors[index];
  }

  /**
   * Add a color (Color object or hex string) to the map.
   */
  addColor(color: Color | string): void {
    if (typeof color === "string") {
      this.colors.push(new Color(color));
    } else {
      this.colors.push(color);
    }
  }

  /**
   * Remove a color by index.
   * @throws if index is out of range.
   */
  removeColor(index: number): void;
  /**
   * Remove a color by value (Color object or hex string).
   * @throws if color is not found.
   */
  removeColor(color: Color | string): void;
  removeColor(arg: number | Color | string): void {
    if (typeof arg === "number") {
      if (arg < 0 || arg >= this.colors.length) {
        throw new Error("Index out of bounds");
      }
      this.colors.splice(arg, 1);
    } else {
      const hexToRemove = typeof arg === "string" ? new Color(arg).getHex() : arg.getHex();
      const index = this.colors.findIndex(c => c.getHex() === hexToRemove);
      if (index === -1) {
        throw new Error("Color not found");
      }
      this.colors.splice(index, 1);
    }
  }

  /**
   * Clear the list of colors.
   */
  clear(): void {
    this.colors = [];
  }

  /**
   * Get the closest color (by RGB distance) to the given color.
   */
  getClosestColor(color: Color | string, eucli: boolean): Color {
    const target = typeof color === "string" ? new Color(color) : color;

    if (this.colors.length === 0) {
      throw new Error("ColorMap is empty");
    }

    let closest = this.colors[0];
    let minDistance = Number.MAX_SAFE_INTEGER;

    for (const c of this.colors) {
      if (c.getHex() === target.getHex()) {
        return c;
      }

      const distance = c.getDistance(target, eucli);
      if (distance < minDistance) {
        minDistance = distance;
        closest = c;
      } else if (distance === minDistance) {
        if (c.getHex() < closest.getHex()) {
          closest = c;
        }
      }
    }

    return closest;
  }
}
