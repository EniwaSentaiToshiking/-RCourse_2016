#ifndef EV3_UNIT_BALANCERCPP_H_
#define EV3_UNIT_BALANCERCPP_H_

#include "ev3api.h"

class Balancer {
public:
    Balancer();
    virtual ~Balancer();

    void init(int offset);
    void update(int angle, int rwEnc, int lwEnc, int battery);
    void setCommand(int forward, int turn, int offset);
    int8_t getPwmRight();
    int8_t getPwmLeft();

private:
    int mForward;
    int mTurn;
    int mOffset;
    int8_t mRightPwm;
    int8_t mLeftPwm;
};

#endif  // EV3_UNIT_BALANCERCPP_H_