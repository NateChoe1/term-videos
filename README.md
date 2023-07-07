# Terminal videos

Pixel-perfect ASCII art videos in the Linux console

## Usage

    ./download.sh [youtube link]
    setfont font.psf && ./play.sh

## A breakdown of font.psf

    72 b5 4a 86         # Magic bytes
    00 00 00 00         # Version
    20 00 00 00         # Header size (32 bytes)
    00 00 00 00         # Flags (No unicode table)
    00 01 00 00         # Size (256 glyphs)
    01 00 00 00         # Glyph size (1 byte)
    01 00 00 00         # Glyph height (1 byte)
    01 00 00 00         # Glyph width (1 byte)

    # Glyph table (every glyph is 1 white pixel except for 0x20, the space char)
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    00 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
    80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80
