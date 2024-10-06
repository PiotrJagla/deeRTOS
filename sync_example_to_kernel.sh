cp -v ./examples/$1/src/deeRTOS.c ./kernel_files/src/deeRTOS.c 
cp -v ./examples/$1/src/mutex.c ./kernel_files/src/mutex.c 
cp -v ./examples/$1/src/semaphore.c ./kernel_files/src/semaphore.c 
cp -v ./examples/$1/src/queue.c ./kernel_files/src/queue.c 

cp -v ./examples/$1/inc/deeRTOS.h ./kernel_files/inc/deeRTOS.h 
cp -v ./examples/$1/inc/mutex.h ./kernel_files/inc/mutex.h 
cp -v ./examples/$1/inc/semaphore.h ./kernel_files/inc/semaphore.h 
cp -v ./examples/$1/inc/queue.h ./kernel_files/inc/queue.h 
