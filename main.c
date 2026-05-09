#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

#define MAX_PRODUCTS 1000
#define NAME_LEN 50
#define BRAND_LEN 30
#define WEBSITE_LEN 100

struct Product {
    char name[NAME_LEN];
    char brand[BRAND_LEN];
    int price;
    char website[WEBSITE_LEN];
};

struct Product database[MAX_PRODUCTS];
int count = 0;

void menu();
void input_menu();
void input_keyboard();
void input_file();
void output_menu();
void output_screen();
void output_file();
void add_data();
void search_menu();
void sort_menu();
void delete_menu();
void sort_data(int field);
void print_product(struct Product p);
void clear_input_buffer();
int get_int_input();

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/**
 * Проверяет корректность введённой строки
 * @param str - проверяемая строка
 * @param max_len - максимальная длина
 * @return 1 если корректна, 0 если нет
 */
int validate_string(const char* str, int max_len) {
    if (str == NULL || strlen(str) == 0) {
        return 0;
    }
    if (strlen(str) >= max_len) {
        return 0;
    }
    return 1;
}




int get_int_input() {
    int value;
    int result;

    while (1) {
        result = scanf("%d", &value);
        clear_input_buffer();

        if (result == 1) {
            return value;
        }
        else {
            printf("Ошибка: введите корректное число: ");
        }
    }
}


void menu() {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("  БАСКЕТБОЛЬНЫЙ МАГАЗИН - БАЗА ДАННЫХ\n");
        printf("========================================\n");
        printf("1. Ввод данных (сброс базы)\n");
        printf("2. Вывод данных\n");
        printf("3. Добавление товара\n");
        printf("4. Поиск\n");
        printf("5. Сортировка\n");
        printf("6. Удаление\n");
        printf("7. Выход\n");
        printf("========================================\n");
        printf("Выберите действие: ");

        choice = get_int_input();

        switch (choice) {
        case 1:
            input_menu();
            break;
        case 2:
            output_menu();
            break;
        case 3:
            add_data();
            break;
        case 4:
            search_menu();
            break;
        case 5:
            sort_menu();
            break;
        case 6:
            delete_menu();
            break;
        case 7:
            printf("Выход из программы.\n");
            exit(0);
        default:
            printf("Ошибка: выберите пункт от 1 до 7.\n");
        }
    }
}


void input_menu() {
    int choice;

    printf("\n--- ВВОД ДАННЫХ ---\n");
    printf("1. Ввод с клавиатуры\n");
    printf("2. Ввод из файла\n");
    printf("0. Отмена\n");
    printf("Выберите способ ввода: ");

    choice = get_int_input();

    switch (choice) {
    case 1:
        input_keyboard();
        break;
    case 2:
        input_file();
        break;
    case 0:
        printf("Ввод отменен.\n");
        break;
    default:
        printf("Ошибка: некорректный выбор.\n");
    }
}


void input_keyboard() {
    int n;

    printf("\nВвод данных с клавиатуры\n");
    printf("Сколько товаров добавить (максимум %d)? ", MAX_PRODUCTS);
    n = get_int_input();

    if (n <= 0 || n > MAX_PRODUCTS) {
        printf("Ошибка: количество должно быть от 1 до %d.\n", MAX_PRODUCTS);
        return;
    }

    count = n;

    for (int i = 0; i < count; i++) {
        printf("\n--- Товар %d ---\n", i + 1);

        printf("Наименование: ");
        if (scanf("%49s", database[i].name) != 1) {
            printf("Ошибка ввода наименования.\n");
            clear_input_buffer();
            i--;
            continue;
        }
        clear_input_buffer();

        printf("Бренд: ");
        if (scanf("%29s", database[i].brand) != 1) {
            printf("Ошибка ввода бренда.\n");
            clear_input_buffer();
            i--;
            continue;
        }
        clear_input_buffer();

        printf("Цена: ");
        database[i].price = get_int_input();

        if (database[i].price < 0) {
            printf("Ошибка: цена не может быть отрицательной.\n");
            i--;
            continue;
        }

        printf("Сайт: ");
        if (scanf("%99s", database[i].website) != 1) {
            printf("Ошибка ввода сайта.\n");
            clear_input_buffer();
            i--;
            continue;
        }
        clear_input_buffer();
    }

    printf("\nВведено товаров: %d\n", count);
}


