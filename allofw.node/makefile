all:
	make -C build
	make -C utils

rebuild:
	rm -rf build
	node-gyp configure
	make -C build
