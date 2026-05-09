# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

- **Qt Creator project** using qmake (Qt 5.8.0, MinGW 32bit). Open `test01/test01.pro` in Qt Creator.
- The project disables Qt GUI (`CONFIG -= qt`) — it's a console app that only uses OpenCV for GUI (`imshow`).
- Shadow build directory: `build-test01-Desktop_Qt_5_8_0_MinGW_32bit-Debug/`

## Building from CLI

```bash
cd build-test01-Desktop_Qt_5_8_0_MinGW_32bit-Debug
qmake ../test01/test01.pro
make
```

Or from Qt Creator: Build → Build Project "test01".

## Architecture

Single-file application (`test01/main.cpp`). No Qt GUI widgets — only OpenCV highgui for windows.

**Dependency:** OpenCV 3.4.2, expected at `C:\Users\ccs\Desktop\code\project_hide\opencv_3.4.2_Qt\`.

**Program flow:**
1. Loads background image(s) from disk (path hardcoded or via CLI args)
2. Opens camera (ID via `argv[1]`, default 0)
3. Real-time loop:
   - BGR → HSV conversion
   - `inRange` thresholding to isolate target color (default: dark/V-low)
   - Morphological opening (ellipse kernel) to remove noise
   - Gaussian blur on mask for edge feathering
   - Pixel-wise blending: `bitwise_and` on foreground/background, then `add`
4. Trackbars: HSV low/high, morph kernel size, blur kernel size. Number keys 1-9 switch backgrounds.

**CLI usage:** `test01 [camera_id] [bg_path1] [bg_path2] ...`
