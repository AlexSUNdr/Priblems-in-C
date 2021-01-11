#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <float.h>

#define N 128

struct person
{
    char name[30];
    char sex;
    char sport[30];
    short height;
    short year;
};

void add (char*);  
void edit (char*); 
void show_all (char*);  
void search (char*);   
void del (char*);      
void find(char*);
void find_uni(char*);

int main(int argc, char * argv[])
{
	
	FILE *f1;
	char inputfile[N];
    int indmin, indmax, indend = 0;
    int ind = 0;
    double i = 0;
    double c_max, c_min, temp;

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

    int menu;
    do
    {
       system ("clear");
       puts ("1. Добавление спортсмена");
       puts ("2. Редактирование спортсмена");
       puts ("3. Информация о спортсмене");
       puts ("4. Просмотр");
       puts ("5. Удаление спортсмена");
       puts ("6. Поиск самого высокого пловца");
       puts ("7. Найти юниорок");
       puts ("8. Выход");
       scanf ("%d%*c", &menu);
       switch (menu)
       {
            case 1 : add (inputfile); break;
            case 2 : edit (inputfile); break;
            case 3 : search (inputfile); break;
            case 4 : show_all (inputfile); break;
            case 5 : del (inputfile); break;
            case 6 : find(inputfile); break;
            case 7 : find_uni(inputfile);
       }
    }
    while (menu!=8);
    return 0;
}

/* ввод данных спортсмена */
struct person input_client (void)
{
    struct person sportsman;
    short temp;
    puts ("Фамилия:");
    gets (sportsman.name);
    puts ("Спорт:");
    gets (sportsman.sport);
    puts ("Пол (m - мужской, w - женский):");
    sportsman.sex = getchar();
    puts ("Год рождения:");
    scanf ("%hd", &temp);
    sportsman.year = temp;
    puts ("Рост:");
    scanf ("%hd", &temp);
    sportsman.height = temp;
    return sportsman;
}

/* добавление спорсмена в бинарный файл */
void add (char* file)
{
    FILE * f;
    struct person sportsman;
    system ("clear");
    if ((f=fopen (file, "ab"))==NULL)
    {
        perror ("Файл не открылся");
        getchar();
        return;
    }
    sportsman = input_client();
    fwrite (&sportsman, sizeof (sportsman), 1, f);
    fclose (f);
}

/* Вывод данных одного спортсмена */
void print1 (struct person data)
{
    printf ("Фамилия : %s\nвид спорта : %s\nпол : %c\nгод рождения : %hd\nРост : %hd\n",
            data.name, data.sport, data.sex, data.year, data.height);
}

/* Редактирование данных спортсмена */
void edit (char* file)
{
    FILE * f;
    struct person client;
    char name[30], yes;
    system ("clear");
    if ((f=fopen (file, "rb+"))==NULL)
    {
        perror ("Ошибка открытия файла"); 
        getchar();
        return;
    }
    rewind(f);

    printf ("Фамилия: "); gets (name);
    while (fread(&client,sizeof(client), 1, f)) {
        printf("%s is in client, %s is from consile\n", client.name, name);
        if (!strcmp(client.name,name))
        {
            print1 (client);
            puts ("Изменить этого спортсмена? (y/n)");
            do
            {
                yes = getchar(); getchar();
            }
            while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
            if (yes=='y' || yes=='Y')
            {
                client = input_client();
                fseek (f,-sizeof(client),SEEK_CUR);
                fwrite (&client, sizeof(client), 1, f);
                fseek (f,0,SEEK_CUR);
            }
        }
    }
    puts ("Поиск завершен"); getchar();
    fclose(f);
}


