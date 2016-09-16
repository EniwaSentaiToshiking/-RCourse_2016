#include "Stairs.h"

Stairs::Stairs(const GyroSensor* gyroSensor, Motor* leftMotor,
	Motor* rightMotor, TailControl* tailControl,
	Clock* clock,BalancingWalker* balancingWalker, LineTraceMethod* lineTraceMethod){

	mGyroSensor = gyroSensor;
	mRightMotor = rightMotor;
	mLeftMotor = leftMotor;
	mTailControl = tailControl;
	mClock = clock;
	mBalancingWalker = balancingWalker;
	mLineTraceMethod = lineTraceMethod;
	stairsFlag = 0;
}

Stairs::~Stairs() {}

void Stairs::run(){

	mClock->reset();
	stairsFlag = 0;
	while(stairsFlag != -1){
		switch(stairsFlag){
			case 0:

			mTailControl->tail_control(90, 50, false);
                
                
                mLeftMotor->setPWM(20);
                mRightMotor->setPWM(20);

                mClock->wait(300);

               mLeftMotor->setPWM(0);
                mRightMotor->setPWM(0);

                mClock->wait(1000);

                mClock->reset();

                  mLeftMotor->setPWM(15);
                mRightMotor->setPWM(-15);

                mClock->wait(4200);

				mLeftMotor->setPWM(0);
                mRightMotor->setPWM(0);  

                mClock->wait(3000);              

                stairsFlag = -1;

			break;

			default:
			stairsFlag = -1;
			break;
		}
		mClock->sleep(4);
	}
}