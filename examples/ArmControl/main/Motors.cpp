
#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor( long reduction_ratio ) : reduction_ratio_( reduction_ratio ) {} 

// -------------------- BLDC MOTOR -------------------- // 
OdriveMotor::OdriveMotor( const long baud_rate, const long reduction_ratio, const int rx_pin, const int tx_pin ) 
  : Motor( reduction_ratio ), 
    odrive_serial_( rx_pin, tx_pin ) 
{  
  // configure the odrive 
  odrive_ = new ODriveUART( odrive_serial_ ); 
  
  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 

  Serial.println("Waiting for odrive..."); 
  int attempts = 0; 
  while ( odrive_->getState() == AXIS_STATE_UNDEFINED ) 
  {
    if ( attempts > ATTEMPT_LIMIT )
    {
      Serial.println("Error: Failed to connect to ODrive");
      Serial.print("Odrive Error Code: ");
      Serial.println( getErrors(), HEX ); 

      abort(); 
      return; 
    }
    else
    {
      attempts++;
    }
    delay(10);
  }

  // Enable closed loop control 
  Serial.println("Enabling closed loop control"); 
  attempts = 0; 
  while ( odrive_->getState() != AXIS_STATE_CLOSED_LOOP_CONTROL ) 
  {
    odrive_->clearErrors();
    odrive_->setState(AXIS_STATE_CLOSED_LOOP_CONTROL); 

    Serial.print("ODrive State: ");
    Serial.println( odrive_->getState() );  

    if ( attempts > ATTEMPT_LIMIT )
    {
      Serial.println("Error: Failed to enable closed loop control"); 
      Serial.print("Odrive Error Code: ");
      Serial.println( getErrors(), HEX ); 

      abort(); 
      return; 
    }
    else
    {
      attempts++;
    }    
    delay(10);
  }
  Serial.print("Odrive Error Code: ");
  Serial.println( getErrors(), HEX ); 
}

void OdriveMotor::setVelocity( const double vel ) 
{
  odrive_serial_.listen(); 
  // delay(0.1); // wait to stabilize 
  odrive_->setVelocity( reduction_ratio_ * vel / METRIC_TO_RAD ); 
  odrive_serial_.stopListening(); // Free up the serial 
}

void OdriveMotor::setPosition( const double pos )
{
  odrive_serial_.listen(); 
  // delay(0.1); // wait to stabilize 

  if ( abs(pos - INVALID_COMMAND) > 0.001 ) 
  {
    odrive_->setPosition( reduction_ratio_ * pos / METRIC_TO_RAD ); 
  }
  odrive_serial_.stopListening(); // Free up the serial
}

double OdriveMotor::getPosition()
{
  // get feedback from odrive 
  odrive_serial_.listen(); 
  // delay(0.1); // wait to stabilize 
  ODriveFeedback feedback = odrive_->getFeedback(); 
  odrive_serial_.stopListening(); // Free up the serial

  return METRIC_TO_RAD * feedback.pos / reduction_ratio_; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  odrive_serial_.listen(); 
  // delay(0.1); // wait to stabilize 

  ODriveFeedback feedback = odrive_->getFeedback(); 
  odrive_serial_.stopListening(); // Free up the serial 

  return METRIC_TO_RAD * feedback.vel / reduction_ratio_; 
}

// Function that prints active errors to the Serial monitor
uint32_t OdriveMotor::getErrors()  
{
  // get feedback from odrive 
  odrive_serial_.listen(); 
  // delay(0.1); // wait to stabilize 

  uint32_t errors = atol(odrive_->getParameterAsString("axis0.active_errors").c_str());
  odrive_serial_.stopListening(); // Free up the serial

  return errors; 
}

//void OdriveMotor::printOdriveState()
//{
//  int state = odrive_->getState(); 
//  switch (state)
//  {
//    case AXIS_STATE_UNDEFINED:
//      Serial.println("AXIS_STATE_UNDEFINED");
//      break;
//    case AXIS_STATE_IDLE:
//      Serial.println("AXIS_STATE_IDLE");
//      break;
//    case AXIS_STATE_STARTUP_SEQUENCE:
//      Serial.println("AXIS_STATE_STARTUP_SEQUENCE");
//      break;
//    case AXIS_STATE_FULL_CALIBRATION_SEQUENCE:
//      Serial.println("AXIS_STATE_FULL_CALIBRATION_SEQUENCE");
//      break;
//    case AXIS_STATE_MOTOR_CALIBRATION:
//      Serial.println("AXIS_STATE_MOTOR_CALIBRATION");
//      break;
//    case AXIS_STATE_ENCODER_INDEX_SEARCH:
//      Serial.println("AXIS_STATE_ENCODER_INDEX_SEARCH");
//      break;
//    case AXIS_STATE_ENCODER_OFFSET_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_OFFSET_CALIBRATION");
//      break;
//    case AXIS_STATE_CLOSED_LOOP_CONTROL:
//      Serial.println("AXIS_STATE_CLOSED_LOOP_CONTROL");
//      break;
//    case AXIS_STATE_LOCKIN_SPIN:
//      Serial.println("AXIS_STATE_LOCKIN_SPIN");
//      break;
//    case AXIS_STATE_ENCODER_DIR_FIND:
//      Serial.println("AXIS_STATE_ENCODER_DIR_FIND");
//      break;
//    case AXIS_STATE_HOMING:
//      Serial.println("AXIS_STATE_HOMING");
//      break;
//    case AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION");
//      break;
//    case AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION");
//      break;
//    case AXIS_STATE_ANTICOGGING_CALIBRATION:
//      Serial.println("AXIS_STATE_ANTICOGGING_CALIBRATION");
//      break;
//    default:
//      Serial.println("Error: State not recognised");
//      break;
//  }
//}
