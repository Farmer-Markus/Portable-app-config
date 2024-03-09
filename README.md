# Portable-app-config

If you want to compile it yourself: <br />

on debian you need to install: <br />
`sudo apt install libcjson-dev` <br />
`sudo apt install libgtk-4-dev` <br />

Compile via:  <br /> 
``gcc `pkg-config --cflags gtk4` main.c -o config `pkg-config --libs gtk4` -lcjson``
