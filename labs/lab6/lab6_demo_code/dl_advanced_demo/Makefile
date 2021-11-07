.DEFAULT_GOAL := dl_advanced_demo

dl: dl_add_3.c dl_faulty_add_4.c plugin_manager.h plugin_manager.c
	gcc -c -Wall -Werror -fPIC dl_add_3.c plugin_manager.h plugin_manager.c
	gcc -shared -o dl_3.so dl_add_3.o plugin_manager.o
	gcc -c -Wall -Werror -fPIC dl_faulty_add_4.c plugin_manager.h plugin_manager.c
	gcc -shared -o dl_4.so dl_faulty_add_4.o plugin_manager.o

dl_advanced_demo: dl dl_advanced_demo.c
	gcc -g -o dl_advanced_demo dl_advanced_demo.c -ldl plugin_manager.h plugin_manager.c

clean:
	rm dl_add_3.o dl_faulty_add_4.o plugin_manager.o plugin_manager.h.gch dl_3.so dl_4.so dl_advanced_demo