#include <stdio.h>
#include <time.h>

void log_event(const char *event_type, const char *message) {
    FILE *log_file = fopen("server.log", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (tm_info == NULL) {
        perror("Failed to get local time");
        fclose(log_file);
        return;
    }

    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_file, "[%s] [%s] %s\n", timestamp, event_type, message);
    printf("[%s] [%s] %s\n", timestamp, event_type, message);
    fclose(log_file);
}