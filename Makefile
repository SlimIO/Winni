.PHONY: install build

install:
	npm install

configure: install
	npx node-gyp configure

build:
	npx node-gyp build
