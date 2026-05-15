#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    FILE *fp;
    char line[1024];
    uid_t my_uid = getuid(); // Отримуємо UID поточного користувача
    int found_others = 0;
    const int UID_THRESHOLD = 1000; // Поріг для звичайних користувачів

    // Відкриваємо потік для читання результату команди
    fp = popen("getent passwd", "r");
    if (fp == NULL)
    {
        perror("Помилка при виконанні getent");
        return EXIT_FAILURE;
    }

    printf("Аналіз користувачів системи (UID >= %d):\n", UID_THRESHOLD);
    printf("--------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        char *username, *uid_str;
        char *saveptr;

        // Копіюємо рядок, оскільки strtok змінює оригінал
        char *line_copy = strdup(line);

        // Формат passwd: username:password:UID:GID:info:home:shell
        username = strtok_r(line_copy, ":", &saveptr); // username
        strtok_r(NULL, ":", &saveptr);                 // skip password
        uid_str = strtok_r(NULL, ":", &saveptr);       // UID

        if (uid_str != NULL)
        {
            int uid = atoi(uid_str);

            // Перевіряємо, чи є користувач звичайним та чи не є він нами
            if (uid >= UID_THRESHOLD)
            {
                if (uid == (int)my_uid)
                {
                    printf("[ПОТОЧНИЙ] Користувач: %-15s | UID: %d\n", username, uid);
                }
                else
                {
                    printf("[ЗНАЙДЕНО]  Користувач: %-15s | UID: %d\n", username, uid);
                    found_others++;
                }
            }
        }
        free(line_copy);
    }

    printf("--------------------------------------------------\n");
    if (found_others > 0)
    {
        printf("Результат: Знайдено %d інших звичайних користувачів.\n", found_others);
    }
    else
    {
        printf("Результат: Інших звичайних користувачів не знайдено.\n");
    }

    pclose(fp);
    return EXIT_SUCCESS;
}