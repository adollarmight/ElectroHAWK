#include <WiFi.h>
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include "server.h"
#include "uart.h"

#define SSID "DIGI_3e8fc8"
#define PASSWORD "f988c170"

#define RX 3
#define TX 1

const int redRGBPin = 12, greenRGBPin = 13, blueRGBPin = 15;

HardwareSerial uartSerial(0);

com::TcpServer* server;
com::Uart<data::SensorData, data::ControlData>* uart;
data::SensorData sensorData = {0, 0, 0, 0, 0, 0};

void setColor(int red, int green, int blue) {
  analogWrite(redRGBPin, red);
  analogWrite(greenRGBPin, green);
  analogWrite(blueRGBPin, blue);
}

void connectWifi() {
  setColor(0, 0, 255);
  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
}

void initializeCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_VGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_vflip(s, 1); // Flips the image vertically
  s->set_hmirror(s, 1); // Flips the image horizontally
}

void setup() {
  pinMode(redRGBPin, OUTPUT);
  pinMode(greenRGBPin, OUTPUT);
  pinMode(blueRGBPin, OUTPUT);

  setColor(255, 0, 0);

  uartSerial.begin(115200, SERIAL_8N1, RX, TX);
  initializeCamera();
  connectWifi();
  
  setColor(0, 255, 0);

  server = new com::TcpServer();
  uart = new com::Uart<data::SensorData, data::ControlData>(uartSerial);
}

void loop() {
  uart->send(server->getInput());
  if (uart->isReadyToReceive())
    sensorData = uart->getReceived();

  server->update(sensorData);
  uart->update();
}
