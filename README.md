# piv - Plain Image Viewer

A plain and simple image viewer. Supports most common image formats and plays
animations.

## Build

Enter the cloned folder and run
```
make
```

### Build dependencies

* [sfml](https://www.sfml-dev.org)
* [boost](https://www.boost.org)
* [stb_image](https://github.com/nothings/stb)
* [libwebp](https://developers.google.com/speed/webp)

## Usage

### Global

Key | Description
--- | ---
Q | Quit
F | Toggle fullscreen

### Multi image view

Key | Description
--- | ---
Arrow Up/Down & Mouse Wheel | Scroll Up/Down
I | Show image info
1-0 | Set columns to 1-10
Left Mouse Click | View selected image in single image view

### Single image view

Key | Description
--- | ---
Space & Extra Mouse Button 1 | Next image
Backspace & Extra Mouse Button 2 | Previous image
Mouse Wheel | Zoom image
Left Mouse Button | Move image on screen
Middle Mouse Click | Reset zoom and position
O | Original image size
I | Show image info
M & Right Mouse Button | Return to multi image view
