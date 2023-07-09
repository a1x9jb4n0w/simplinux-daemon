# simplinux-daemon

hangs with the socket on port 31254

on a connection sends "Hello", closes the connection and keeps listenning

if launched by root - receives key events, symbols are written to ./backkey

Compilation :

    cd dirname
    git clone https://github.com/a1x9jb4n0w/simplinux-daemon
    cd simplinux-daemon
    cmake ./
    make
    ./sudo simpld
