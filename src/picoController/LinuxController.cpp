#include "LinuxController.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

static int32_t readSerialLinux(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    fd_set set;
    struct timeval timeout{};
    int rv;
    int filedesc = open(LinuxController::serialPort, O_RDWR);

    if (filedesc < 0) {
        perror("open");
        while (true) {}
        return -1;
    }

    uint16_t bytesToBeRead = count;

    FD_ZERO(&set); /* clear the set */
    FD_SET(filedesc, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 0;
    timeout.tv_usec = LinuxController::READ_TIMEOUT * 1000;

    do {
        rv = select(filedesc + 1, &set, nullptr, nullptr, &timeout);

        if (rv == -1) {
            perror("select error");
            close(filedesc);
            return -1;
        } else if (rv == 0) {
//            printf("timeout\n");
            close(filedesc);
            return 0;
        } else {
            ssize_t bytesRead = read(filedesc, buf, count); /* there was data to read */
            if (bytesRead == -1) {
                perror("read");
                close(filedesc);
                return -1;
            }

            count -= bytesRead;
        }


    } while (count);

    close(filedesc);

    return bytesToBeRead;
}

static int32_t writeSerialLinux(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    int filedesc = open(LinuxController::serialPort, O_RDWR);

    if (write(filedesc, buf, count) < 0) {
        close(filedesc);
        return -1;
    }

    close(filedesc);
    return count;
}

void LinuxController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.read = readSerialLinux;

    platform_conf.write = writeSerialLinux;
}


