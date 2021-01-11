#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <float.h>

int main()
{
	FILE *f;
    int n;
    double x;
    if((f = fopen("int.bin", "wb")) == NULL) {
        perror("Ошибка открытия файла"); 
        return 2;
    }
    printf("Сколько чисел записать в файл?\n");
    scanf("%d", &n);
    printf("Введите %d чисел\n", n);
    while(n--) {
        scanf("%lf", &x);
        fwrite(&x, sizeof(double), 1, f);
    }
    fclose(f);
	return 0;
}
