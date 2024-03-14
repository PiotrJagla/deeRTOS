target extended-remote localhost:3333
break src/main.c:main
break src/main.c:systick_handler
run
c
lay next

