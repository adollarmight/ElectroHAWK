#include "instance.h"

Instance* instance;

void setup() {
  instance = new Instance();
}

void loop() {
  instance->update();
}
