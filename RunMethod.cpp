#include "RunMethod.h"

RunMethod::RunMethod(GyroSensor* gyroSensor,LineTraceMethod* lineTraceMethod,
  RemoteStart* remoteStart,Calibration* calibration,
  TailControl* tailControl,Clock* clock,TouchSensor* touchSensor,
  LookupMethod* lookupMethod,Stairs* stairs,Garage* g,SonarAlert* sonarAlert,gray_check* g_check2, Measure* mea2){

  mLineTraceMethod = lineTraceMethod;
  mRemoteStart = remoteStart;
  mCalibration = calibration;
  mTailControl = tailControl;
  mClock = clock;
  mState = UNDEFINED;
  mTouchSensor = touchSensor;
  mGyroSensor = gyroSensor;
  mLookupMethod = lookupMethod;
  mStairs = stairs;
  mSonarAlert = sonarAlert;
  gar = g;
  g_check1 = g_check2;
  mea1 = mea2;

  tail = 95;
  tail_flag = false;
}

RunMethod::~RunMethod(){

  tail = 95;
  tail_flag = false;

}

void RunMethod::run(){
  switch(mState){
    case UNDEFINED: //キャリブレーション
    execUndefined();
    break;
    case WAITING_FOR_START: //スタート待ち
    execWaitingForStart();
    break;
    case LINE_TRACE: //ただのコース走行
    execLineTracing();
    break;
    case SCENARIO_TRACE: //難所走行
    execScenarioTracing();
    break;
    default:
    break;
}
}

/**
 * 未定義状態の処理
**/
 void RunMethod::execUndefined() {
  mCalibration->initCali();
  mTailControl->tail_control(tail,80,true);

  if(mCalibration->touch_counter == 3){
    mState = WAITING_FOR_START;
}

mClock->sleep(10);
}

/**
 * 開始待ち状態の処理
**/
 void RunMethod::execWaitingForStart() {

  mTailControl->tail_control(tail,20,true);

  if(mRemoteStart->remote_start()){
    mState = LINE_TRACE;
    Line_Trace_flag = 0;
    mClock->reset();
    tail = 0;
}
mClock->sleep(10);
}

/**
 * ライントレース状態の処理
**/
//右
 void RunMethod::execLineTracing(){
  switch(Line_Trace_flag){
      case 0: //ここから走行スタート
          mTailControl->tail_control(tail,20,false);
          mLineTraceMethod->run(0.34, 0.0, 0.01,120,-120,120,0);
          if(mea1->point_x>230){
              Line_Trace_flag = 1;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 1:
          //mLineTraceMethod->run(0.86, 0.008, 0.032,70,-100,100,0);
          mLineTraceMethod->run(1.6, 0.01, 0.21,100,-100,100,0);
          //mLineTraceMethod->run(1.0, 0.01, 0.03,80,-80,80,0);
          if(mea1->point_y>40){
              Line_Trace_flag = 2;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 2:
          mLineTraceMethod->run(0.4, 0.0, 0.01,120,-120,120,0);
          if(mea1->point_y>200){
              Line_Trace_flag = 4;
              ev3_speaker_play_tone (480,100);
          }
          break;
//      case 3:
//          mLineTraceMethod->run(1.7, 0.01, 0.04,100,-100,100,0);
//          if(mea1->point_y>180){
//              Line_Trace_flag = 4;
//              ev3_speaker_play_tone (480,100);
//          }
//          break;
//      case 20:
//          mLineTraceMethod->run(0.8, 0.01, 0.04,100,-100,100,0);
//          if(mea1->point_x<200){
//              Line_Trace_flag = 21;
//              ev3_speaker_play_tone (480,100);
//          }
//          break;
//      case 21:
//          mLineTraceMethod->run(1.8, 0.01, 0.04,100,-100,100,0);
//          if(mea1->point_y<175){
//              Line_Trace_flag = 4;
//              ev3_speaker_play_tone (480,100);
//          }
//          break;
      case 4:
          mLineTraceMethod->run(1.6, 0.01, 0.35,80,-80,80,0);
          if(mea1->point_y<190){
              Line_Trace_flag = 5;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 5:
          mLineTraceMethod->run(0.34, 0.01, 0.04,100,-100,100,0);
          if(mea1->point_y<95){
              Line_Trace_flag = 6;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 6:
          mLineTraceMethod->run(1.3, 0.01, 0.04,100,-100,100,0);
          if(mea1->point_y>92){
              Line_Trace_flag = 7;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 7:
           mLineTraceMethod->run(0.3, 0.005, 0.03,18,-18,18,0);
          
          if(mea1->point_y>150){
              mLineTraceMethod->run(0, 0, 0,0,0,0,0);
              ev3_speaker_play_tone (480,100);
              //Line_Trace_flag= 8;
              //倒立
              // if 静止　case 8へ
          }
          break;
      case 8:
          
          mLineTraceMethod->spin_run();
          
          //if 一回転 -> case9へ
          
          break;
          
      case 9:
          
          
          //case 7,8 のような動作を繰り返し
          //case 10へ
          break;
          
          
      case 10:
           mLineTraceMethod->run(0.3, 0.005, 0.03,18,-18,18,0);
          
          // if 灰色検知 -> ガレージ in
          
          break;
          // mLineTraceMethod->run(0.27, 0.0, 0.01,120,-120,120,0);
          // mLineTraceMethod->run(0.86, 0.008, 0.032,70,-100,100,0);//p,i,d,forward,pidの最小値,pidの最大値/0.022
          // mLineTraceMethod->run(0.3, 0.005, 0.03,30,-30,30,-8);
          // mLineTraceMethod->run(0.3, 0.005, 0.03,18,-30,30,0);

  case -2:

  mLineTraceMethod->run(0.40, 0.0, 0.00,18,-18,18,-1);
  mTailControl->tail_control(tail,20,false);


  if(mGyroSensor->getAnglerVelocity() <= -100 && mClock->now() >= 3000){

    mState = SCENARIO_TRACE;
    SCENARIO_flag = 3;

  }

break;

default:
break;
}
}

/**
 * ゴール後の処理
 */
 void RunMethod::execScenarioTracing(){

  switch(SCENARIO_flag){
    case 1:

    if(mClock->now() <= 3000){
        mLineTraceMethod->run(0.0, 0.0, 0.0,0,0,0,0);
        mTailControl->tail_control(85, 50, true);
    }else{
        mLookupMethod->run();
        SCENARIO_flag = 0;
    }
    break;

    case 2:
    gar->slowrun_stop();
    break;

    case 3:

    mStairs->run();
 break;

 default:

 mLineTraceMethod->SCENARIO_run();

 break;
}
}