void input_file() {
    char filename[100];
    FILE* file;

    printf("\nВвод данных из файла\n");
    printf("Введите имя файла: ");
    if (scanf("%99s", filename) != 1) {
        printf("Ошибка ввода имени файла.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл '%s'.\n", filename);
        return;
    }

    count = 0;
    while (count < MAX_PRODUCTS &&
        fscanf(file, "%49s %29s %d %99s",
            database[count].name,
            database[count].brand,
            &database[count].price,
            database[count].website) == 4) {

        if (database[count].price < 0) {
            printf("Предупреждение: отрицательная цена в строке %d, пропускаем.\n", count + 1);
            continue;
        }
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("Ошибка: не удалось прочитать данные из файла.\n");
    }
    else {
        printf("Успешно загружено %d товаров из файла '%s'.\n", count, filename);
    }
}


void output_menu() {
    int choice;

    if (count == 0) {
        printf("\nБаза данных пуста.\n");
        return;
    }

    printf("\n--- ВЫВОД ДАННЫХ ---\n");
    printf("1. Вывод на экран\n");
    printf("2. Вывод в файл\n");
    printf("0. Отмена\n");
    printf("Выберите способ вывода: ");

    choice = get_int_input();

    switch (choice) {
    case 1:
        output_screen();
        break;
    case 2:
        output_file();
        break;
    case 0:
        printf("Вывод отменен.\n");
        break;
    default:
        printf("Ошибка: некорректный выбор.\n");
    }
}

void output_screen() {
    if (count == 0) {
        printf("\nБаза данных пуста.\n");
        return;
    }

    printf("\n№\tНаименование\tБренд\t\tЦена\tСайт\n");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t\t%s\t\t%d\t%s\n", 
               i + 1, 
               database[i].name, 
               database[i].brand, 
               database[i].price, 
               database[i].website);
    }
}


void output_file() {
    char filename[100];
    FILE* file;

    printf("\nВывод данных в файл\n");
    printf("Введите имя файла: ");
    if (scanf("%99s", filename) != 1) {
        printf("Ошибка ввода имени файла.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл '%s'.\n", filename);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d %s\n",
            database[i].name,
            database[i].brand,
            database[i].price,
            database[i].website);
    }

    fclose(file);
    printf("Данные успешно сохранены в файл '%s' (%d товаров).\n", filename, count);
}