/* Показать всех спортсменов */
void show_all (char* file)
{
    FILE * f;
    struct person client;
    int page = 0, read = 0, page_n = 1;
    char signal;
    system ("clear");
    if ((f=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла");
        getchar();
        return;
    }
     
    while(fread(&client, sizeof(client), 1, f) == 1) {
        page++;
    }
    page = (page + 4) / 5;
    rewind(f);

    do {
        printf("Это %d страница списка\n", page_n);
        puts ("|        Фамилия        |    Спорт    | Год рождения |   Пол   |   Рост   |");
        puts ("---------------------------------------------------------------------------");
        while (fread(&client, sizeof(client), 1, f)==1 && read < 5) {
            printf ("|%-23s|%-13s|%14hd|%9c|%10hd|\n",
                    client.name, client.sport, client.year, client.sex, client.height);
            read++;
        }
        puts ("--------------------------------------------------------------------------");
        puts("r - следующая страница, l - предыдущая страница, q - выход");
        signal = getchar();
        getchar();

        system("clear");
        if(signal == 'r') {
            if(page_n + 1 > page) {
                puts("Это последняя страница, вы не можете посмотреть следующую страницу из-за её отсутствия.");
            }
            else {
                page_n++;
            }
            fseek(f, 5*(page_n - 1)*sizeof(client), SEEK_SET);
        }
        else if(signal == 'l') {
            if(page_n - 1 < 1) {
                puts("Это самая первая страница, вы не можете посмотреть предыдущую страницу из-за её отсутствия.");
            }
            else {
                page_n--;
            }
            fseek(f, 5*(page_n - 1)*sizeof(client), SEEK_SET);
        }
        read = 0;
    }
    while(signal != 'q');
    fclose (f);
    // getchar();
}

/* Поиск спортсмена */
void search (char* file)
{
    FILE * f;
    struct person client;
    char name[30], yes;
    system ("clear");
    if ((f=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла"); 
        getchar();
        return;
    }
    rewind(f);

    printf ("Фамилия: "); gets (name);
    while (fread(&client,sizeof(client), 1, f))
        if (!strcmp(client.name,name))
        {
            print1 (client);
            puts ("Это искомый спортсмен? (y/n)");
            do
            {
                yes = getchar(); getchar();
            }
            while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
            if (yes=='y' || yes=='Y')
            {
                break;
            }
        }
    puts ("Поиск завершен"); getchar();
    fclose(f);
}

/* Удаление спортсмена */
void del (char* file)
{
    FILE *f1, *f2;
    unsigned int n, n_file, i = 0;
    struct person client;
    char name[30], yes;
    system ("clear");
    if ((f1=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла"); 
        getchar();
        return;
    }

    if ((f2=fopen ("temp.dat", "wb"))==NULL)
    {
        perror ("Ошибка открытия файла");
        getchar();
        return;
    }

    printf ("Фамилия: "); gets (name);
    while (fread(&client, sizeof(client), 1, f1))
        if (!strcmp(client.name, name))
        {
            print1 (client);
            puts ("Это искомый спортсмен? (y/n)");
            do
            {
                yes = getchar(); getchar();
            }
            while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
            if (yes=='n' || yes=='N')
            {
                fwrite(&client, sizeof(client), 1, f2);
            }
        }
        else {
            fwrite(&client, sizeof(client), 1, f2);
        }
    puts ("Поиск завершен"); getchar();

    fclose (f1);
    fclose (f2);
    remove (file);
    rename ("temp.dat", file);
    puts ("Удаление закончено");
    getchar();
    // getchar();
}

void find(char* file) {
    FILE * f;
    struct person client;
    int res = -1, k = 0, col = 0;
    short max_h = 0;
    system ("clear");
    if ((f=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла"); 
        getchar();
        return;
    }
    rewind(f);

    while (fread(&client, sizeof(client), 1, f)) {
        // printf("%c is sex, %s is sport, %hd is height\n", client.sex, client.sport, client.height);
        if (client.sex == 'm' && !strcmp(client.sport, "swimming") && client.height >= max_h)
        {
            if(client.height == max_h) {
                col++;
            }
            else {
                col = 1;
                res = k;
                max_h = client.height;
            }
        }
        k++;
    }
    
    // system("clear");
    if(res == -1) {
        puts("В списке нет пловцов");
    }
    else if(col > 1) {
        puts("Самого высокого пловца нет");
    }
    else {
        fseek(f, res*sizeof(client), SEEK_SET);
        fread(&client, sizeof(client), 1, f);
        puts("Самый высокий пловец:");
        print1(client);
    }
    puts ("Поиск завершен"); getchar();
    fclose(f);
}

void find_uni (char* file)
{
    FILE * f, * f1;
    struct person client;
    int flag = 0;
    system ("clear");
    if ((f=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла");
        getchar();
        return;
    }

    if((f1 = fopen("temp.bin", "ab")) == NULL) {
        perror("Ошибка открытия файла");
        getchar();
        return;
    }
    
    while (fread(&client,sizeof(client), 1, f)) {
        if (client.sex == 'w' && 2020 - client.year < 18 && 2020 - client.year > 13)
        {
            flag = 1;
            fwrite(&client, sizeof(client), 1, f1);
        }
    }

    fclose (f);
    fclose (f1);
    if(flag) {
        puts("Сейчас откроется список юниорок");
        getchar();
        show_all("temp.bin");
    }
    else {
        puts("В списке спортсменов нет юниорок");
        getchar();
    }
    
    remove("temp.bin");
    // getchar();
}
