cd "$(dirname "$0")"
if [ -f portaudio/lib/.libs/libportaudio.a ] 
then echo "Found portaudio"
 else cd portaudio 
 ./configure && make
 fi

cd ..
if [ -f libpd/libs/libpd.a ]
then echo "Found static libpd"
 else cd libpd
 make STATIC=true EXTRA=true
 fi
 
if [ -f libpd/libs/libpd.dylib ]
then echo "Found dynamic libpd"
  else make clean
  make EXTRA=true
  fi
