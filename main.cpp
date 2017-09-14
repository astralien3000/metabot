#include "main.h"

#include <stdlib.h>
#include <math.h>

#include "function.h"
#include "config.h"
#include "motion.h"
#include "leds.h"
#include "mapping.h"

#include <xtimer.h>
#include <dynamixel.h>

extern "C" {
#include <rclc/rclc.h>
}

#define ENABLE_DEBUG 0
#include <debug.h>

void* __dso_handle;

extern float dx;
extern float turn;

#define MAX_DX (100)
#define MAX_TURN (40)

#include <custom_msgs/msg/gyro.h>

void chatter_callback(const void* v_msg)
{
  const custom_msgs__msg__Gyro* msg = (const custom_msgs__msg__Gyro*)v_msg;
  printf("I heard: [ %i %i %i ]\n", (int)msg->z_angle, (int)msg->z_angle, (int)msg->z_angle);

  turn = msg->z_angle;
  dx = msg->x_angle;

  if(turn > MAX_TURN) turn = MAX_TURN;
  if(turn < -MAX_TURN) turn = -MAX_TURN;

  if(dx > MAX_DX) dx = MAX_DX;
  if(dx < -MAX_DX) dx = -MAX_DX;
}

int main(void) {
  static int argc = 0;
  static char **argv = NULL;

  DEBUG("setup\n");
  setup();

  rclc_init(argc, argv);
  rclc_node_t* node = rclc_create_node("listener");
  rclc_subscription_t* sub =
      rclc_create_subscription(node, ROSIDL_GET_MSG_TYPE_SUPPORT(custom_msgs, Gyro), "chatter", chatter_callback, 1, false);

  while (rclc_ok()) {
    loop();
    rclc_sleep_ms(10);
  }

  rclc_destroy_subscription(sub);
  rclc_destroy_node(node);

  return 0;
}

bool flag = false;
bool isUSB = false;

// Time
float t = 0.0;

void setFlag()
{
  flag = true;
}

// Enabling/disabling move
bool move = true;

// Average voltage
int voltage = 75;

// Dynamixel stream
uart_half_duplex_t stream;

// Dynamixel functions
void dxl_set_angle(uint8_t id, float angle) {
  uint16_t pos = (angle/300.0)*1024+512;
  dynamixel_t dev;
  dynamixel_init(&dev, &stream, id);
  dynamixel_write16(&dev, XL320_GOAL_POSITION, pos);
}

void dxl_configure_all() {
  for (int i=0; i<3; i++) {
    dynamixel_t dev;
    dynamixel_init(&dev, &stream, 0xFF);
    dynamixel_write8(&dev, XL320_RETURN_DELAY_TIME, 0);
    dynamixel_write8(&dev, XL320_RETURN_LEVEL, 1);
  }
}

//Initializing
void setup()
{
  (void) signs;
  motion_init();

  // Initializing the DXL bus
  xtimer_usleep(500000);

  static uint8_t buff[64];
  uart_half_duplex_params_t params;
  params.baudrate = 1000000;
  params.uart = 1;
  params.dir = UART_HALF_DUPLEX_DIR_NONE;
  uart_half_duplex_init(&stream, buff, sizeof(buff), &params);

  // Initializing config (see config.h)
  config_init();

  // Initializing positions to 0
  for (int i=0; i<12; i++) {
    dxl_set_angle(servos_order[i], 0.0);
  }
  for (int i=0; i<4; i++) {
    l1[i] = l2[i] = l3[i] = 0;
  }

  dxl_configure_all();
}

// Computing the servo values
void tick()
{
  // Computing average voltage
  static int idToRead = 0;
  static int blink;

  idToRead++;
  if (idToRead >= 12) idToRead = 0;
  uint8_t voltageOnce;
  dynamixel_t dev;
  dynamixel_init(&dev, &stream, idToRead+1);
  int success = dynamixel_read8(&dev, XL320_PRESENT_VOLTAGE, &voltageOnce);
  if (success == DYNAMIXEL_OK) {
    if (voltageOnce < voltage) voltage--;
    if (voltageOnce > voltage) voltage++;
  }

  if (voltage < 60) {
    dynamixel_t dev;
    dynamixel_init(&dev, &stream, 0xFF);
    dynamixel_write16(&dev, XL320_GOAL_TORQUE, 0);
    blink++;
    if (blink > 10) {
      blink = 0;
    }
    dynamixel_write8(&dev, XL320_LED, blink<5);
    return;
  }

  if (!move) {
    t = 0.0;
    return;
  }

  // Incrementing and normalizing t
  t += motion_get_f()*0.02;
  if (t > 1.0) {
    t -= 1.0;
    colorize();
  }
  if (t < 0.0) t += 1.0;

  motion_tick(t);

  // Sending order to servos
  uint32_t timeout_save = stream.timeout_us;
  stream.timeout_us = 0;
  dxl_set_angle(mapping[0], l1[0]);
  dxl_set_angle(mapping[3], l1[1]);
  dxl_set_angle(mapping[6], l1[2]);
  dxl_set_angle(mapping[9], l1[3]);

  dxl_set_angle(mapping[1], l2[0]);
  dxl_set_angle(mapping[4], l2[1]);
  dxl_set_angle(mapping[7], l2[2]);
  dxl_set_angle(mapping[10], l2[3]);

  dxl_set_angle(mapping[2], l3[0]);
  dxl_set_angle(mapping[5], l3[1]);
  dxl_set_angle(mapping[8], l3[2]);
  dxl_set_angle(mapping[11], l3[3]);
  stream.timeout_us = timeout_save;
}

uint32_t last = 0;

void loop()
{
  if(xtimer_now_usec() - last >= 40000) {
    last = xtimer_now_usec();
    tick();
  }
}
