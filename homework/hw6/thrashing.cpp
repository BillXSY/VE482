#include <stdio.h>
#include <stdlib.h>

typedef struct _Big {
	int *d1; // 4GB
	int *d2; // 4GB
	int *d3; // 4GB
	int *d4; // 4GB
	int *d5; // 4GB
	int *d6; // 4GB
	int *d7; // 4GB
	int *d8; // 4GB
} Big;

void initBit(Big *data);

int main() {
	Big data;
	initBit(&data);
	free(data.d1);
	free(data.d2);
	free(data.d3);
	free(data.d4);
	free(data.d5);
	free(data.d6);
	free(data.d7);
	free(data.d8);
	return 0;
}

void initBit(Big *data) {
	data->d1 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d1 init ok\n");
	data->d2 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d2 init ok\n");
	data->d3 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d3 init ok\n");
	data->d4 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d4 init ok\n");
	data->d5 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d5 init ok\n");
	data->d6 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d6 init ok\n");
	data->d7 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d7 init ok\n");
	data->d8 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d8 init ok\n");

	for (int i = 0; i < 1073741824; i++) (data->d1)[i] = i;
	printf("d1 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d2)[i] = i;
	printf("d2 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d3)[i] = i;
	printf("d3 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d4)[i] = i;
	printf("d4 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d5)[i] = i;
	printf("d5 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d6)[i] = i;
	printf("d6 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d7)[i] = i;
	printf("d7 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d8)[i] = i;
	printf("d8 calc ok\n");
}
