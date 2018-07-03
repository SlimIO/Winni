main: main.cpp
	cl /EHsc main.cpp src/NetworkAdapters.cpp /I src

temp: main.cpp
	cl /EHsc main.cpp
