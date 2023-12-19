#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "cJSON.h"

void readCPUInfo(int jsonFlag, cJSON *root)
{
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL)
    {
        perror("Error opening /proc/cpuinfo");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), cpuinfo))
    {
        if (strstr(line, "model name"))
        {
            char *colon = strchr(line, ':');
            if (colon)
            {
                char *info = colon + 2;
                info[strcspn(info, "\n")] = '\0';
                printf("CPU model and type: %s\n", info);
                if (jsonFlag)
                {
                    cJSON_AddStringToObject(root, "CPU model and type", info);
                }
            }
        }
        else if (strstr(line, "cpu cores"))
        {
            char *colon = strchr(line, ':');
            if (colon)
            {
                char *info = colon + 2;
                info[strcspn(info, "\n")] = '\0';
                printf("CPU cores: %s\n", info);
                if (jsonFlag)
                {
                    cJSON_AddStringToObject(root, "CPU cores", info);
                }
            }
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "L1 cache", line);
            }
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "L2 cache", line);
            }
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion(int jsonFlag,cJSON *root)
{
    FILE *version = fopen("/proc/version", "r");
    if (version == NULL)
    {
        perror("Error opening /proc/version");
        return;
    }
    char line[256];
    if (fgets(line, sizeof(line), version))
    {
        printf("Kernel version: %s", line);
        if (jsonFlag)
        {
            cJSON_AddStringToObject(root, "Kernel version", line);
        }
    }
    fclose(version);
}

void readUptime(int jsonFlag,cJSON *root)
{
    FILE *uptime = fopen("/proc/uptime", "r");
    if (uptime == NULL)
    {
        perror("Error opening /proc/uptime");
        return;
    }
    double uptime_seconds;
    if (fscanf(uptime, "%lf", &uptime_seconds) == 1)
    {
        int days = (int)(uptime_seconds / 86400);
        int hours = (int)((uptime_seconds - days * 86400) / 3600);
        int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);

        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
        char elapsedTime[50];
        sprintf(elapsedTime, "%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
        if (jsonFlag)
        {
            cJSON_AddStringToObject(root, "Elapsed time", elapsedTime);
        }
    }
    fclose(uptime);
}

void readFileSystems(int jsonFlag,cJSON *root)
{
    FILE *filesystems = fopen("/proc/filesystems", "r");
    if (filesystems == NULL)
    {
        perror("Error opening /proc/filesystems");
        return;
    }
    char line[256];
    printf("Kernel suported filesystems:\n");
    cJSON *filesystemsArray = cJSON_CreateArray();
    while (fgets(line, sizeof(line), filesystems))
    {
        printf("%s", line);
        if (jsonFlag)
        {
            cJSON_AddItemToArray(filesystemsArray, cJSON_CreateString(line));
        }
    }
    if (jsonFlag)
    {
        cJSON_AddItemToObject(root, "Kernel suported filesystems", filesystemsArray);
    }
    fclose(filesystems);
}

void readPartitions(int jsonFlag,cJSON *root)
{
    FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts == NULL)
    {
        perror("Error opening /proc/mounts");
        return;
    }
    char line[256];
    printf("Partitions and filesystems:\n");
    cJSON *partitionsArray = cJSON_CreateArray();
    while (fgets(line, sizeof(line), mounts))
    {
        printf("%s", line);
        if (jsonFlag)
        {
            cJSON_AddItemToArray(partitionsArray, cJSON_CreateString(line));
        }
    }
    if (jsonFlag)
    {
        cJSON_AddItemToObject(root, "Partitions and filesystems", partitionsArray);
    }
    fclose(mounts);
}

