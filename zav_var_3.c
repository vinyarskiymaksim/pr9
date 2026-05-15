#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    const char *filename = "no_permissions.txt";
    const char *secret = "Цей текст було зчитано попри права 000!";
    char buffer[128] = {0};
    int fd;

    // 1. Створюємо файл і записуємо дані
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Помилка створення файлу");
        return EXIT_FAILURE;
    }
    write(fd, secret, strlen(secret));

    // 2. Знімаємо ВСІ права доступу (chmod 000)
    printf("Встановлюємо права 000 на файл '%s'...\n", filename);
    if (fchmod(fd, 0) == -1)
    {
        perror("Помилка chmod");
        close(fd);
        return EXIT_FAILURE;
    }

    // 3. Спроба відкрити файл знову (має не вдатися для звичайного користувача)
    int fd_new = open(filename, O_RDONLY);
    if (fd_new == -1)
    {
        printf("Нова спроба open(): ВІДМОВЛЕНО (очікувано)\n");
    }
    else
    {
        printf("Нова спроба open(): УСПІШНО (можливо, ви root?)\n");
        close(fd_new);
    }

    // 4. Читання через СТАРИЙ дескриптор (має спрацювати!)
    lseek(fd, 0, SEEK_SET);
    if (read(fd, buffer, sizeof(buffer)) > 0)
    {
        printf("Читання через старий дескриптор: %s\n", buffer);
    }
    else
    {
        perror("Помилка читання");
    }

    close(fd);
    unlink(filename); // Очищення
    return EXIT_SUCCESS;
}