#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_COUNT 100 // Максимальное количество чисел для сортировки

// Объявления функции сортировки, реализованной в sort.s
void sort_numbers(long long int* array, int count);

// Функция парсинга аргументов 
int parse_arg(const char* arg, const char* prefix, int* value) {
    if (strncmp(arg, prefix, strlen(prefix)) == 0) {
        const char* num_part = arg + strlen(prefix);
        if (*num_part == '\0') {
            *value = 0; // Если нет числа после '=', трактуем как 0
            return 1;
        }
        char* end;
        long parsed_value = strtol(num_part, &end, 10);
        *value = parsed_value;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int from_flag = 0, to_flag = 0, from_value = 0, to_value = 0;

    // Проверка количества параметров
    if (argc < 2) return -1; // Меньше одного параметра
    if (argc > 3) return -2; // Больше двух параметров

    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        if (parse_arg(argv[i], "--from=", &from_value)) {
            if (from_flag) return -3; // Ошибка: дублирование --from=
            from_flag = 1;
        } else if (parse_arg(argv[i], "--to=", &to_value)) {
            if (to_flag) return -3; // Ошибка: дублирование --to=
            to_flag = 1;
        }
    }

    if (!from_flag && !to_flag) return -4; // Ошибка: некорректные параметры

    // Чтение чисел с помощью scanf
    long long int numbers[MAX_NUM_COUNT]; // Массив для ввода чисел
    int count = 0, stdout_count = 0, stderr_count = 0;
    while (scanf("%lld", &numbers[count]) == 1 && count < MAX_NUM_COUNT) {
        if (from_flag && numbers[count] <= from_value) {
            printf("%lld", numbers[count]); // Числа для вывода в stdout
            stdout_count++;
            if (count < stdout_count - 1)
            printf(" "); 
        } if (to_flag && numbers[count] >= to_value) {
            fprintf(stderr, "%lld", numbers[count]); // Числа для вывода в stderr
            stderr_count++;
            if (count < stderr_count - 1)
            fprintf(stderr, " ");
        }
        count++;
    }

    long long int sorted_numbers[MAX_NUM_COUNT]; // Массив под числа для сортировки
    int changed_count = 0, sorted_count = 0;
    // Поиск чисел для сортировки с учетом наличия флагов
    for (int i = 0; i < count; i++) {
        if (from_flag && !to_flag && numbers[i] > from_value) 
            sorted_numbers[sorted_count++] = numbers[i];
        if (!from_flag && to_flag && numbers[i] < to_value)
            sorted_numbers[sorted_count++] = numbers[i];
        if (from_flag && to_flag && numbers[i] > from_value && numbers[i] < to_value)
            sorted_numbers[sorted_count++] = numbers[i];
        }

    // Сохраняем копию массива до сортировки для сравнения
    long long int original_sorted[MAX_NUM_COUNT];
    for (int i = 0; i < sorted_count; i++)
        original_sorted[i] = sorted_numbers[i];

    // Сортировка подходящих чисел
    if (!from_flag || !to_flag || from_value < to_value)
        sort_numbers(sorted_numbers, sorted_count);

    // Подсчёт количества чисел, изменивших позицию
    for (int i = 0; i < sorted_count; i++) {
        if (sorted_numbers[i] != original_sorted[i]) changed_count++;
    }

    // Возвращаем количество элементов, изменивших позицию
    return changed_count;
}