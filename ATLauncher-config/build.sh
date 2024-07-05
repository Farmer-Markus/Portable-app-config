#!/bin/bash

glib-compile-resources --target gresource.c --generate-source gresource.xml

gcc `pkg-config --cflags gtk+-3.0` -o config main.c gresource.c `pkg-config --libs gtk+-3.0` -lcjson
