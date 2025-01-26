.PHONY: all lib app clean

all: lib app

lib:
	$(MAKE) -C src

app:
	$(MAKE) -C app

clean:
	$(MAKE) -C src clean
	$(MAKE) -C app clean

