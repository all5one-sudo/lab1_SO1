# Trabajo Práctico 1: Sistemas Operativos 1
## Introducción
En este archivo de markdown se irá documentando el desarrollo del primer trabajo práctico de la materia Sistemas Operativos 1. Este repositorio corresponde al alumno Villar Federico Ignacio, de la carrera Ingeniería en Computación e Ingeniería Electrónica. Por separado se mostrarán las consignas y los resultados obtenidos, así como un pequeño marco teórico.
## Herramientas utilizadas
Para el desarrollo de este trabajo práctico, se cuenta con las siguientes herramientas:
- Ubuntu 22.04 LTS
- Oracle VirtualBox
- UTM
- Visual Studio Code
## Desarrollo
### Step A: rdproc, versión inicial
#### Consigna
Crear la siguiente aplicación, llamada **rdproc**.
Escriba una versión inicial del programa que inspeccione las variables del kernel a través de **/proc** e informe por stdout:
- Tipo y modelo de CPU, número de cores, caché L1 y L2
- Versión de Kernel
- Tiempo transcurrido desde que se inició el sistema operativo, con el formato dd:hh:mm:ss.
- Lista de filesystems soportados por el kernel.
- Lista de particiones con su correspondiente tipo de filesystems (ej: /ext4)

También se pide incluir donde se indique el nombre de la máquina y la fecha y hora actual.
#### Resolución
El código implementado es el siguiente:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void readCPUInfo()
{
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL)
    {
        perror("Error in /proc/cpuinfo");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), cpuinfo))
    {
        if (strstr(line, "model name"))
        {
            printf("CPU model and type: %s", line);
        }
        else if (strstr(line, "cpu cores"))
        {
            printf("CPU cores: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion()
{
    FILE *version = fopen("/proc/version", "r");
    if (version == NULL)
    {
        perror("Error in /proc/version");
        return;
    }
    char line[256];
    if (fgets(line, sizeof(line), version))
    {
        printf("Kernel version: %s", line);
    }
    fclose(version);
}

void readUptime()
{
    FILE *uptime = fopen("/proc/uptime", "r");
    if (uptime == NULL)
    {
        perror("Error in /proc/uptime");
        return;
    }
    double uptime_seconds;
    if (fscanf(uptime, "%lf", &uptime_seconds) == 1)
    {
        unsigned int days = (int)(uptime_seconds / 86400);
        unsigned int hours = (int)((uptime_seconds - days * 86400) / 3600);
        unsigned int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        unsigned int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);
        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
    }
    fclose(uptime);
}

void readFileSystems()
{
    FILE *filesystems = fopen("/proc/filesystems", "r");
    if (filesystems == NULL)
    {
        perror("Error in /proc/filesystems");
        return;
    }
    char line[256];
    printf("Kernel suported filesystems:\n");
    while (fgets(line, sizeof(line), filesystems))
    {
        printf("%s", line);
    }
    fclose(filesystems);
}

void readPartitions()
{
    FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts == NULL)
    {
        perror("Error in /proc/mounts");
        return;
    }
    char line[256];
    printf("Partitions and filesystems:\n");
    while (fgets(line, sizeof(line), mounts))
    {
        printf("%s", line);
    }
    fclose(mounts);
}

int main()
{
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
    printf("Current date and time: %s\n", asctime(localTime));
    readCPUInfo();
    readKernelVersion();
    readUptime();
    readFileSystems();
    readPartitions();
    return (0);
}
```
Cuya salida por terminal es la siguiente:
```
Machine name: fedev-VirtualBox
Current date and time: Thu Sep 21 13:33:29 2023
CPU model and type: model name  : Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: cpu cores    : 2
CPU model and type: model name  : Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: cpu cores    : 2
Kernel version: Linux version 6.2.0-33-generic (buildd@lcy02-amd64-073) (x86_64-linux-gnu-gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #33~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Thu Sep  7 10:33:52 UTC 2
Elapsed time: 00:00:05:15
Kernel suported filesystems:
nodev   sysfs
nodev   tmpfs
nodev   bdev
nodev   proc
nodev   cgroup
nodev   cgroup2
nodev   cpuset
nodev   devtmpfs
nodev   configfs
nodev   debugfs
nodev   tracefs
nodev   securityfs
nodev   sockfs
nodev   bpf
nodev   pipefs
nodev   ramfs
nodev   hugetlbfs
nodev   devpts
        ext3
        ext2
        ext4
        squashfs
        vfat
nodev   ecryptfs
        fuseblk
nodev   fuse
nodev   fusectl
nodev   mqueue
nodev   pstore
nodev   autofs
nodev   binfmt_misc
Partitions and filesystems:
sysfs /sys sysfs rw,nosuid,nodev,noexec,relatime 0 0
proc /proc proc rw,nosuid,nodev,noexec,relatime 0 0
udev /dev devtmpfs rw,nosuid,relatime,size=4023836k,nr_inodes=1005959,mode=755,inode64 0 0
devpts /dev/pts devpts rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000 0 0
tmpfs /run tmpfs rw,nosuid,nodev,noexec,relatime,size=812408k,mode=755,inode64 0 0
/dev/sda3 / ext4 rw,relatime,errors=remount-ro 0 0
securityfs /sys/kernel/security securityfs rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /dev/shm tmpfs rw,nosuid,nodev,inode64 0 0
tmpfs /run/lock tmpfs rw,nosuid,nodev,noexec,relatime,size=5120k,inode64 0 0
cgroup2 /sys/fs/cgroup cgroup2 rw,nosuid,nodev,noexec,relatime,nsdelegate,memory_recursiveprot 0 0
pstore /sys/fs/pstore pstore rw,nosuid,nodev,noexec,relatime 0 0
bpf /sys/fs/bpf bpf rw,nosuid,nodev,noexec,relatime,mode=700 0 0
systemd-1 /proc/sys/fs/binfmt_misc autofs rw,relatime,fd=29,pgrp=1,timeout=0,minproto=5,maxproto=5,direct,pipe_ino=16491 0 0
debugfs /sys/kernel/debug debugfs rw,nosuid,nodev,noexec,relatime 0 0
mqueue /dev/mqueue mqueue rw,nosuid,nodev,noexec,relatime 0 0
tracefs /sys/kernel/tracing tracefs rw,nosuid,nodev,noexec,relatime 0 0
hugetlbfs /dev/hugepages hugetlbfs rw,relatime,pagesize=2M 0 0
fusectl /sys/fs/fuse/connections fusectl rw,nosuid,nodev,noexec,relatime 0 0
configfs /sys/kernel/config configfs rw,nosuid,nodev,noexec,relatime 0 0
ramfs /run/credentials/systemd-sysusers.service ramfs ro,nosuid,nodev,noexec,relatime,mode=700 0 0
/dev/loop0 /snap/bare/5 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop1 /snap/code/140 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop2 /snap/core20/1974 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop3 /snap/core22/858 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop4 /snap/firefox/2987 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop5 /snap/gnome-42-2204/120 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop7 /snap/snap-store/959 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop6 /snap/gtk-common-themes/1535 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop8 /snap/snapd-desktop-integration/83 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop9 /snap/snapd/19457 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop10 /snap/telegram-desktop/5062 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/sda3 /var/snap/firefox/common/host-hunspell ext4 ro,noexec,noatime,errors=remount-ro 0 0
/dev/loop11 /snap/gnome-3-38-2004/143 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/sda2 /boot/efi vfat rw,relatime,fmask=0077,dmask=0077,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0 0
binfmt_misc /proc/sys/fs/binfmt_misc binfmt_misc rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /run/snapd/ns tmpfs rw,nosuid,nodev,noexec,relatime,size=812408k,mode=755,inode64 0 0
nsfs /run/snapd/ns/snapd-desktop-integration.mnt nsfs rw 0 0
tmpfs /run/user/1000 tmpfs rw,nosuid,nodev,relatime,size=812404k,nr_inodes=203101,mode=700,uid=1000,gid=1000,inode64 0 0
gvfsd-fuse /run/user/1000/gvfs fuse.gvfsd-fuse rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
portal /run/user/1000/doc fuse.portal rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
nsfs /run/snapd/ns/snap-store.mnt nsfs rw 0 0
nsfs /run/snapd/ns/telegram-desktop.mnt nsfs rw 0 0
```
##### Explicación de las funciones implementadas
- `void readCPUInfo()`: en esta función, se abre el archivo `/proc/cpuinfo` en modo lectura usando la función `fopen`. Este archivo contiene información sobre CPU y otros aspectos del hardware. Es accesible en sistemas Linux, fue probado en una computadora MacBook Air M1, pero esta vez sin éxito, por lo que se terminó ejecutando en una máquina virtual en una PC de escritorio. Dentro de la función se llama a `fgets` para leer líneas del archivo e ir parseando de acuerdo a la información requerida dentro de la consigna del mismo (esto se hace para el CPU Model Name y para la caché L1 y L2). Una vez finalizada la tarea, se hace uso de la función `fclose` que cierra el archivo.
- `void readKernelVersion()`: se abre el archivo `/proc/version` en modo lectura, y, al igual que antes se verifica que se pueda abrir antes de imprimir. Al leer este archivo se obtiene información de la versión del Kernel de Linux utilizado.
- `void readUptime()`: nuevamente se lee un archivo de `/proc`, en este caso `/proc/uptime` en modo lectura. Se usa la función `fscanf` para leer un valor decimal de punto flotante desde el archivo para poder devolver el tiempo, luego se convierte a tiempo en el formato solicitado por medio de código.
- `void readFileSystems()`: abre el archivo mostrado en la consigna, el encabezado impreso indica que se va a mostrar una lista de filesystems soportados por el Kernel. Los tipos de sistemas de archivos mostrados en el Log son los siguientes:
	- sysfs
	- tmpfs
	- bdev
	- proc
	- cgroup
	- cgroup2
	- cpuset
	- devtmpfs
	- configfs
	- debugfs
	- tracefs
	- securityfs
	- sockfs
	- bpf
	- pipefs
	- ramfs
	- hugetlbfs
	- devpts
	- ext3
	- ext2
	- ext4
	- squashfs
	- vfat
	- ecryptfs
	- fuseblk
	- fuse
	- fusectl
	- mqueue
	- pstore
	- autofs
	- binfmt_misc
- `void readPartitions()`: abre el archivo `/proc/mounts`. Este archivo contiene información relevante sobre las particiones montadas y los sistemas de archivos correspondientes a cada una. El bucle se encarga de ir parseando y encontrando las distintas particiones dentro de las líneas del archivo abierto.
- `int main()`: en el main se llama a cada una de las funciones anteriormente declaradas, pero como cabecera se imprime también el nombre de la máquina. Esta acción requiere de la invocación a la función `gethostname()`, la cual puede llegar a devolver un error, es por eso que se la trabaja dentro de un condicional `if`.

Este primer programa, se ejecutó en consola, con el comando `gcc rdproc.c -o rdprocA`, de forma de obtener un ejecutable con el nombre `rdprocA`, con el fin de que para cada versión compilada se tenga un ejecutable distinto, y poder ver la evolución del programa principal a través de las versiones.
### Step B: Command line arguments
#### Consigna
Escriba una segunda versión del programa que imprima la misma información que la versión por defecto, pero en caso de invocarse con la **opción -s**, agrega la siguientes estadísticas del sistema:
- Cantidad de tiempo de CPU utilizado para usuarios, sistema y proceso idle.
- Cantidad de cambios de contexto.
- Número de procesos creados desde el inicio del sistema.

#### Resolución
Se implementó el siguiente código:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void readCPUInfo()
{
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL)
    {
        perror("Error in /proc/cpuinfo");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), cpuinfo))
    {
        if (strstr(line, "model name"))
        {
            printf("CPU model and type: %s", line);
        }
        else if (strstr(line, "cpu cores"))
        {
            printf("CPU cores: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion()
{
    FILE *version = fopen("/proc/version", "r");
    if (version == NULL)
    {
        perror("Error in /proc/version");
        return;
    }
    char line[256];
    if (fgets(line, sizeof(line), version))
    {
        printf("Kernel version: %s", line);
    }
    fclose(version);
}

void readUptime()
{
    FILE *uptime = fopen("/proc/uptime", "r");
    if (uptime == NULL)
    {
        perror("Error in /proc/uptime");
        return;
    }
    double uptime_seconds;
    if (fscanf(uptime, "%lf", &uptime_seconds) == 1)
    {
        unsigned int days = (int)(uptime_seconds / 86400);
        unsigned int hours = (int)((uptime_seconds - days * 86400) / 3600);
        unsigned int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        unsigned int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);
        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
    }
    fclose(uptime);
}

void readFileSystems()
{
    FILE *filesystems = fopen("/proc/filesystems", "r");
    if (filesystems == NULL)
    {
        perror("Error in /proc/filesystems");
        return;
    }
    char line[256];
    printf("Kernel suported filesystems:\n");
    while (fgets(line, sizeof(line), filesystems))
    {
        printf("%s", line);
    }
    fclose(filesystems);
}

void readPartitions()
{
    FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts == NULL)
    {
        perror("Error in /proc/mounts");
        return;
    }
    char line[256];
    printf("Partitions and filesystems:\n");
    while (fgets(line, sizeof(line), mounts))
    {
        printf("%s", line);
    }
    fclose(mounts);
}

void readSystemStats()
{
    FILE *stat = fopen("/proc/stat", "r");
    if (stat == NULL)
    {
        perror("Error in /proc/stat");
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
            int cpu_number = line[3] - '0';
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long user_time = 0, system_time = 0, idle_time = 0;
            user_time = strtoul(token, NULL, 10);
            token = strtok(NULL, " ");
            system_time = strtoul(token, NULL, 10);
            token = strtok(NULL, " ");
            idle_time = strtoul(token, NULL, 10);
            printf("CPU%d User Time: %lu\n", cpu_number, user_time);
            printf("CPU%d System Time: %lu\n", cpu_number, system_time);
            printf("CPU%d Idle Time: %lu\n", cpu_number, idle_time);
        }
        if (strstr(line, "ctxt"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long ctxt = strtoul(token, NULL, 10);
            printf("Context Switches: %lu\n", ctxt);
        }
        if (strstr(line, "processes"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long processes = strtoul(token, NULL, 10);
            printf("Processes Created: %lu\n", processes);
        }
    }
    fclose(stat);
}

int main(int argc, char *argv[])
{
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
    while ((opt = getopt(argc, argv, "s")) != -1)
    {
        switch (opt)
        {
        case 's':
            showStats = 1;
            break;
        default:
            fprintf(stderr, "Use: %s [-s]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (showStats)
    {
        printf("System stats:\n");
        readSystemStats();
    }
    readCPUInfo();
    readKernelVersion();
    readUptime();
    readFileSystems();
    readPartitions();
    return (0);
}
```
Se produjo la siguiente salida:
```
Machine name: fedev-VirtualBox
Current date and time: Thu Sep 21 16:32:33 2023
System stats:
CPU0 User Time: 61254
CPU0 System Time: 266
CPU0 Idle Time: 27335
CPU1 User Time: 29341
CPU1 System Time: 35
CPU1 Idle Time: 52239
Context Switches: 8044058
Processes Created: 10481
CPU model and type: model name  : Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: cpu cores    : 2
CPU model and type: model name  : Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: cpu cores    : 2
Kernel version: Linux version 6.2.0-33-generic (buildd@lcy02-amd64-073) (x86_64-linux-gnu-gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #33~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Thu Sep  7 10:33:52 UTC 2
Elapsed time: 00:03:04:19
Kernel suported filesystems:
nodev   sysfs
nodev   tmpfs
nodev   bdev
nodev   proc
nodev   cgroup
nodev   cgroup2
nodev   cpuset
nodev   devtmpfs
nodev   configfs
nodev   debugfs
nodev   tracefs
nodev   securityfs
nodev   sockfs
nodev   bpf
nodev   pipefs
nodev   ramfs
nodev   hugetlbfs
nodev   devpts
        ext3
        ext2
        ext4
        squashfs
        vfat
nodev   ecryptfs
        fuseblk
nodev   fuse
nodev   fusectl
nodev   mqueue
nodev   pstore
nodev   autofs
nodev   binfmt_misc
Partitions and filesystems:
sysfs /sys sysfs rw,nosuid,nodev,noexec,relatime 0 0
proc /proc proc rw,nosuid,nodev,noexec,relatime 0 0
udev /dev devtmpfs rw,nosuid,relatime,size=4023836k,nr_inodes=1005959,mode=755,inode64 0 0
devpts /dev/pts devpts rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000 0 0
tmpfs /run tmpfs rw,nosuid,nodev,noexec,relatime,size=812408k,mode=755,inode64 0 0
/dev/sda3 / ext4 rw,relatime,errors=remount-ro 0 0
securityfs /sys/kernel/security securityfs rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /dev/shm tmpfs rw,nosuid,nodev,inode64 0 0
tmpfs /run/lock tmpfs rw,nosuid,nodev,noexec,relatime,size=5120k,inode64 0 0
cgroup2 /sys/fs/cgroup cgroup2 rw,nosuid,nodev,noexec,relatime,nsdelegate,memory_recursiveprot 0 0
pstore /sys/fs/pstore pstore rw,nosuid,nodev,noexec,relatime 0 0
bpf /sys/fs/bpf bpf rw,nosuid,nodev,noexec,relatime,mode=700 0 0
systemd-1 /proc/sys/fs/binfmt_misc autofs rw,relatime,fd=29,pgrp=1,timeout=0,minproto=5,maxproto=5,direct,pipe_ino=16491 0 0
debugfs /sys/kernel/debug debugfs rw,nosuid,nodev,noexec,relatime 0 0
mqueue /dev/mqueue mqueue rw,nosuid,nodev,noexec,relatime 0 0
tracefs /sys/kernel/tracing tracefs rw,nosuid,nodev,noexec,relatime 0 0
hugetlbfs /dev/hugepages hugetlbfs rw,relatime,pagesize=2M 0 0
fusectl /sys/fs/fuse/connections fusectl rw,nosuid,nodev,noexec,relatime 0 0
configfs /sys/kernel/config configfs rw,nosuid,nodev,noexec,relatime 0 0
ramfs /run/credentials/systemd-sysusers.service ramfs ro,nosuid,nodev,noexec,relatime,mode=700 0 0
/dev/loop0 /snap/bare/5 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop1 /snap/code/140 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop2 /snap/core20/1974 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop3 /snap/core22/858 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop4 /snap/firefox/2987 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop5 /snap/gnome-42-2204/120 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop7 /snap/snap-store/959 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop6 /snap/gtk-common-themes/1535 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop8 /snap/snapd-desktop-integration/83 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop9 /snap/snapd/19457 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop10 /snap/telegram-desktop/5062 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/sda3 /var/snap/firefox/common/host-hunspell ext4 ro,noexec,noatime,errors=remount-ro 0 0
/dev/loop11 /snap/gnome-3-38-2004/143 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/sda2 /boot/efi vfat rw,relatime,fmask=0077,dmask=0077,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0 0
binfmt_misc /proc/sys/fs/binfmt_misc binfmt_misc rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /run/snapd/ns tmpfs rw,nosuid,nodev,noexec,relatime,size=812408k,mode=755,inode64 0 0
nsfs /run/snapd/ns/snapd-desktop-integration.mnt nsfs rw 0 0
tmpfs /run/user/1000 tmpfs rw,nosuid,nodev,relatime,size=812404k,nr_inodes=203101,mode=700,uid=1000,gid=1000,inode64 0 0
gvfsd-fuse /run/user/1000/gvfs fuse.gvfsd-fuse rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
portal /run/user/1000/doc fuse.portal rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
nsfs /run/snapd/ns/snap-store.mnt nsfs rw 0 0
nsfs /run/snapd/ns/telegram-desktop.mnt nsfs rw 0 0
nsfs /run/snapd/ns/firefox.mnt nsfs rw 0 0
/dev/loop12 /snap/snapd/20092 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop13 /snap/core20/2015 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop14 /snap/core22/864 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
/dev/loop15 /snap/gnome-42-2204/132 squashfs ro,nodev,relatime,errors=continue,threads=single 0 0
```
##### Explicación
Este programa, con respecto al anterior, agrega una función nueva, llamada `readSystemStats()`, que se encarga de abrir el archivo `/proc/stat`, y de allí toma información del tiempo de CPU. En el caso de la impresión por consola, se muestran 2 CPU porque se le dieron 2 cores a la máquina virtual. Al inicio del bucle que parsea la información, se coloca un condicional que saltea la línea 3, de modo de no mostrar la información total del CPU. Al inicio, cuando se armó esta función, mostraba 3 CPU, en donde aparecían los dos núcleos, y un último que resultaba del CPU total, mostrando la suma de los tiempos visualizados en los otros dos cores. Luego, se accede a `ctxt` y a `processes`, que dan información sobre la cantidad de cambios de contexto y el número de procesos creados desde el inicio del sistema, respectivamente.
En la función principal, se agregó un while, junto con una variable que puede tomar valor 0 o 1, representando un `true` o un `false`, respectivamente, que representa si desde la consola se ejecutó el programa con la opción `-s` o no, decidiendo si ejecuta o no la función `readSystemStats()`.
Para compilar, se usó el siguiente comando: `gcc rdproc.c -o rdprocB`, y para ejecutar solicitando la impresión de las estadísticas del sistema, se hace `./rdprocB -s`.
### Step C: Polling
#### Consigna
La tercera parte involucra imprimir todo lo de las versiones anterioress, pero cuando se invoca con la opción `-l <interval> <duration>` imprime además:
- Número de peticiones al disco realizadas
- Cantidad de memoria configurada en el hardware
- Cantidad de memoria disponible
- Lista de los promedios de carga de 1 minuto

Así por ejemplo `rdproc -l 2 100` mostrará el promedio de carga de 1 minuto por 100 segundos tomando muestras en intervalos de 2 segundos. El comando `rdproc -l a b`, lee los datos indicados arriba (peticiones de disco, cantidades de memoria, promedios de carga) de `/proc`, y los imprime repetidamente cada a segundos, esto se repite hasta que hayan pasado b segundos. Por ejemplo:
```
$ rdproc -l 5 15
Peticiones a disco: 12345
Memoria disponible / total: 1000000 / 8000000
Promedio de carga en el último minuto: 0.88
[Pausa de 5 segundos]
Peticiones a disco: 12348
Memoria disponible / total: 2000000 / 8000000
Promedio de carga en el último minuto: 0.82
[Pausa de 5 segundos]
Peticiones a disco: 12645
Memoria disponible / total: 500000 / 8000000
Promedio de carga en el último minuto: 0.98
[Pausa de 5 segundos]
Peticiones a disco: 99995
Memoria disponible / total: 300000 / 8000000
Promedio de carga en el último minuto: 1.07
```
En particular, noten que el promedio de carga de un minuto es un valor que el sistema
provee ya calculado, no es algo que ustedes tengan que calcular.
Notar que cada opción incluye a la otra, por lo que `rdproc -s -l 2 100` no debería ser aceptado
y en tales casos resulta útil imprimir por la salida estándar un resumen de las opciones
aceptadas.
#### Resolución
Se implementó el siguiente código:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

void readCPUInfo()
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
            printf("CPU model and type: %s", line);
        }
        else if (strstr(line, "cpu cores"))
        {
            printf("CPU cores: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion()
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
    }
    fclose(version);
}

void readUptime()
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
        unsigned int days = (int)(uptime_seconds / 86400);
        unsigned int hours = (int)((uptime_seconds - days * 86400) / 3600);
        unsigned int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        unsigned int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);
        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
    }
    fclose(uptime);
}

