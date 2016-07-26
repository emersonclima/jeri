# Makefile for building Lua
# See doc/readme.html for installation and customization instructions.

all: compile install

clean:
	$(MAKE) -C src clean

compile:
	$(MAKE) -C src

install: compile
	echo "Instalando..."

uninstall:
	echo "Desinstalando..."