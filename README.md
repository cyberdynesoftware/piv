# piv - Plain Image Viewer

* Supports most common image formats.
* Plays animations (gif and webp).
* Images occupy 100% of the window. No decorations, no fuss.
* Images are layouted in columns. The number of columns can be adjusted on the
  fly.

## Build

Enter the cloned folder and run
```
make
```

### Dependencies

* [sfml](https://www.sfml-dev.org)
* [boost](https://www.boost.org)
* [libwebp](https://developers.google.com/speed/webp)

## Usage

Key | Description
--- | ---
<kbd>Q</kbd> <kbd>Esc</kbd> | Quit
<kbd>F</kbd> | Toggle fullscreen
Mouse Wheel | Scroll
<kbd>Up</kbd> <kbd>K</kbd> | Scroll up
<kbd>Down</kbd> <kbd>J</kbd> | Scroll down
<kbd>PgUp</kbd> <kbd>U</kbd> | Scroll up fast
<kbd>PgDn</kbd> <kbd>D</kbd> | Scroll down fast
<kbd>I</kbd> | Toggle info
<kbd>1</kbd> - <kbd>0</kbd> | Set columns to 1-10
