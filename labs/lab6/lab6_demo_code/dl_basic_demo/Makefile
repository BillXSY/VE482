.DEFAULT_GOAL := dl_basic_demo

dl: dl_helloworld.c
	gcc -shared -Wall -Werror -o dl_helloworld.so dl_helloworld.c

dl_basic_demo: dl dl_basic_demo.c
	gcc -o dl_basic_demo dl_basic_demo.c -ldl

clean:
	rm dl_helloworld.so dl_basic_demo