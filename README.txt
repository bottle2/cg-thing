The code is currently in a miserable state. Only select.{h, cpp} module remains untainted.
[ ] Documentation;
[ ] Clean code.

Help is inside the running program.

Requirements implemented:

[X] Load BMP files;
[X] Move images individually with the mouse;
[X] Be able to select images;
[X] Change image render:
    [X] Original;
    [X] Separate RGB channels;
    [X] Grayscale.
[ ] Rotate image horizontally and vertically;
[X] Plot histogram for RGB and luminancy;
[ ] Halve or double image size;
[X] Provide buttons to operate over selected image;
[X] Load images from working directory;
[X] Show help on program usage.

Extra requirements implemented:

[ ] Apply any rotation;
[ ] Brighten and darken image;
[ ] 7-segment display;
[ ] Operate over histogram as if an image.

Other extra requirements implemented:

[ ] Implement BMP loader from scratch;
    [ ] Load monochrome BMP;
    [ ] Load 16 color BMP;
    [ ] Load 256 color BMP;
    [ ] Load 24-bit color BMP;
    [ ] Load compressed BMP;
    [ ] Load BMP with legacy info header;
    [ ] Load BMP with offset hacks.
[ ] File explorer;
[X] Load any number of images;
[X] Select multiple images;
[X] Operate over multiple images;
[ ] Save image;
[ ] Viewport to zoom in and out the screen;
[ ] Move images on top or below others;
[ ] Keep images on screen after reducing screen size.
[X] Invert colors

Acronyms:

DIB device independent bitmap
BE  big-endian
LE  little-endian
n_  length of
_i  index of