#Sync source files
cp -v ./examples/$1/src/* ./kernel_files/src/ 
cp -v ./examples/$1/inc/* ./kernel_files/inc/ 

#Sync port
cp -v ./examples/$1/inc/portmacro.h ./portable/$1/
cp -v ./examples/$1/inc/port.c ./portable/$1/
