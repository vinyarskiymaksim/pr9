#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const char *user_file = "user_origin.txt";
    const char *root_copy = "root_owned_copy.txt";

    // 1. Створюємо файл від імені звичайного користувача
    FILE *f = fopen(user_file, "w");
    if (f == NULL)
    {
        perror("Помилка створення файлу");
        return EXIT_FAILURE;
    }
    fprintf(f, "Початковий вміст від користувача.\n");
    fclose(f);
    printf("Файл '%s' створено.\n", user_file);

    // 2. Копіюємо файл від імені root у домашній каталог (поточну директорію)
    // Використовуємо sudo для зміни власника копії на root
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "sudo cp %s %s", user_file, root_copy);
    printf("Виконання копіювання від імені root: %s\n", cmd);
    system(cmd);

    // 3. Спроба змінити файл від імені звичайного користувача
    printf("\n--- Спроба змінити файл '%s' ---\n", root_copy);
    f = fopen(root_copy, "a");
    if (f == NULL)
    {
        printf("Результат: Відмовлено у доступі (Permission denied). Користувач не може писати у файл root.\n");
    }
    else
    {
        fprintf(f, "Спроба запису.\n");
        fclose(f);
    }

    // 4. Спроба видалити файл за допомогою rm
    printf("\n--- Спроба видалити файл '%s' за допомогою rm ---\n", root_copy);
    snprintf(cmd, sizeof(cmd), "rm %s", root_copy);
    int status = system(cmd);

    if (status == 0)
    {
        printf("Результат: Файл успішно видалено.\n");
    }
    else
    {
        printf("Результат: Помилка при видаленні.\n");
    }

    // Очищення
    unlink(user_file);

    return EXIT_SUCCESS;
}