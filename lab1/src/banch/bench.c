#include "myvar2.h"
#include <time.h>
#include <stdlib.h>

int removeNonDigits(char* str) {
    int res = 0;
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        }
        str++;
    }
    return res;
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

char* generateRandomPhone() {
    char* phone = malloc(15);
    if(!phone) {
        return NULL;
    }
    
    sprintf(phone, "+%03d-%03d-%07d", rand() % 1000, rand() % 1000, rand() % 10000000);
    
    return phone;
}

int main() {
    srand(time(NULL));
    printf("Выберите размер массива: ");
    int n = 0;
    scanf("%d", &n);

    Pair pairs[n];
    char newStr[] = "value";

    for (int i = 0; i < n; i++) {
        pairs[i].number = generateRandomPhone();
        pairs[i].value = (char*)malloc(5*sizeof(char));
        strcpy(pairs[i].value, newStr);
    }

    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = removeNonDigits(pairs[i].number);
    }

    CreatingDigits(pairs, n);

    clock_t start_radix = clock();
    radix_sort(pairs, n);
    clock_t end_radix = clock();
    double time_radix = (double)(end_radix - start_radix) / CLOCKS_PER_SEC;
    
    clock_t start_quick = clock();
    qsort(arr, n, sizeof(int), compare);
    clock_t end_quick = clock();
    double time_quick = (double)(end_quick - start_quick) / CLOCKS_PER_SEC;

    printf("Время работы быстрой сортировки - %f сек\n", time_quick);
    printf("Время работы поразрядной сортировки - %f сек\n", time_radix);

    free(pairs->value);
}