void readFileSystems()
{
    FILE *filesystems = fopen("/proc/filesystems", "r");
    if (filesystems == NULL)
    {
        perror("Error opening /proc/filesystems");
        return;
    }
    char line[256];
    printf("Kernel suported filesystems:\n");
    while (fgets(line, sizeof(line), filesystems))
    {
        printf("%s", line);
    }
    fclose(filesystems);
}

void readPartitions()
{
    FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts == NULL)
    {
        perror("Error opening /proc/mounts");
        return;
    }
    char line[256];
    printf("Partitions and filesystems:\n");
    while (fgets(line, sizeof(line), mounts))
    {
        printf("%s", line);
    }
    fclose(mounts);
}

void readSystemStats()
{
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
        }
        if (strstr(line, "ctxt"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long ctxt = strtoul(token, NULL, 10);
            printf("Context Switches: %lu\n", ctxt);
        }
        if (strstr(line, "processes"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long processes = strtoul(token, NULL, 10);
            printf("Processes Created: %lu\n", processes);
        }
    }
    fclose(stat);
}
void readDiskRequests()
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
}

void readMemoryInfo()
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
}

void readLoadAverage()
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
}

int main(int argc, char *argv[])
{
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
    int interval = 0;
    int duration = 0;
    while ((opt = getopt(argc, argv, "sl:")) != -1)
    {
        switch (opt)
        {
        case 's':
            showStats = 1;
            break;
        case 'l':
            interval = atoi(optarg);
            duration = atoi(argv[optind]);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }
    if (showStats)
    {
        printf("System stats:\n");
        readSystemStats();
    }
    if (interval > 0 && duration > 0)
    {
        int elapsed_time = 0;
        while (elapsed_time < duration)
        {
            readDiskRequests();
            readMemoryInfo();
            readLoadAverage();
            sleep(interval);
            elapsed_time += interval;
        }
    }
    readCPUInfo();
    readKernelVersion();
    readUptime();
    readFileSystems();
    readPartitions();
    return (0);
}
```

Con lo implementado, si por ejemplo se llama al programa de la siguiente forma: `./rdprocC -l 2 15`, se produce por consola la siguiente salida (recortada a lo pertinente al step C por cuestiones de simplicidad), que responde a lo solicitado en la consigna.
```
Machine name: all5one-ubuntu
Current date and time: Fri Dec  1 13:56:56 2023
Disk requests: 11014696
Available memory / total memory: 19233248 / 24502376
Load average in last minute: 0.77
Disk requests: 11014696
Available memory / total memory: 19226572 / 24502376
Load average in last minute: 0.77
Disk requests: 11014696
Available memory / total memory: 19362364 / 24502376
Load average in last minute: 0.79
Disk requests: 11014696
Available memory / total memory: 19330432 / 24502376
Load average in last minute: 0.79
Disk requests: 11014696
Available memory / total memory: 19342492 / 24502376
Load average in last minute: 1.05
Disk requests: 11014696
Available memory / total memory: 19348948 / 24502376
Load average in last minute: 1.05
Disk requests: 11014696
Available memory / total memory: 19315604 / 24502376
Load average in last minute: 1.05
Disk requests: 11014696
Available memory / total memory: 19313120 / 24502376
Load average in last minute: 0.96
```

Pueden verse alli las estadisticas de uso del disco, para un intervalo de 15 segundos, con saltos de 2 segundos. Esta informacion es la que se imprime al inicio.
### Step D: Process
#### Consigna
Agregar a rdproc, la siguiente opción `-p <pid>`. Con esta nueva opción, además, se imprime información sobre los file descriptors a los que tiene acceso un proceso en particular.
Agregar la opción `-f <pid>`, para imprimir los límites de archivos abiertos para un cierto proceso.
Finalmente, agregar la opción `-t` para imprimir el kernel stack trace actual de un cierto proceso. Imprimir solamente el nombre del símbolo.
Se tiene el siguiente ejemplo:
```
% rdproc -p 123
<rdproc previous info>
<permits> <file type> <path to fd>
...
<permits> <file type> <path to fd>
```
Como Testing se requiere:
1. Testear con un programa que abra un archivo y se quede en un loop infinito.
2. Utilizando shell:
	Primero, se puede conseguir el process ID usando el comando ps:
	`% ps aux | grep processName`
	Opcionalmente, se puede instalar y utilizar el command pidof:
	`% pidof processName`
	Luego, podemos acceder a `/proc/<id>/fd`
	Finalmente, un comando que resume esta consigna:
	`% lsof -a -p <pid>`
3. Testear la opción `-f` con el comando ‘ulimit’.

#### Resolución
Se implemento el siguiente codigo:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

void readCPUInfo()
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
            printf("CPU model and type: %s", line);
        }
        else if (strstr(line, "cpu cores"))
        {
            printf("CPU cores: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion()
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
    }
    fclose(version);
}

void readUptime()
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
        unsigned int days = (int)(uptime_seconds / 86400);
        unsigned int hours = (int)((uptime_seconds - days * 86400) / 3600);
        unsigned int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        unsigned int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);
        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
    }
    fclose(uptime);
}

void readFileSystems()
{
    FILE *filesystems = fopen("/proc/filesystems", "r");
    if (filesystems == NULL)
    {
        perror("Error opening /proc/filesystems");
        return;
    }
    char line[256];
    printf("Kernel suported filesystems:\n");
    while (fgets(line, sizeof(line), filesystems))
    {
        printf("%s", line);
    }
    fclose(filesystems);
}

void readPartitions()
{
    FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts == NULL)
    {
        perror("Error opening /proc/mounts");
        return;
    }
    char line[256];
    printf("Partitions and filesystems:\n");
    while (fgets(line, sizeof(line), mounts))
    {
        printf("%s", line);
    }
    fclose(mounts);
}

void readSystemStats()
{
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
        }
        if (strstr(line, "ctxt"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long ctxt = strtoul(token, NULL, 10);
            printf("Context Switches: %lu\n", ctxt);
        }
        if (strstr(line, "processes"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long processes = strtoul(token, NULL, 10);
            printf("Processes Created: %lu\n", processes);
        }
    }
    fclose(stat);
}
void readDiskRequests()
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
}

void readMemoryInfo()
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
}

void readLoadAverage()
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
}

void readProcessFileDescriptors(char *pid)
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
			printf("Regular file ");
		else if (S_ISDIR(st.st_mode))
			printf("Directory ");
		else if (S_ISCHR(st.st_mode))
			printf("Character device ");
		else if (S_ISBLK(st.st_mode))
			printf("Block device ");
		else if (S_ISFIFO(st.st_mode))
			printf("FIFO ");
		else if (S_ISLNK(st.st_mode))
			printf("Symbolic link ");
		else if (S_ISSOCK(st.st_mode))
			printf("Socket ");
		else
			printf("Unknown ");
		char target[256];
		ssize_t len = readlink(fdPath, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			printf("%s\n", target);
		}
		else
		{
			printf("\n");
		}
	}
	closedir(dir);
}

void readProcessFileLimits(char *pid)
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
			break;
		}
	}
	fclose(file);
}

void printKernelStackTrace(char *pid)
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
	while (fgets(line, sizeof(line), file))
	{
		char *symbol = strtok(line, " ");
		if (symbol != NULL)
		{
			printf("%s\n", symbol);
		}
	}
	fclose(file);
}

int main(int argc, char *argv[])
{
	printf("\033[0;36mTrabajo Practico 1, Sistemas Operativos I, Federico I. Villar, 2023\033[0""m\n");
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
	int interval = 0;
	int duration = 0;
	readCPUInfo();
	readKernelVersion();
	readUptime();
	readFileSystems();
	readPartitions();
	while ((opt = getopt(argc, argv, "sl:p:f:t:")) != -1)
	{
		switch (opt)
		{
		case 's':
			showStats = 1;
			break;
		case 'l':
			interval = atoi(optarg);
			duration = atoi(argv[optind]);
			break;
		case 'p':
			readProcessFileDescriptors(argv[2]);
			break;
		case 'f':
			readProcessFileLimits(argv[2]);
			break;
		case 't':
			printKernelStackTrace(optarg);
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	if (showStats)
	{
		printf("System stats:\n");
		readSystemStats();
	}
	if (interval > 0 && duration > 0)
	{
		int elapsed_time = 0;
		while (elapsed_time < duration)
		{
			readDiskRequests();
			readMemoryInfo();
			readLoadAverage();
			sleep(interval);
			elapsed_time += interval;
		}
	}
	return (0);
}
```

Se solicito realizar testing de acuerdo a ciertos criterios. Para ello, se creo un archivo de texto, llamado `testFile.txt`, que esta vacio, de modo que sea ese el que se abre en un programa llamado `testD1.c`, que sigue los lineamientos planteados en la consigna. El codigo de este ultimo es el siguiente:
```c
#include <stdio.h>

int main() {
	FILE *file = fopen("testFile.txt", "r");
	if (file == NULL) {
		printf("Failed to open file\n");
	return 1;
	}
	while (1) {
	}
	fclose(file);
	return 0;
}
```
Para poder probar el programa, se tiene lo llama a traves de un script de shell, que se encarga de:
1. Ejecutar en segundo plano el programa anterior (que se encarga de abrir el archivo y quedarse en un bucle infinito).
2. Encontrar el `pid` del ultimo proceso iniciado.
3. Esperar 1 segundo.
4. Llamar a `rdproc` para con los 3 ultimos argumentos agregados, de forma de poder ver la salida.
5. Matar el proceso (bucle infinito).

El codigo implementado es el siguiente:

```sh
./testD1 &

pid = $!

sleep 1

./rdprocD -p $pid
./rdprocD -f $pid
./rdprocD -t $pid

kill $pid
```

Se muestra ahora, a continuacion las salidas de cada una de las llamadas (por simplicidad se recortan, mostrando de esta forma solo lo referente a los processes).
```
File descriptors for process 126635:
500 Directory 
555 Directory 
666 Character device /dev/null
620 Character device /dev/pts/2
620 Character device /dev/pts/2
664 Regular file /home/all5one/Documents/soi_tp1/soi-2023-lab1-rdproc-all5one-sudo/testFile.txt
664 Regular file /home/all5one/.config/Code/logs/20231201T143423/ptyhost.log
777 Socket socket:[736305]
664 Regular file /snap/code/146/usr/share/code/resources/app/node_modules.asar
644 Regular file /snap/code/146/usr/share/code/v8_context_snapshot.bin
```

```
File limits for process 126635:
Max open files            1048576              1048576              files
```

```
Kernel stack trace for process 126635:
```

Puede verse que en el stack trace no se imprime nada, por tratarse de un proceso de usuario y no de kernel.

Para la ultima prueba, se pide ejecutar `./rdproc -t $(ulimit -n)`, de donde se obtiene:

```
Kernel stack trace for process 1048576:
Error opening file: No such file or directory
```

Ese archivo no es accesible por encontrarse en la zona de boot.

### Step E: JSON format
#### Consigna
Este último punto consiste en agregar la opción -j, que puede ser combinada con cualquiera de los puntos anteriores, de tal manera que imprime la información en formato JSON. Por ejemplo para al llamar a rdproc -j, producirá una salida del siguiente estilo:
```json
{
	"cpu_type": "GenuineIntel",
	"cpu_model": "Intel(R) Core(TM) i7-4790K CPU @ 4.00GHz",
	"kerner_version": "Linux pegaso 5.8.8-arch1-1",
	"number_of_threads": 8
	....
	....
}
```
#### Resolución
Se implemento el siguiente codigo:
```c
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

int jsonFlag = 0;
cJSON *root;
cJSON *argumentL;

void readCPUInfo()
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
                if(jsonFlag){
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
                if(jsonFlag){
                    cJSON_AddStringToObject(root, "CPU cores", info);
                }
            }
        }
        else if (strstr(line, "cache size") && strstr(line, "L1"))
        {
            printf("L1 cache: %s", line);
            if(jsonFlag){
                cJSON_AddStringToObject(root, "L1 cache", line);
            }
        }
        else if (strstr(line, "cache size") && strstr(line, "L2"))
        {
            printf("L2 cache: %s", line);
            if(jsonFlag){
                cJSON_AddStringToObject(root, "L2 cache", line);
            }
        }
    }
    fclose(cpuinfo);
}

void readKernelVersion()
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
        if(jsonFlag){
            cJSON_AddStringToObject(root, "Kernel version", line);
        }
    }
    fclose(version);
}

void readUptime()
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
        unsigned int days = (int)(uptime_seconds / 86400);
        unsigned int hours = (int)((uptime_seconds - days * 86400) / 3600);
        unsigned int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        unsigned int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);

        printf("Elapsed time: %02d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
        char elapsedTime[50];
        sprintf(elapsedTime, "%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
        if(jsonFlag){
            cJSON_AddStringToObject(root, "Elapsed time", elapsedTime);
        }
    }
    fclose(uptime);
}

void readFileSystems()
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
        if(jsonFlag){
            cJSON_AddItemToArray(filesystemsArray, cJSON_CreateString(line));
        }
    }
    if(jsonFlag){
        cJSON_AddItemToObject(root, "Kernel suported filesystems", filesystemsArray);
    }
    fclose(filesystems);
}

void readPartitions()
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
        if(jsonFlag){
            cJSON_AddItemToArray(partitionsArray, cJSON_CreateString(line));
        }
    }
    if(jsonFlag){
        cJSON_AddItemToObject(root, "Partitions and filesystems", partitionsArray);
    }
    fclose(mounts);
}

void readSystemStats()
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
            if(jsonFlag){
                char cpuNumberString[15];
                sprintf(cpuNumberString, "CPU%d", cpuNumber);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " User Time"), userTime);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " System Time"), systemTime);
                cJSON_AddNumberToObject(root, strcat(cpuNumberString, " Idle Time"), idleTime);
            }
        }
        if (strstr(line, "ctxt"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long ctxt = strtoul(token, NULL, 10);
            printf("Context Switches: %lu\n", ctxt);
            if(jsonFlag){
                cJSON_AddNumberToObject(root, "Context Switches", ctxt);
            }
        }
        if (strstr(line, "processes"))
        {
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            unsigned long processes = strtoul(token, NULL, 10);
            printf("Processes Created: %lu\n", processes);
            if(jsonFlag){
                cJSON_AddNumberToObject(root, "Processes Created", processes);
            }
        }
    }
    fclose(stat);
}

void readDiskRequests()
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
    if(jsonFlag){
        cJSON_AddNumberToObject(argumentL, "Disk requests", totalRequests);
    }
}

void readMemoryInfo()
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
    if(jsonFlag){
        cJSON_AddNumberToObject(argumentL, "Available memory", memoryAvailable);
        cJSON_AddNumberToObject(argumentL, "Total memory", memoryTotal);
    }
}

void readLoadAverage()
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
    if(jsonFlag){
        cJSON_AddNumberToObject(argumentL, "Load average in last minute", loadAverage);
    }
}

void readProcessFileDescriptors(char *pid)
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
            printf("Regular file ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Regular file", fdPath);
            }
        else if (S_ISDIR(st.st_mode))
            printf("Directory ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Directory", fdPath);
            }
        else if (S_ISCHR(st.st_mode))
            printf("Character device ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Character device", fdPath);
            }
        else if (S_ISBLK(st.st_mode))
            printf("Block device ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Block device", fdPath);
            }
        else if (S_ISFIFO(st.st_mode))
            printf("FIFO ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "FIFO", fdPath);
            }
        else if (S_ISLNK(st.st_mode))
            printf("Symbolic link ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Symbolic link", fdPath);
            }
        else if (S_ISSOCK(st.st_mode))
            printf("Socket ");
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Socket", fdPath);
            }
        else
            printf("Unknown ");
        char target[256];
        ssize_t len = readlink(fdPath, target, sizeof(target) - 1);
        if (len != -1)
        {
            target[len] = '\0';
            printf("%s\n", target);
            if(jsonFlag){
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

void readProcessFileLimits(char *pid)
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
            if(jsonFlag){
                cJSON_AddStringToObject(root, "Max open files", line);
            }
            break;
        }
    }
    fclose(file);
}

void printKernelStackTrace(char *pid)
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
            if(jsonFlag){
                cJSON_AddItemToArray(stackTraceArray, cJSON_CreateString(symbol));
            }
        }
    }
    if(jsonFlag){
        cJSON_AddItemToObject(root, "Kernel stack trace", stackTraceArray);
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
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
    int interval = 0;
    int duration = 0;
    while ((opt = getopt(argc, argv, "sl:p:f:t:j")) != -1)
    {
        switch (opt)
        {
        case 's':
            showStats = 1;
            break;
        case 'l':
            interval = atoi(optarg);
            duration = atoi(argv[optind]);
            break;
        case 'p':
            readProcessFileDescriptors(argv[2]);
            break;
        case 'f':
            readProcessFileLimits(argv[2]);
            break;
        case 't':
            printKernelStackTrace(optarg);
            break;
        case 'j':
            jsonFlag = 1;
            root = cJSON_CreateObject();
            argumentL = cJSON_CreateObject();
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
    readCPUInfo();
    readKernelVersion();
    readUptime();
    readFileSystems();
    readPartitions();
    if (showStats)
    {
        printf("System stats:\n");
        readSystemStats();
    }
    if (interval > 0 && duration > 0)
    {
        int elapsed_time = 0;
        while (elapsed_time < duration)
        {
            readDiskRequests();
            readMemoryInfo();
            readLoadAverage();
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
```

Puede verse al inicio, que se tiene un flag que sirve para determinar si el programa se ejecutara creando un archivo JSON o no. Para ello, se crean dos objetos JSON:
- Uno para almacenar toda la informacion solicitada, independientemente del argumento.
- Otro para almacenar la informacion referente al comando `-l`, ya que se creyo mas conveniente hacerlo de esa forma. 

En el caso de haberse creado ambos, al final del programa se imprimen uno atras del otro. Se muestra a continuacion, la salida para el caso en que se ejecute la siguiente llamada en la consola `./rdprocE -l 2 8 -j`:

```
Machine name: all5one-Ubuntu
Current date and time: Sun Dec  3 01:04:26 2023
CPU model and type: Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: 4
CPU model and type: Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: 4
CPU model and type: Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: 4
CPU model and type: Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz
CPU cores: 4
Kernel version: Linux version 5.19.0-32-generic (buildd@lcy02-amd64-026) (x86_64-linux-gnu-gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #33~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Mon Jan 30 17:03:34 UTC 2
Elapsed time: 00:00:53:12
Kernel suported filesystems:
nodev   sysfs
nodev   tmpfs
nodev   bdev
nodev   proc
nodev   cgroup
nodev   cgroup2
nodev   cpuset
nodev   devtmpfs
nodev   configfs
nodev   debugfs
nodev   tracefs
nodev   securityfs
nodev   sockfs
nodev   bpf
nodev   pipefs
nodev   ramfs
nodev   hugetlbfs
nodev   devpts
        ext3
        ext2
        ext4
        squashfs
        vfat
nodev   ecryptfs
        fuseblk
nodev   fuse
nodev   fusectl
nodev   mqueue
nodev   pstore
nodev   autofs
nodev   binfmt_misc
Partitions and filesystems:
sysfs /sys sysfs rw,nosuid,nodev,noexec,relatime 0 0
proc /proc proc rw,nosuid,nodev,noexec,relatime 0 0
udev /dev devtmpfs rw,nosuid,relatime,size=12212396k,nr_inodes=3053099,mode=755,inode64 0 0
devpts /dev/pts devpts rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000 0 0
tmpfs /run tmpfs rw,nosuid,nodev,noexec,relatime,size=2450240k,mode=755,inode64 0 0
/dev/sda8 / ext4 rw,relatime,errors=remount-ro 0 0
securityfs /sys/kernel/security securityfs rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /dev/shm tmpfs rw,nosuid,nodev,inode64 0 0
tmpfs /run/lock tmpfs rw,nosuid,nodev,noexec,relatime,size=5120k,inode64 0 0
cgroup2 /sys/fs/cgroup cgroup2 rw,nosuid,nodev,noexec,relatime,nsdelegate,memory_recursiveprot 0 0
pstore /sys/fs/pstore pstore rw,nosuid,nodev,noexec,relatime 0 0
bpf /sys/fs/bpf bpf rw,nosuid,nodev,noexec,relatime,mode=700 0 0
systemd-1 /proc/sys/fs/binfmt_misc autofs rw,relatime,fd=29,pgrp=1,timeout=0,minproto=5,maxproto=5,direct,pipe_ino=17139 0 0
mqueue /dev/mqueue mqueue rw,nosuid,nodev,noexec,relatime 0 0
hugetlbfs /dev/hugepages hugetlbfs rw,relatime,pagesize=2M 0 0
tracefs /sys/kernel/tracing tracefs rw,nosuid,nodev,noexec,relatime 0 0
debugfs /sys/kernel/debug debugfs rw,nosuid,nodev,noexec,relatime 0 0
fusectl /sys/fs/fuse/connections fusectl rw,nosuid,nodev,noexec,relatime 0 0
configfs /sys/kernel/config configfs rw,nosuid,nodev,noexec,relatime 0 0
ramfs /run/credentials/systemd-sysusers.service ramfs ro,nosuid,nodev,noexec,relatime,mode=700 0 0
/dev/loop14 /snap/snapd-desktop-integration/83 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop2 /snap/core20/2015 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop3 /snap/gnome-3-38-2004/143 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop15 /snap/snapd-desktop-integration/49 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/sda8 /var/snap/firefox/common/host-hunspell ext4 ro,noexec,noatime,errors=remount-ro 0 0
/dev/loop1 /snap/core20/1822 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop0 /snap/bare/5 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop7 /snap/snapd/20290 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop11 /snap/firefox/2356 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop13 /snap/telegram-desktop/5305 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop16 /snap/snap-store/959 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop10 /snap/core22/864 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop12 /snap/gnome-3-38-2004/119 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop6 /snap/gtk-common-themes/1535 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop8 /snap/snap-store/638 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop4 /snap/code/146 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop5 /snap/snapd/18357 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/loop9 /snap/gnome-42-2204/141 squashfs ro,nodev,relatime,errors=continue 0 0
/dev/sda6 /boot/efi vfat rw,relatime,fmask=0077,dmask=0077,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0 0
binfmt_misc /proc/sys/fs/binfmt_misc binfmt_misc rw,nosuid,nodev,noexec,relatime 0 0
tmpfs /run/user/1000 tmpfs rw,nosuid,nodev,relatime,size=2450236k,nr_inodes=612559,mode=700,uid=1000,gid=1000,inode64 0 0
portal /run/user/1000/doc fuse.portal rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
tmpfs /run/snapd/ns tmpfs rw,nosuid,nodev,noexec,relatime,size=2450240k,mode=755,inode64 0 0
nsfs /run/snapd/ns/snapd-desktop-integration.mnt nsfs rw 0 0
gvfsd-fuse /run/user/1000/gvfs fuse.gvfsd-fuse rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0
nsfs /run/snapd/ns/snap-store.mnt nsfs rw 0 0
nsfs /run/snapd/ns/firefox.mnt nsfs rw 0 0
Disk requests: 7199382
Available memory / total memory: 21523148 / 24502376
Load average in last minute: 0.70
Disk requests: 7199382
Available memory / total memory: 21522392 / 24502376
Load average in last minute: 0.64
Disk requests: 7199382
Available memory / total memory: 21508556 / 24502376
Load average in last minute: 0.64
Disk requests: 7199382
Available memory / total memory: 21508556 / 24502376
Load average in last minute: 0.59
{
        "Machine Name": "all5one-Ubuntu",
        "Date": "Sun Dec  3 01:04:26 2023\n",
        "CPU model and type":   "Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz",
        "CPU cores":    "4",
        "CPU model and type":   "Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz",
        "CPU cores":    "4",
        "CPU model and type":   "Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz",
        "CPU cores":    "4",
        "CPU model and type":   "Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz",
        "CPU cores":    "4",
        "Kernel version":       "Linux version 5.19.0-32-generic (buildd@lcy02-amd64-026) (x86_64-linux-gnu-gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #33~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Mon Jan 30 17:03:34 UTC 2\n",
        "Elapsed time": "00:00:53:12",
        "Kernel suported filesystems":  ["nodev\tsysfs\n", "nodev\ttmpfs\n", "nodev\tbdev\n", "nodev\tproc\n", "nodev\tcgroup\n", "nodev\tcgroup2\n", "nodev\tcpuset\n", "nodev\tdevtmpfs\n", "nodev\tconfigfs\n", "nodev\tdebugfs\n", "nodev\ttracefs\n", "nodev\tsecurityfs\n", "nodev\tsockfs\n", "nodev\tbpf\n", "nodev\tpipefs\n", "nodev\tramfs\n", "nodev\thugetlbfs\n", "nodev\tdevpts\n", "\text3\n", "\text2\n", "\text4\n", "\tsquashfs\n", "\tvfat\n", "nodev\tecryptfs\n", "\tfuseblk\n", "nodev\tfuse\n", "nodev\tfusectl\n", "nodev\tmqueue\n", "nodev\tpstore\n", "nodev\tautofs\n", "nodev\tbinfmt_misc\n"],
        "Partitions and filesystems":   ["sysfs /sys sysfs rw,nosuid,nodev,noexec,relatime 0 0\n", "proc /proc proc rw,nosuid,nodev,noexec,relatime 0 0\n", "udev /dev devtmpfs rw,nosuid,relatime,size=12212396k,nr_inodes=3053099,mode=755,inode64 0 0\n", "devpts /dev/pts devpts rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000 0 0\n", "tmpfs /run tmpfs rw,nosuid,nodev,noexec,relatime,size=2450240k,mode=755,inode64 0 0\n", "/dev/sda8 / ext4 rw,relatime,errors=remount-ro 0 0\n", "securityfs /sys/kernel/security securityfs rw,nosuid,nodev,noexec,relatime 0 0\n", "tmpfs /dev/shm tmpfs rw,nosuid,nodev,inode64 0 0\n", "tmpfs /run/lock tmpfs rw,nosuid,nodev,noexec,relatime,size=5120k,inode64 0 0\n", "cgroup2 /sys/fs/cgroup cgroup2 rw,nosuid,nodev,noexec,relatime,nsdelegate,memory_recursiveprot 0 0\n", "pstore /sys/fs/pstore pstore rw,nosuid,nodev,noexec,relatime 0 0\n", "bpf /sys/fs/bpf bpf rw,nosuid,nodev,noexec,relatime,mode=700 0 0\n", "systemd-1 /proc/sys/fs/binfmt_misc autofs rw,relatime,fd=29,pgrp=1,timeout=0,minproto=5,maxproto=5,direct,pipe_ino=17139 0 0\n", "mqueue /dev/mqueue mqueue rw,nosuid,nodev,noexec,relatime 0 0\n", "hugetlbfs /dev/hugepages hugetlbfs rw,relatime,pagesize=2M 0 0\n", "tracefs /sys/kernel/tracing tracefs rw,nosuid,nodev,noexec,relatime 0 0\n", "debugfs /sys/kernel/debug debugfs rw,nosuid,nodev,noexec,relatime 0 0\n", "fusectl /sys/fs/fuse/connections fusectl rw,nosuid,nodev,noexec,relatime 0 0\n", "configfs /sys/kernel/config configfs rw,nosuid,nodev,noexec,relatime 0 0\n", "ramfs /run/credentials/systemd-sysusers.service ramfs ro,nosuid,nodev,noexec,relatime,mode=700 0 0\n", "/dev/loop14 /snap/snapd-desktop-integration/83 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop2 /snap/core20/2015 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop3 /snap/gnome-3-38-2004/143 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop15 /snap/snapd-desktop-integration/49 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/sda8 /var/snap/firefox/common/host-hunspell ext4 ro,noexec,noatime,errors=remount-ro 0 0\n", "/dev/loop1 /snap/core20/1822 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop0 /snap/bare/5 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop7 /snap/snapd/20290 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop11 /snap/firefox/2356 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop13 /snap/telegram-desktop/5305 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop16 /snap/snap-store/959 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop10 /snap/core22/864 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop12 /snap/gnome-3-38-2004/119 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop6 /snap/gtk-common-themes/1535 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop8 /snap/snap-store/638 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop4 /snap/code/146 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop5 /snap/snapd/18357 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/loop9 /snap/gnome-42-2204/141 squashfs ro,nodev,relatime,errors=continue 0 0\n", "/dev/sda6 /boot/efi vfat rw,relatime,fmask=0077,dmask=0077,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0 0\n", "binfmt_misc /proc/sys/fs/binfmt_misc binfmt_misc rw,nosuid,nodev,noexec,relatime 0 0\n", "tmpfs /run/user/1000 tmpfs rw,nosuid,nodev,relatime,size=2450236k,nr_inodes=612559,mode=700,uid=1000,gid=1000,inode64 0 0\n", "portal /run/user/1000/doc fuse.portal rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0\n", "tmpfs /run/snapd/ns tmpfs rw,nosuid,nodev,noexec,relatime,size=2450240k,mode=755,inode64 0 0\n", "nsfs /run/snapd/ns/snapd-desktop-integration.mnt nsfs rw 0 0\n", "gvfsd-fuse /run/user/1000/gvfs fuse.gvfsd-fuse rw,nosuid,nodev,relatime,user_id=1000,group_id=1000 0 0\n", "nsfs /run/snapd/ns/snap-store.mnt nsfs rw 0 0\n", "nsfs /run/snapd/ns/firefox.mnt nsfs rw 0 0\n"]
}
{
        "Disk requests":        7199382,
        "Available memory":     21523148,
        "Total memory": 24502376,
        "Load average in last minute":  0.699999988079071,
        "Disk requests":        7199382,
        "Available memory":     21522392,
        "Total memory": 24502376,
        "Load average in last minute":  0.63999998569488525,
        "Disk requests":        7199382,
        "Available memory":     21508556,
        "Total memory": 24502376,
        "Load average in last minute":  0.63999998569488525,
        "Disk requests":        7199382,
        "Available memory":     21508556,
        "Total memory": 24502376,
        "Load average in last minute":  0.5899999737739563
}
```