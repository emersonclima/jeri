INSTALL_TOP= /usr/local/bin
INSTALL=install -p

all: compile install

clear:
	$(MAKE) -C src clean

compile:
	$(MAKE) -C src

install: compile
	cd src && $(INSTALL) pdbsel $(INSTALL_TOP)

uninstall:
	echo "Desinstalando..."