void readSystemStats(int jsonFlag,cJSON *root)
{
    printf("CPU stats:\n");
    FILE *stat = fopen("/proc/stat", "r");
    if (stat == NULL)
    {
        perror("Error opening /proc/stat");
        return;
    }
    char line[256];

    while (fgets(line, sizeof(line), stat))
    {
        if (strstr(line, "cpu"))
        {
            if (line[3] == ' ')
            {
                continue;
            }
            int cpuNumber = line[3] - '0';
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long userTime = 0, systemTime = 0, idleTime = 0;
            userTime = strtoul(token, NULL, 10);
            token = strtok(NULL, " ");
            systemTime = strtoul(token, NULL, 10);
            token = strtok(NULL, " ");
            idleTime = strtoul(token, NULL, 10);
            printf("CPU%d User Time: %lu\n", cpuNumber, userTime);
            printf("CPU%d System Time: %lu\n", cpuNumber, systemTime);
            printf("CPU%d Idle Time: %lu\n", cpuNumber, idleTime);
            if (jsonFlag)
            {
                char cpuNumberString[15];
                sprintf(cpuNumberString, "CPU%d", cpuNumber);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " User Time"), (double)userTime);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " System Time"), (double)systemTime);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " Idle Time"), (double)idleTime);
            }
        }
        if (strstr(line, "ctxt"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long ctxt = strtoul(token, NULL, 10);
            printf("Context Switches: %lu\n", ctxt);
            if (jsonFlag)
            {
                cJSON_AddNumberToObject(root, "Context Switches", (double)ctxt);
            }
        }
        if (strstr(line, "processes"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long processes = strtoul(token, NULL, 10);
            printf("Processes Created: %lu\n", processes);
            if (jsonFlag)
            {
                cJSON_AddNumberToObject(root, "Processes Created", (double)processes);
            }
        }
    }
    fclose(stat);
}

void readDiskRequests(int jsonFlag,cJSON *argumentL)
{
    FILE *diskRequests = fopen("/proc/diskstats", "r");
    char line[256];
    unsigned int totalRequests = 0;
    if (diskRequests == NULL)
    {
        perror("Error abriendo /proc/diskstats");
    }
    while (fgets(line, sizeof(line), diskRequests))
    {
        unsigned int requests;
        if (sscanf(line, "%*d %*d %*s %*u %*u %u", &requests) == 1)
        {
            totalRequests += requests;
        }
    }
    fclose(diskRequests);
    printf("Disk requests: %u\n", totalRequests);
    if (jsonFlag)
    {
        cJSON_AddNumberToObject(argumentL, "Disk requests", totalRequests);
    }
}

void readMemoryInfo(int jsonFlag,cJSON *argumentL)
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL)
    {
        perror("Error opening /proc/meminfo");
        exit(1);
    }
    char line[256];
    int memoryTotal = 0;
    int memoryAvailable = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "MemTotal:"))
        {
            sscanf(line, "MemTotal: %d", &memoryTotal);
        }
        else if (strstr(line, "MemAvailable:"))
        {
            sscanf(line, "MemAvailable: %d", &memoryAvailable);
        }
    }
    fclose(file);
    printf("Available memory / total memory: %d / %d\n", memoryAvailable, memoryTotal);
    if (jsonFlag)
    {
        cJSON_AddNumberToObject(argumentL, "Available memory", memoryAvailable);
        cJSON_AddNumberToObject(argumentL, "Total memory", memoryTotal);
    }
}

void readLoadAverage(int jsonFlag,cJSON *argumentL)
{
    FILE *file = fopen("/proc/loadavg", "r");
    if (file == NULL)
    {
        perror("Error opening /proc/loadavg");
        exit(1);
    }
    float loadAverage = 0.0;
    fscanf(file, "%f", &loadAverage);
    fclose(file);
    printf("Load average in last minute: %.2f\n", loadAverage);
    if (jsonFlag)
    {
        cJSON_AddNumberToObject(argumentL, "Load average in last minute", loadAverage);
    }
}

void readProcessFileDescriptors(char *pid, int jsonFlag,cJSON *root)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/fd", pid);
    printf("File descriptors for process %s:\n", pid);
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char fdPath[270];
        snprintf(fdPath, sizeof(fdPath), "/proc/%s/fd/%s", pid, entry->d_name);
        struct stat st;
        if (stat(fdPath, &st) == -1)
        {
            perror("Error getting file status");
            continue;
        }

        printf("%o ", st.st_mode & 0777);

        if (S_ISREG(st.st_mode))
        {
            printf("Regular file ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Regular file", fdPath);
            }
        }
        else if (S_ISDIR(st.st_mode))
        {
            printf("Directory ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Directory", fdPath);
            }
        }
        else if (S_ISCHR(st.st_mode))
        {
            printf("Character device ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Character device", fdPath);
            }
        }
        else if (S_ISBLK(st.st_mode))
        {
            printf("Block device ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Block device", fdPath);
            }
        }
        else if (S_ISFIFO(st.st_mode))
        {
            printf("FIFO ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "FIFO", fdPath);
            }
        }
        else if (S_ISLNK(st.st_mode))
        {
            printf("Symbolic link ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Symbolic link", fdPath);
            }
        }
        else if (S_ISSOCK(st.st_mode))
        {
            printf("Socket ");
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Socket", fdPath);
            }
        }
        else
        {
            printf("Unknown ");
        }
        char target[256];
        ssize_t len = readlink(fdPath, target, sizeof(target) - 1);
        if (len != -1)
        {
            target[len] = '\0';
            printf("%s\n", target);
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Target", target);
            }
        }
        else
        {
            printf("\n");
        }
    }
    closedir(dir);
}

