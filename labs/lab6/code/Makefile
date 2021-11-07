CC = clang
CFLAGS = -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla
CFLAGS_main = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
DL_SORT_TXT_SRC = dl_sort_txt/list.c dl_sort_txt/sort.c dl_sort_txt/ui.c dl_sort_txt/dl_sort_txt.c
DL_SORT_CSV_SRC = dl_sort_csv/list.c dl_sort_csv/sort.c dl_sort_csv/ui.c dl_sort_csv/dl_sort_csv.c
DL_SORT_TXT = dl_sort_txt.so
DL_SORT_CSV = dl_sort_csv.so
L6 = l6
L6_MEMORY_CHECK = l6_memory_check
.PHONY: clean

all: $(DL_SORT_TXT_SRC) $(DL_SORT_CSV_SRC) main
	@echo l6 successfully constructed

$(DL_SORT_TXT):
	$(CC) $(CFLAGS) -shared -fPIC $(DL_SORT_TXT_SRC) -o $(DL_SORT_TXT)
	$(CC) $(CFLAGS) -shared -fPIC $(DL_SORT_CSV_SRC) -o $(DL_SORT_CSV)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main: main.c $(DL_SORT_TXT) $(DL_SORT_CSV_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_main) main.c -ldl -o $(L6_MEMORY_CHECK) -g
	$(CC) $(CFLAGS) $(CFLAGS_main) main.c -ldl -o $(L6) -g

clean:
	rm $(DL_SORT_TXT) $(DL_SORT_CSV) $(L6_MEMORY_CHECK) $(L6)
