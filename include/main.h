#ifndef MAIN_H
#define MAIN_H

enum ErrorCode {
    EWIFI_TIME,
    EI2C_GRD,
    EI2C_VDD,
    EMONITOR
};

struct ErrorCounter{
  int Wifi_time_cnt;
  int I2C_GRD_cnt;
  int I2C_VDD_cnt;
};

#endif // ANOTHERCLASS_H