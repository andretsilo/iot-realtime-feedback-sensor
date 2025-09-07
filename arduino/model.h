#ifndef MODEL_H
#define MODEL_H
#include <Arduino.h>
#include "feedback.h"
struct FeedbackModel {
  String sensorId;     
  FeedbackLevel feedback; 
  String timestamp;    
  String timezone;    
  FeedbackModel(String id, FeedbackLevel fb, String ts, String tz)
  : sensorId(id), feedback(fb), timestamp(ts), timezone(tz) {}
};
#endif 
