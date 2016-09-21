#include "Garage.h"

Garage::Garage(Motor* r, Motor* l, TailControl* tc, BalancingWalker* b, Clock* clock, GyroSensor* g){
	rMotor = r;
	lMotor = l;
	mTailControl = tc;
    bw = b;
    c = clock;
    gyro = g;
    GARAGE_flag = 0;
}

Garage::~Garage(){
}

void Garage::slowrun_stop(){
	
		switch(GARAGE_flag){
			case 0: //バランスを取りながら走行
               
                mTailControl->tail_control(85, 50, false);
                
                
                lMotor->setPWM(15);
                rMotor->setPWM(15);
                
                c->wait(300);
                
                lMotor->setPWM(0);
                rMotor->setPWM(0);
                
                c->wait(1000);
               
               
                for(int i = 84; i >= 70; i--){
                    mTailControl->tail_control(i,10,true);
                    
                    c->sleep(300);
                    
                }
                
                c->wait(2000);
                
                mTailControl->tail_control(75, 50, true);
                c->wait(1000);
                
                for(int i = 76; i <= 93; i++){
                     mTailControl->tail_control(i, 10, true);
                
                    c->sleep(4);
                }
                
                
                GARAGE_flag = 1;
                
                break;

            case 1: //尻尾を出しながら少し傾く
                // bw->setCommand(0,0,-15);
                // bw->run();
                
                rMotor->setPWM(10);
                lMotor->setPWM(10);
                
                mTailControl->tail_control(85, 50, false);
                
                c->wait(400);
                
                GARAGE_flag=2;
                
                break;
                
            case 2:
                
                rMotor->setBrake(true);
                lMotor->setBrake(true);
                
                rMotor->setPWM(0);
                lMotor->setPWM(0);
                
                break;
                
        }
}


