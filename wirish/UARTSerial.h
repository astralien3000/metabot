#ifndef UART_SERIAL_H
#define UART_SERIAL_H

#include "Serial.h"

#include <aversive/buffer.hpp>

#include <periph/uart.h>

template<unsigned int INPUT_BUFFER_SIZE = 128>
class UARTSerial : public Serial {

private:
  const uart_t _uart_id;
  Buffer<char, INPUT_BUFFER_SIZE> _input;

public:
  inline UARTSerial(uart_t uart_id)
    : _uart_id(uart_id) {
  }

public:
  void begin(uint32_t baudrate) {
    uart_init(_uart_id, baudrate, [](void* t, uint8_t data){ ((UARTSerial*)t)->_input.enqueue(data); }, (void*)this);
  }

public:
  virtual void write(uint8 ch) {
    uart_write(_uart_id, &ch, 1);
  }

  //virtual void write(const char *str);
  virtual void write(const void *buf, uint32 len) {
    uart_write(_uart_id, (const uint8_t*)buf, len);
  }

  virtual uint32 available(void) {
    return _input.size();
  }

  virtual uint8 read(void) {
    char ret = _input.head();
    _input.dequeue();
    return ret;
  }

public:
  void waitDataToBeSent(void) {
    for(volatile unsigned int i = 0 ; i < 0xFF ; i++);
  }
};

extern UARTSerial<256> Serial1;
extern UARTSerial<256> Serial2;
extern UARTSerial<256> SerialUSB;

#endif

