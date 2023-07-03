#!/bin/sh

if [ $# -lt 1 ] ; then
	echo "Usage: $0 [video link]"
	exit
fi

youtube-dl -f 'bestvideo[height<=480]+bestaudio' -o video -- "$1"

FILE="$(ls -1 video.* | head -n1)"

ffmpeg -i "$FILE" frames/%04d.ppm

make -C frames

FRAMES=$(ls -1 frames/*.ppm | sort -n)

for FRAME in $FRAMES ; do
	NEW_FRAME=$(echo "$FRAME" | sed "s/ppm/txt/")
	./frames/convert "$FRAME" "$NEW_FRAME"
done
