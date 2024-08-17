target extended-remote localhost:3333
break src/main.c:main
break src/cmsis_util.c:systick_handler
run
c
lay next

