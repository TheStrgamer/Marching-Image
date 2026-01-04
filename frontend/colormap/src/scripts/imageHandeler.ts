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

  mapImage(colorMap: ColorMap, eucli: boolean) {
    if (!this.canvas || !this.ctx) return;
    const { width, height } = this.canvas;
    const imageData = this.ctx.getImageData(0, 0, width, height);
    const data = imageData.data;

    for (let i = 0; i < data.length; i += 4) {
      const col = new Color(data[i], data[i + 1], data[i + 2]);
      const closest = colorMap.getClosestColor(col, eucli);

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

  blurImageBilateral(radius = 3, sigmaColor = 25, sigmaSpace = 3) {
    if (!this.canvas || !this.ctx) return;

    const { width, height } = this.canvas;
    const imageData = this.ctx.getImageData(0, 0, width, height);
    const src = imageData.data;
    const dst = new Uint8ClampedArray(src.length);

    const getIndex = (x: number, y: number) => (y * width + x) * 4;

    const gaussian = (x: number, sigma: number) =>
      Math.exp(-(x * x) / (2 * sigma * sigma));

    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        let rSum = 0, gSum = 0, bSum = 0, aSum = 0, wSum = 0;

        const i = getIndex(x, y);
        const r0 = src[i], g0 = src[i + 1], b0 = src[i + 2], a0 = src[i + 3];

        for (let ky = -radius; ky <= radius; ky++) {
          const yy = y + ky;
          if (yy < 0 || yy >= height) continue;
          for (let kx = -radius; kx <= radius; kx++) {
            const xx = x + kx;
            if (xx < 0 || xx >= width) continue;

            const j = getIndex(xx, yy);
            const r = src[j], g = src[j + 1], b = src[j + 2], a = src[j + 3];

            const ws = gaussian(Math.sqrt(kx * kx + ky * ky), sigmaSpace);

            const wc = a0 && a
              ? gaussian(Math.sqrt((r - r0) ** 2 + (g - g0) ** 2 + (b - b0) ** 2), sigmaColor)
              : 0;

            const w = ws * wc;

            rSum += r * w;
            gSum += g * w;
            bSum += b * w;
            aSum += a * w;
            wSum += w;
          }
        }

        if (wSum > 0) {
          dst[i] = rSum / wSum;
          dst[i + 1] = gSum / wSum;
          dst[i + 2] = bSum / wSum;
          dst[i + 3] = aSum / wSum;
        } else {
          dst[i] = r0;
          dst[i + 1] = g0;
          dst[i + 2] = b0;
          dst[i + 3] = a0;
        }
      }
    }
    const out = new ImageData(dst, width, height);
    this.ctx.putImageData(out, 0, 0);
  }

  resize(maxSize: number = 720) {
    if (!this.canvas || !this.ctx) return;

    const imgWidth = this.canvas.width;
    const imgHeight = this.canvas.height;

    if (Math.max(imgWidth, imgHeight) < maxSize) return;

    const scale = Math.min(maxSize / Math.max(imgWidth, imgHeight), 1);

    const width = Math.round(imgWidth * scale);
    const height = Math.round(imgHeight * scale);

    const resizedCanvas = document.createElement("canvas");
    resizedCanvas.width = width;
    resizedCanvas.height = height;
    const resizedCtx = resizedCanvas.getContext("2d")!;
    resizedCtx.drawImage(this.canvas, 0, 0, width, height);

    this.canvas = resizedCanvas;
    this.ctx = resizedCtx;
  }

}
