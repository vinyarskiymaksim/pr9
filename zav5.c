#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void test_access(const char *filename)
{
    printf("Перевірка доступу до '%s':\n", filename);

    // Спроба читання
    FILE *f = fopen(filename, "r");
    if (f)
    {
        printf("  - Читання: УСПІШНО\n");
        fclose(f);
    }
    else
    {
        printf("  - Читання: ВІДМОВЛЕНО\n");
    }

    // Спроба запису
    f = fopen(filename, "a");
    if (f)
    {
        printf("  - Запис:   УСПІШНО\n");
        fclose(f);
    }
    else
    {
        printf("  - Запис:   ВІДМОВЛЕНО\n");
    }
}

int main()
{
    const char *filename = "temp_security_test.txt";
    char cmd[256];

    // 1. Створення файлу від імені звичайного користувача
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("Помилка створення файлу");
        return EXIT_FAILURE;
    }
    fprintf(f, "Початкові дані користувача.\n");
    fclose(f);
    printf("--- Файл створено користувачем ---\n");
    test_access(filename);

    // 2. Зміна власника на root (вимагає sudo)
    printf("\n--- Зміна власника на root (chown) ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chown root %s", filename);
    system(cmd);

    // 3. Зміна прав: лише для root (rw-------)
    printf("--- Встановлення прав 600 (лише root) ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 600 %s", filename);
    system(cmd);
    test_access(filename);

    // 4. Зміна прав: читання/запис для всіх (rw-rw-rw-)
    printf("\n--- Встановлення прав 666 (доступ для всіх) ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 666 %s", filename);
    system(cmd);
    test_access(filename);

    // Очищення (вимагає sudo, бо власник root)
    snprintf(cmd, sizeof(cmd), "sudo rm %s", filename);
    system(cmd);

    return EXIT_SUCCESS;
}