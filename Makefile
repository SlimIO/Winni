.PHONY: install

install:
	npm install

configure: install
	npx node-gyp configure

build: configure
	npx node-gyp build
