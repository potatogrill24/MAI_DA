#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {   //структура для хранения номера, значения и разрядов номера по-отдельности
    char* number;
    char* value;
    int NUM[3];
} Pair;

void CreatingDigits(Pair* arr, int n) { // функция для дробления номера телефона на разряды 
    for (int i = 0; i < n; i++) {
        char temp[20];
        strcpy(temp, arr[i].number);
        char *token = strtok(temp, "-"); //записываем номер в массив temp и в массив token записываем его часть до знака -

        if (token != NULL) {
            arr[i].NUM[0] = atoll(token);
        }
        else {
            arr[i].NUM[1] = 0;   // конвертируем массив char* в число int и записываем в поле рассматриваемого элемента массива arr
        }

        token = strtok(NULL, "-"); // дробим дальше до знака - и конвертируем
        if (token != NULL) {
            arr[i].NUM[1] = atoll(token);
        }
        else {
            arr[i].NUM[1] = 0;
        }
    
        token = strtok(NULL, "\0"); // дробим до знака конца строки и конвертируем
        if (token != NULL) {
            arr[i].NUM[2] = atoll(token);
        }
        else {
            arr[i].NUM[2] = 0;
        }
    }
}

void counting_sort(Pair *arr, int n, int exp, int var) { // функция сортировки подсчетом input = [3, 4, 5, 7, 1]
    Pair output[n];
    int count[10] = {0}; // создает выходной массив output и массив count из 10 элементов (каждый элемент - для каждой цифры)

    for (int i = 0; i < n; i++) {
        int index = (arr[i].NUM[var] / exp) % 10; // получем число индекс для нужного разряда каждого числа в массиве
        count[index]++; //увеличиваем значение соответствующего числу элемента массива на единицу  count = [0, 1, 0, 1, 1, 1, 0, 1, 0, 0]
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1]; // находим сумму префиксов [0, 1, 1, 2, 3, 4, 4, 5, 5, 5]
    }

    for (int i = n - 1; i >= 0; i--) { // формируем выходной массив записывая в него такое количество нужных чисел, какое их количество хранится в массиве count
        int index = (arr[i].NUM[var] / exp) % 10;
        output[count[index] - 1] = arr[i];  // [1, 3, 4, 5, 7]
        count[index]--; // уменьшаем на единицу нужное значение массива count
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];   // записываем данные выходного массива в исходный
    }
}

int MaxLength(Pair *arr, int n, int pos) { // функция для поиска максимального числа в массиве
    int max = arr[0].NUM[pos];
    for (int i = 1; i < n; i++) {
        if (arr[i].NUM[pos] > max) {
            max = arr[i].NUM[pos];
        }
    }
    return max;
}

void radix_sort(Pair* arr, int n) { // функция поразрядной сортировки
    int max1 = MaxLength(arr, n, 2); // ищем среди номеров телефона максимальное число
    for (int exp = 1; max1 / exp > 0; exp *= 10) { // вызываем сортировку подсчетом столько раз сколько разрядов содержится в максимальном числе массива
        counting_sort(arr, n, exp, 2);
    }

    int max2 = MaxLength(arr, n, 1); // ищем среди кодов города максимально число
    for (int exp = 1; max2 / exp > 0; exp *= 10) {
        counting_sort(arr, n, exp, 1);
    }

    int max3 = MaxLength(arr, n, 0); // ищем среди кодов стран максимальное число
    for (int exp = 1; max3 / exp > 0; exp *= 10) {
        counting_sort(arr, n, exp, 0);
    }

    for (int i = 0; i < n; i++) { // выводим получившийся отсортированный массив в виде "ключ-значение"
        printf("%s\t%s\n", arr[i].number, arr[i].value);
    }
}

int main() {
    Pair* pairs = NULL; // инициализация массива pairs
    pairs = (Pair*)malloc(0 * sizeof(Pair));
    if (pairs == NULL) return 1;
    char line[2071];
    int c = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) { //Начинаем считывать стандратный ввод до EOF
        if (line[0] == '\t' || line[0] == '\n') { // Если считывается пустая строка то пропускаем ее
            continue;
        }
        char *second_field = strchr(line, '\t');
        int l = strlen(second_field);
        // printf("%d\n", l);

        Pair* temp = (Pair*)realloc(pairs, (c + 1) * sizeof(Pair)); // увеличиваем количество выделяемых байт памяти на одну единицу (единицу измерения размера структуры Pair)
        if (temp == NULL) {
            free(pairs);
            return 1;
        }
        pairs = temp;
        pairs[c].number = (char*)malloc(25*sizeof(char)); // выделяем память для поля number каждого элемента массива pairs
        pairs[c].value = (char*)malloc(l*sizeof(char));
        sscanf(line, "%s\t%s", pairs[c].number, pairs[c].value); // считываем два поля через знак табуляции и записываем их в поля number и value соответсвтенно
        // pairs[c].value[2048] = '\0';
        c++;
    }

    CreatingDigits(pairs, c);
    radix_sort(pairs, c);
    free(pairs); // освобождаем память, выделенную под массив pairs
}

