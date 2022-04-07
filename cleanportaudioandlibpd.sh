cd "$(dirname "$0")"
cd portaudio
make clean



cd ../libpd
make clean
cd libs
rm libpd.a