void readProcessFileLimits(char *pid, int jsonFlag,cJSON *root)
{
    printf("File limits for process %s:\n", pid);
    char path[256];
    sprintf(path, "/proc/%s/limits", pid);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening limits file for process");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "Max open files") != NULL)
        {
            printf("%s", line);
            if (jsonFlag)
            {
                cJSON_AddStringToObject(root, "Max open files", line);
            }
            break;
        }
    }
    fclose(file);
}

void printKernelStackTrace(char *pid, int jsonFlag,cJSON *root)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/stack", pid);
    printf("Kernel stack trace for process %s:\n", pid);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    char line[256];
    cJSON *stackTraceArray = cJSON_CreateArray();
    while (fgets(line, sizeof(line), file))
    {
        char *symbol = strtok(line, " ");
        if (symbol != NULL)
        {
            printf("%s\n", symbol);
            if (jsonFlag)
            {
                cJSON_AddItemToArray(stackTraceArray, cJSON_CreateString(symbol));
            }
        }
    }
    if (jsonFlag)
    {
        cJSON_AddItemToObject(root, "Kernel stack trace", stackTraceArray);
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    cJSON *root = cJSON_CreateObject();
    cJSON *argumentL = cJSON_CreateObject();
    char machineName[256];
    time_t currentTime;
    struct tm *localTime;
    if (gethostname(machineName, sizeof(machineName)) != 0)
    {
        perror("Error in machine name");
        return 1;
    }
    currentTime = time(NULL);
    localTime = localtime(&currentTime);
    printf("Machine name: %s\n", machineName);
    printf("Current date and time: %s", asctime(localTime));
    int showStats = 0;
    int opt;
    unsigned int interval = 0;
    unsigned int duration = 0;
    int jsonFlag = 0;
    while ((opt = getopt(argc, argv, "sl:p:f:t:j")) != -1)
    {
        switch (opt)
        {
        case 's':
            showStats = 1;
            break;
        case 'l':
            interval = (unsigned int)atoi(optarg);
            duration = (unsigned int)atoi(argv[optind]);
            break;
        case 'p':
            readProcessFileDescriptors(argv[2], jsonFlag, root);
            break;
        case 'f':
            readProcessFileLimits(argv[2], jsonFlag, root);
            break;
        case 't':
            printKernelStackTrace(optarg, jsonFlag, root);
            break;
        case 'j':
            jsonFlag = 1;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }
    if (jsonFlag)
    {
        cJSON_AddStringToObject(root, "Machine Name", machineName);
        cJSON_AddStringToObject(root, "Date", asctime(localTime));
    }
    readCPUInfo(jsonFlag, root);
    readKernelVersion(jsonFlag, root);
    readUptime(jsonFlag, root);
    readFileSystems(jsonFlag, root);
    readPartitions(jsonFlag, root);
    if (showStats)
    {
        printf("System stats:\n");
        readSystemStats(jsonFlag,root);
    }
    if (interval > 0 && duration > 0)
    {
        unsigned int elapsed_time = 0;
        while (elapsed_time < duration)
        {
            readDiskRequests(jsonFlag,argumentL);
            readMemoryInfo(jsonFlag,argumentL);
            readLoadAverage(jsonFlag,argumentL);
            sleep(interval);
            elapsed_time += interval;
        }
    }
    if (jsonFlag)
    {
        char *json = cJSON_Print(root);
        printf("%s\n", json);
        free(json);
        cJSON_Delete(root);
        char *argumentLString = cJSON_Print(argumentL);
        printf("%s\n", argumentLString);
        free(argumentLString);
        cJSON_Delete(argumentL);
    }
    return (0);
}