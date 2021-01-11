#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <float.h>

#define N 128

int main(int argc, char * argv[])
{
	
	FILE *f1;
	char inputfile[N];
    int indmin, indmax, indend = 0;
    int ind = 0;
    double i = 0;
    double c_max, c_min;

	setlocale(LC_ALL, "Rus");

	
	if(argc > 1)
		strcpy(inputfile, argv[1]);
	else {
		puts("Введите имя входного файла: ");
		gets(inputfile);
	}

	
	if((f1 = fopen(inputfile, "rb+")) == NULL) {
		perror("Error open file");
		// fclose(f1);
		return 1;
	}

    puts("Before changing:");
    fread(&c_max, sizeof(double), 1, f1);
    c_min = c_max;
    rewind(f1);
    indmin = indmax = 0;

    while(fread(&i, sizeof(double), 1, f1)) {
        printf("%lf\n", i);
        if(i > c_max) {
            c_max = i;
            indmax = ind;
        }
        if(i < c_min) {
            c_min = i;
            indmin = ind;
        }
        ind++;
    }
    indend = ind - 1;

    fseek(f1, indmax*sizeof(double), SEEK_SET);
    fwrite(&i, sizeof(double), 1, f1);

    fseek(f1, indend*sizeof(double), SEEK_SET);
    fwrite(&c_max, sizeof(double), 1, f1);


    if(indmin != indend || indmax != 0) {
        rewind(f1);
        ind = 0;
        while(fread(&i, sizeof(double), 1, f1)) {
            if(i == c_min) 
                indmin = ind;
            ind++;
        }
        rewind(f1);
        fread(&i, sizeof(double), 1, f1);
        rewind(f1);
        fwrite(&c_min, sizeof(double), 1, f1);
        fseek(f1, indmin*sizeof(double), SEEK_SET);
        fwrite(&i, sizeof(double), 1, f1);
    }

    rewind(f1);
    puts("After changing:");
    while(fread(&i, sizeof(double), 1, f1)) {
        printf("%lf\n", i);
    }

	fclose(f1);
	return 0;
}

// 3 2 5 4 1
// 5 2 1 4 3
