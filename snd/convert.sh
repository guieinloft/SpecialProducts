for i in *.wav; do ffmpeg -i "$i" "${i%.*}.ogg"; done
