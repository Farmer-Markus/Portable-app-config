# Portable-app-config

If you want to compile it yourself: <br />

on debian you need to install: <br />
`build-essential` <br />
`libcjson-dev` <br />
`libgtk-4-dev` <br />

`sudo apt-get install build-essential libcjson-dev libgtk-4-dev` <br />

Compile via:  <br /> 
``gcc `pkg-config --cflags gtk4` main.c -o config `pkg-config --libs gtk4` -lcjson``
