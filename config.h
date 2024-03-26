#ifndef CONFIG_H_IN
#define CONFIG_H_IN

#define MODBUS_TIMEOUT 500000
#define BAUD_RATE 38400
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define COMMUNICATION_WITH_PS_INTERVAL 1000000
#define POWER_SUPPLY 200

// Only for testing:
#define LINUX_VERSION "ON"
#define VIRTUAL_PORT_LINUX "/dev/pts/3"
#define ILLEGAL_DATA_VALUE_LINUX_TEST 0
#define ANSWERS_ONE_OF_N_REQUESTS_LINUX_TEST 0
#define ONE_OF_N_REQUESTS_IS_RANDOM_BYTES_LINUX_TEST 0
#define ERROR_RESPONSE_TO_ONE_OF_N_READ_REQUESTS_LINUX_TEST 0

#endif // CONFIG_H_IN