void add_data() {
    if (count >= MAX_PRODUCTS) {
        printf("Ошибка - база данных заполнена (максимум %d товаров).\n", MAX_PRODUCTS);
        return;
    }

    printf("\nДОБАВЛЕНИЕ ТОВАРА\n");

    printf("Наименование: ");
    if (scanf("%49s", database[count].name) != 1) {
        printf("Ошибка ввода наименования.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Бренд: ");
    if (scanf("%29s", database[count].brand) != 1) {
        printf("Ошибка ввода бренда.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Цена: ");
    database[count].price = get_int_input();

    if (database[count].price < 0) {
        printf("Ошибка: цена не может быть отрицательной.\n");
        return;
    }

    printf("Сайт: ");
    if (scanf("%99s", database[count].website) != 1) {
        printf("Ошибка ввода сайта.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    count++;
    printf("\nТовар добавлен. Всего товаров: %d\n", count);
}


void search_menu() {
    if (count == 0) {
        printf("\nБаза данных пуста.\n");
        return;
    }

    int field, query_price;
    char query[WEBSITE_LEN];

    printf("\n--- ПОИСК ТОВАРА ---\n");
    printf("1. По наименованию\n");
    printf("2. По бренду\n");
    printf("3. По цене\n");
    printf("4. По сайту\n");
    printf("0. Отмена\n");
    printf("Выберите поле для поиска: ");

    field = get_int_input();

    if (field == 0) {
        printf("Поиск отменен.\n");
        return;
    }

    switch (field) {
    case 1:
        printf("Введите наименование: ");
        if (scanf("%49s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    case 2:
        printf("Введите бренд: ");
        if (scanf("%29s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    case 3:
        printf("Введите цену: ");
        query_price = get_int_input();
        break;
    case 4:
        printf("Введите сайт: ");
        if (scanf("%99s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    default:
        printf("Ошибка: некорректный выбор.\n");
        return;
    }

    int found = 0;
    printf("\n--- РЕЗУЛЬТАТЫ ПОИСКА ---\n");
    printf("%-4s %-20s %-15s %-10s %-30s\n", "№", "Наименование", "Бренд", "Цена", "Сайт");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        int match = 0;

        switch (field) {
        case 1:
            if (strstr(database[i].name, query) != NULL) {
                match = 1;
            }
            break;
        case 2:
            if (strstr(database[i].brand, query) != NULL) {
                match = 1;
            }
            break;
        case 3:
            if (database[i].price == query_price) {
                match = 1;
            }
            break;
        case 4:
            if (strstr(database[i].website, query) != NULL) {
                match = 1;
            }
            break;
        }

        if (match) {
            printf("%-4d ", i + 1);
            print_product(database[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("Совпадений не найдено.\n");
    }
    else {
        printf("\nНайдено товаров: %d\n", found);
    }
}

// Меню сортировки
void sort_menu() {
    if (count == 0) {
        printf("\nБаза данных пуста.\n");
        return;
    }

    int field;

    printf("\n--- СОРТИРОВКА ---\n");
    printf("1. По наименованию\n");
    printf("2. По бренду\n");
    printf("3. По цене\n");
    printf("4. По сайту\n");
    printf("0. Отмена\n");
    printf("Выберите поле для сортировки: ");

    field = get_int_input();

    if (field == 0) {
        printf("Сортировка отменена.\n");
        return;
    }

    if (field < 1 || field > 4) {
        printf("Ошибка: некорректный выбор.\n");
        return;
    }

    sort_data(field - 1);
    printf("Данные отсортированы.\n");
    output_screen();
}


void sort_data(int field) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int swap = 0;
            switch (field) {
            case 0:
                swap = strcmp(database[i].name, database[j].name) > 0;
                break;
            case 1:
                swap = strcmp(database[i].brand, database[j].brand) > 0;
                break;
            case 2:
                swap = database[i].price > database[j].price;
                break;
            case 3:
                swap = strcmp(database[i].website, database[j].website) > 0;
                break;
            }

            if (swap) {
                struct Product temp = database[i];
                database[i] = database[j];
                database[j] = temp;
            }
        }
    }
}


void delete_menu() {
    if (count == 0) {
        printf("\nБаза данных пуста.\n");
        return;
    }

    int field, query_price;
    char query[WEBSITE_LEN];

    printf("\n--- УДАЛЕНИЕ ТОВАРА ---\n");
    printf("1. По наименованию\n");
    printf("2. По бренду\n");
    printf("3. По цене\n");
    printf("4. По сайту\n");
    printf("5. По номеру\n");
    printf("0. Отмена\n");
    printf("Выберите критерий удаления: ");

    field = get_int_input();

    if (field == 0) {
        printf("Удаление отменено.\n");
        return;
    }

    if (field == 5) {
        printf("Введите номер товара для удаления (1-%d): ", count);
        int num = get_int_input();

        if (num < 1 || num > count) {
            printf("некорректный номер товара.\n");
            return;
        }

        for (int i = num - 1; i < count - 1; i++) {
            database[i] = database[i + 1];
        }
        count--;
        printf("Товар удален. Осталось товаров: %d\n", count);
        return;
    }

    switch (field) {
    case 1:
        printf("Введите наименование: ");
        if (scanf("%49s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    case 2:
        printf("Введите бренд: ");
        if (scanf("%29s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    case 3:
        printf("Введите цену: ");
        query_price = get_int_input();
        break;
    case 4:
        printf("Введите сайт: ");
        if (scanf("%99s", query) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        break;
    default:
        printf("Ошибка: некорректный выбор.\n");
        return;
    }

    int deleted = 0;
    int i = 0;

    while (i < count) {
        int match = 0;

        switch (field) {
        case 1:
            if (strstr(database[i].name, query) != NULL) {
                match = 1;
            }
            break;
        case 2:
            if (strstr(database[i].brand, query) != NULL) {
                match = 1;
            }
            break;
        case 3:
            if (database[i].price == query_price) {
                match = 1;
            }
            break;
        case 4:
            if (strstr(database[i].website, query) != NULL) {
                match = 1;
            }
            break;
        }

        if (match) {
            for (int j = i; j < count - 1; j++) {
                database[j] = database[j + 1];
            }
            count--;
            deleted++;
        }
        else {
            i++;
        }
    }

    if (deleted == 0) {
        printf("Совпадений не найдено. Ничего не удалено.\n");
    }
    else {
        printf("Удалено товаров: %d. Осталось: %d\n", deleted, count);
    }
}


void print_product(struct Product p) {
    printf("%-20s %-15s %-10d %-30s\n", p.name, p.brand, p.price, p.website);
}



int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    menu();
    return 0;
}