#include <wirish/UARTSerial.h>

UARTSerial<256> Serial1(1);
UARTSerial<256> Serial2(0);
UARTSerial<256> SerialUSB(0);

void* __dso_handle;
