#!/bin/sh

FILE="$(ls -1 video.* | head -n1)"
FRAMES=$(ls -1 frames/*.txt | sed "s/frames\///g" | sort -n)

# https://askubuntu.com/questions/110264/how-to-find-frames-per-second-of-any-video-file
FRAMERATE="$(ffmpeg -i "$FILE" 2>&1 | sed -n "s/.*, \(.*\) fp.*/\1/p")"

DELAY="$(dc -e "10 k 1 $FRAMERATE / p")"

for FRAME in $FRAMES ; do
	cat "frames/$FRAME"
	sleep "$DELAY"
done
