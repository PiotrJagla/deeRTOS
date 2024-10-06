#Sync source files
cp -v ./kernel_files/src/* ./examples/$1/src
cp -v ./kernel_files/inc/* ./examples/$1/inc

#Sync port
cp -v ./portable/$1/portmacro.h ./examples/$1/inc/
cp -v ./portable/$1/port.c ./examples/$1/src/
