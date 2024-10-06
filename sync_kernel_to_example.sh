#Sync source files
cp -v ./kernel_files/src/deeRTOS.c ./examples/$1/src/deeRTOS.c
cp -v ./kernel_files/src/mutex.c ./examples/$1/src/mutex.c
cp -v ./kernel_files/src/queue.c ./examples/$1/src/queue.c
cp -v ./kernel_files/src/semaphore.c ./examples/$1/src/semaphore.c

cp -v ./kernel_files/inc/deeRTOS.h ./examples/$1/inc/deeRTOS.h
cp -v ./kernel_files/inc/mutex.h ./examples/$1/inc/mutex.h
cp -v ./kernel_files/inc/queue.h ./examples/$1/inc/queue.h
cp -v ./kernel_files/inc/semaphore.h ./examples/$1/inc/semaphore.h

#Sync port
cp -v ./portable/$1/portmacro.h ./examples/$1/inc/portmacro.h
cp -v ./portable/$1/port.c ./examples/$1/src/port.c
