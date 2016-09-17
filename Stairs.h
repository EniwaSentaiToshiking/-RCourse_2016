#ifndef STAIRS_H
#define STAIRS_H

#include "GyroSensor.h"
#include "Motor.h"
#include "TailControl.h"
#include "Clock.h"
#include "BalancingWalker.h"
#include "LineTraceMethod.h"
#include "gray_check.h"

using namespace ev3api;

class Stairs{

private:
  const GyroSensor* mGyroSensor;
  Motor* mLeftMotor;
  Motor* mRightMotor;
  TailControl* mTailControl;
  Clock* mClock;
  BalancingWalker* mBalancingWalker;
  LineTraceMethod* mLineTraceMethod;
    gray_check* g_check1;
    Measure* mea1;

public:
  Stairs(const GyroSensor* gyroSensor, Motor* leftMotor,
    Motor* rightMotor, TailControl* tailControl,Clock* clock,BalancingWalker* balancingWalker,LineTraceMethod* lineTraceMethod,gray_check* g_check2, Measure* mea2);
  void run();
    
      int stairsFlag;
  virtual ~Stairs();

};

#endif