#include <esp32-hal-log.h>

struct positionMap {
  const char* label;
  unsigned int angle;
};

// Adjust depending on your servo and print
const positionMap servoPos[12] = {
  { "Clear", 0 },
  { "Clouds", 36 },
  { "Rain", 72 },
  { "Thunderstorm", 108 },
  { "Snow", 144 },
  { "Mist", 180 },
  { "Now", 0 },
  { "3hr", 36 },
  { "6hr", 72 },
  { "12hr", 108 },
  { "24hr", 144 },
  { "48hr", 180 }
};

const int POSMAP_SIZE = sizeof(servoPos) / sizeof(servoPos[0]);

unsigned int weatherLookup(unsigned int id) {
  if(id >= 800 && id <= 802) {
    log_i("Clear");
    return servoPos[0].angle;
  }
  if(id >= 803 && id <= 804) {
    log_i("Clouds");
    return servoPos[1].angle;
  }
  if(id >= 701 && id <= 799) {
    log_i("Mist");
    return servoPos[5].angle;
  }
  if(id >= 600 && id <= 622) {
    log_i("Snow");
    return servoPos[4].angle;
  }
  if(id >= 500 && id <= 531) {
    log_i("Rain");
    return servoPos[2].angle;
  }
  if(id >= 300 && id <= 321) {
    log_i("Rain");
    return servoPos[2].angle;
  }
  if(id >= 200 && id <= 232) {
    log_i("Thunderstorm");
    return servoPos[3].angle;
  }
  else {
    log_i("Mist");
    return servoPos[5].angle;
  }
}
