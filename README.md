# audioappdemo
An example of binding qt, libpd and portaudio together.

If you want to use macdeplyqt you have to adjust the copy commands in the .pro file, otherwise delete them or
copy the libs manually. There was an issue with macdeployqt. I could not convince it to search
for libpd.dylib in any other directory than /Users/MYUSERNAME/libs, no matter what.
Maybe that's is fixed with the update to Qt6.


<img width="626" alt="AudioAppScreenshot" src="https://user-images.githubusercontent.com/10684202/162209396-d91505ce-1c8f-4928-8132-664ccf7f005a.png">
