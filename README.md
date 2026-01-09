# Marching Image

Convert images into one or more **flat 3D models (STL files)** using a modified marching squares–style algorithm.

This tool is primarily intended for **logos and simple graphics**, but can also be used with more complex images by mapping additional colors.

For most images the model will be pretty complex, so it is reccomended to preprocces the model in programs like blender. I reccomend the decimate modifier to reduce face cound without removing too many details.

---

## Features

- Image to flat 3D model (STL) conversion
- Color-based shape extraction
- Modified marching squares algorithm
- Export single or multiple colors
- Frontend GUI (TypeScript)
- Planned C++ backend (Docker-ready)

---

## Intended use

- Logos and icons
- Flat relief-style 3D models
- Rapid STL generation from images
- 3D printable models
- Geometry extraction experiments

## Installation

```bash
git clone https://github.com/TheStrgamer/Marching-Image.git
cd Marching-Image
```

## How to run frontend
For now only the frontend version is fully implemented, to run it, use the following commands
```Bash
cd frontend/colormap
npm install
npm run dev
```

## How it works

The image is processed one color at a time.
A binary matrix is created for each color:

1 → pixel matches the selected color

0 → pixel does not match

A marching squares–inspired lookup table determines vertex placement.
Faces are generated to create flat 3D geometry

The result is exported as an STL file

For more information on standard marching squares, see:
https://en.wikipedia.org/wiki/Marching_squares

## Example with c++ logo
The logo mapped in the application
![cpp_logo](readme_images/cpp_image.png)

The resulting model in blender
![cpp_logo_model](readme_images/cpp_model.png)

## Example with smaller shape
The shape mapped in the application
![cpp_logo](readme_images/shape_image.png)

The resulting model in blender
![cpp_logo_model](readme_images/shape_model.png)


## Example of more complex image
For more complex images, more colors might be needed to make it look good
Here is an example with an image of me.
![cpp_logo](readme_images/me_image.png)

The resulting model in blender, with colors
![cpp_logo_model](readme_images/me_model.png)


