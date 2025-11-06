import * as StackBlur from "stackblur-canvas";
import { ColorMap } from "./colorMapper";
import { Color } from "./color";

export class ImageHandler {
  canvas: HTMLCanvasElement | null = null;
  ctx: CanvasRenderingContext2D | null = null;

  setImage(img: HTMLImageElement) {
    this.canvas = document.createElement("canvas");
    this.canvas.width = img.width;
    this.canvas.height = img.height;
    this.ctx = this.canvas.getContext("2d")!;
    this.ctx.drawImage(img, 0, 0);
  }

  mapImage(colorMap: ColorMap) {
    if (!this.canvas || !this.ctx) return;
    const { width, height } = this.canvas;
    const imageData = this.ctx.getImageData(0, 0, width, height);
    const data = imageData.data; // Uint8ClampedArray [r,g,b,a,r,g,b,a...]

    for (let i = 0; i < data.length; i += 4) {
      const col = new Color(data[i], data[i + 1], data[i + 2]);
      const closest = colorMap.getClosestColor(col);

      data[i]     = closest.getRed();
      data[i + 1] = closest.getGreen();
      data[i + 2] = closest.getBlue();
    }

    this.ctx.putImageData(imageData, 0, 0);
  }

  blurImage(radius = 3) {
    if (!this.canvas) return;
    StackBlur.canvasRGBA(this.canvas, 0, 0, this.canvas.width, this.canvas.height, radius);
  }

  getDataURL(): string | null {
    if (!this.canvas) return null;
    return this.canvas.toDataURL("image/png");
  }

  cleanup() {
    // nothing – GC handles it
  }
}
