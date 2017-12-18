all:
	mkdir -p build/
	cd bindings/lisp/ && \
		ln -sf ../../build/
	cd bindings/python/ && \
		ln -sf ../../build/
	cd build/ && \
		cmake .. && \
		make
clean:
	cd bindings/lisp/ && \
		rm -f build
	cd bindings/python/ && \
		rm -f build
	rm -rf build/
