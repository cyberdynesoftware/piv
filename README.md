# piv - Plain Image Viewer

* Supports most common image formats.
* Plays animations (gif and webp).
* Images occupy 100% of the window. No decorations, no fuss.
* Images are layouted in columns. The number of columns can be adjusted on the fly.
* Copy or move images to a selection folder.

## Build

Enter the cloned folder and run
```
cmake -B build
cmake --build build
```

### Dependencies

* [sfml](https://www.sfml-dev.org)
* [libwebp](https://developers.google.com/speed/webp)
* [box2d](https://box2d.org)

## Usage

Run with
```
piv path/to/image
```

Key | Description
--- | ---
<kbd>Esc</kbd> | Quit
<kbd>F</kbd> | Toggle fullscreen
<kbd>H</kbd> | Display help
