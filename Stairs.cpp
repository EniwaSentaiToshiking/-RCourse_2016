#include "Stairs.h"

Stairs::Stairs(const GyroSensor* gyroSensor, Motor* leftMotor,
	Motor* rightMotor, TailControl* tailControl,
	Clock* clock,BalancingWalker* balancingWalker, LineTraceMethod* lineTraceMethod,gray_check* g_check2, Measure* mea2){

	mGyroSensor = gyroSensor;
	mRightMotor = rightMotor;
	mLeftMotor = leftMotor;
	mTailControl = tailControl;
	mClock = clock;
	mBalancingWalker = balancingWalker;
	mLineTraceMethod = lineTraceMethod;
    g_check1 = g_check2;
    mea1 = mea2;

	stairsFlag = 0;
}

Stairs::~Stairs() {}

void Stairs::run(){

	mClock->reset();
    
		switch(stairsFlag){
			case 0:

			mTailControl->tail_control(85, 50, false);
                
                
                mLeftMotor->setPWM(20);
                mRightMotor->setPWM(20);

                mClock->wait(300);

                mLeftMotor->setPWM(0);
                mRightMotor->setPWM(0);

                mClock->wait(2000);
                
                for(int i = 86; i<= 93; i++){
                    
                    mTailControl->tail_control(i,10,true);
                    
                    mClock->wait(4);
                }

                mClock->wait(2000);
                
                stairsFlag = -1;

			break;

            case 1:
                
                
                //if 距離進んでなかったら音鳴らしてストップ。
                
                break;
                
			default:
			stairsFlag = -1;
			break;

    }
}