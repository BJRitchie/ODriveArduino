
#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor() {} 

void Motor::setBaudRate( const long baud_rate )
{
  baud_rate_ = baud_rate; 
}

// -------------------- BLDC MOTOR -------------------- // 
OdriveMotor::OdriveMotor( const long baud_rate, const int rx_pin, const int tx_pin ) 
  : //Motor(),                          // init the base class 
    odrive_serial_( rx_pin, tx_pin ), // init the odrive's serial 
    odrive_( odrive_serial_ )         // init the odrive handle 
{
  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 
  
  Serial.print( "BLDC Motor Constructed with baud rate: " );
  Serial.println( baud_rate ); 
  setBaudRate( baud_rate );  
}

void OdriveMotor::setVelocity( const double vel ) 
{
  odrive_.setVelocity( vel ); 
}

void OdriveMotor::setPosition( const double pos )
{
  odrive_.setPosition( pos ); 
}

double OdriveMotor::getPosition()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_.getFeedback(); 
  return feedback.pos; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_.getFeedback(); 
  return feedback.vel; 
}
