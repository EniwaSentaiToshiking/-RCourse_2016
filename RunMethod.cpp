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
    
    counter = 0;
    

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
          
          if(!tail_flag){
              
              mTailControl->tail_control(100,50,true);
              tail_flag = true;
          }
          
          mLineTraceMethod->run(0.40, 0.0, 0.030,120,-120,120,0);
          
          mTailControl->tail_control(tail,100,false);
          if(mea1->point_x>210){
              Line_Trace_flag = 1;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 1:
          //mLineTraceMethod->run(0.86, 0.008, 0.032,70,-100,100,0);
          //mLineTraceMethod->run(1.0, 0.01, 0.03,80,-80,80,0);
          //mLineTraceMethod->run(1.6, 0.01, 0.3,100,-100,100,0);
          
          mLineTraceMethod->run(1.4, 0.0, 0.08,80,-80,80,0);
          if(mea1->point_y>45){
              Line_Trace_flag = 2;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 2:
          mLineTraceMethod->run(0.40, 0.0, 0.03,120,-120,120,0);
          if(mea1->point_y>210){
              Line_Trace_flag = 4;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 3:
           mLineTraceMethod->run(0.40, 0.0, 0.03,100,-100,100,0);
          if(mea1->point_y>195){
              Line_Trace_flag = 4;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 4:
          //mLineTraceMethod->run(1.84, 0.0, 0.105,60,-60,60,0);
          mLineTraceMethod->run(1.5, 0.0, 0.07,60,-60,60,0);
          if(mea1->point_y<190){
              Line_Trace_flag = 5;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 5:
          mLineTraceMethod->run(0.4, 0.0, 0.03,100,-100,100,0);
          if(mea1->point_y<95){
              Line_Trace_flag = 6;
              ev3_speaker_play_tone (480,100);
          }
          break;
      case 6:
          mLineTraceMethod->run(1.82, 0.0, 0.108,80,-80,80,0);
          if(mea1->point_y>85&&mea1->point_x<200){
              Line_Trace_flag = 7;
              ev3_speaker_play_tone (480,100);
              mClock->reset();
          }
          break;
      case 7:
          
	mLineTraceMethod->run(0.3, 0.0, 0.03,18,-18,18,-1);

          // Todo  if 灰色検知 ->　mea1->distance_reset()
          if(mea1->point_x<150){
              ev3_speaker_play_tone (480,100);
              //尻尾下ろす処理
              mClock->reset();
              //灰色検知し始める
              
              Line_Trace_flag = 8;
          }

          break;
          
      case 8:
          
          if(mClock->now() <= 1000){
              mTailControl->tail_control(85,50,false);
               mLineTraceMethod->run(0.0, 0.0, 0.0,0,0,0,0);
          }else {
              mState = SCENARIO_TRACE;
              SCENARIO_flag = 0;
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
          
      case 0:
           gar->slowrun_stop();
          
          SCENARIO_flag = -1;
          
          break;

    case 2:
          
          if(mClock->now() >= 6500){
              gar->slowrun_stop();
          }else {
              mLineTraceMethod->SCENARIO_run();
          }

    break;


default:
          mLineTraceMethod->SCENARIO_run();
          
          if(g_check1->gray_checker(mCalibration->LIGHT_BLACK,mCalibration->LIGHT_WHITE)==0){
              SCENARIO_flag = 2;
              mClock->reset();
              ev3_speaker_play_tone (480,100);
          }


 break;
}
}
