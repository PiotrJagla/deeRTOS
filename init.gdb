target extended-remote localhost:3333
break src/main.c:main
break src/deeRTOS.c:systick_handler
break src/deeRTOS.c:pendsv_handler
run
c
lay next

