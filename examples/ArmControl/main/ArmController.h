#ifndef ARMCONTROLLER_H
#define ARMCONTROLLER_H

#include "Motors.h"
#include "commands.h"

class ArmController
{
  public:
    ArmController( const int dofs ); 
    ~ArmController(); 

    void initArm( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num ); 
    void addMotor( const char* type, const long reduction_ratio, const int pin1, const int pin2, const long baud ); 
    void setMotorVel( const double* cmd_vels ); 
    void setMotorPos( const double* cmd_pos ); 
    void getMotorStates( double* pos_arr, double* vel_arr ) const; 
    int getNumMotors() const; 
    void printMotorErrors() const; 

  private:
    // --- STATIC CONSTANTS --- // 
    static const int MAX_MOTOR_NUM = 3; 
    static const int MOTOR_TYPE_BUFFERSIZE = 7; // can only be odrive, stepper or servo 

    // --- DATA --- // 
    Motor* motors_[MAX_MOTOR_NUM]; 
    int motor_num_; 
}; 


#endif 
