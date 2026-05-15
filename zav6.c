#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void try_op(const char *label, const char *cmd)
{
    printf("\n--- Спроба: %s ---\n", label);
    int res = system(cmd);
    if (res != 0)
    {
        printf("Результат: Операцію відхилено або виникла помилка (Код: %d).\n", res);
    }
    else
    {
        printf("Результат: Операція пройшла успішно.\n");
    }
}

int main()
{
    // 1. Перегляд прав доступу у важливих директоріях [cite: 104]
    printf("Перегляд прав доступу (ls -l):\n");
    system("ls -ld ~ /usr/bin /etc | head -n 20");

    // 2. Спроба читання захищеного файлу в /etc [cite: 105]
    try_op("Читання /etc/shadow", "cat /etc/shadow 2>/dev/null");

    // 3. Спроба запису в системний файл у /usr/bin [cite: 105]
    try_op("Запис у /usr/bin/ls", "echo 'hack' >> /usr/bin/ls 2>/dev/null");

    // 4. Демонстрація обходу прав на виконання (Theory bypass)
    printf("\n--- Тестування обходу прав на виконання ---\n");
    system("echo 'echo \"Скрипт працює без права +x!\"' > noperm.sh");
    system("chmod -x noperm.sh"); // Видаляємо права на виконання [cite: 69]

    printf("Прямий запуск (./noperm.sh): ");
    fflush(stdout);
    system("./noperm.sh 2>&1"); // Очікуємо Permission denied [cite: 74, 76]

    printf("Запуск через bash (bash noperm.sh): ");
    fflush(stdout);
    system("bash noperm.sh"); // Має спрацювати [cite: 60, 70, 72]

    // Очищення
    unlink("noperm.sh");

    return 0;
}