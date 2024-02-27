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
    uint16_t bytesToBeRead = count;

    FD_ZERO(&set); /* clear the set */
    FD_SET(filedesc, &set); /* add our file descriptor to the set */

    timeout.tv_sec = byte_timeout_ms;
    timeout.tv_usec = 0;

    ssize_t bytesRead = 0;

    do {
        rv = select(filedesc + 1, &set, nullptr, nullptr, &timeout);
        if (rv == -1)
            perror("select"); /* an error accured */
        else if (rv == 0)
            printf("timeout"); /* a timeout occured */
        else
            bytesRead = read(filedesc, buf, count); /* there was data to read */

        if (bytesRead > 0)
            count -= bytesRead;

    } while (rv > 0 && count && bytesRead >= 0);

    close(filedesc);

    return bytesToBeRead - count;
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


