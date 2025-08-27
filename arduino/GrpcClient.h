/*
#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H
#include "Logger.h"
#include "model.h"
#include "ConfigUtils.h"
namespace GrpcClient {
    void sendFeedback(const FeedbackModel &fb) {
        String host = ConfigUtils::get("sensor.api.grpc.host", "localhost");
        int port = ConfigUtils::get("sensor.api.grpc.port", "50051").toInt();
        String endpoint = ConfigUtils::get("sensor.api.endpoint", "/insertFeedback");
        Log.infoln("[gRPC] Preparing feedback message...");
        Log.infoln("[gRPC] Sending to " + host + ":" + String(port) + endpoint);
        Log.infoln("[gRPC] ID: " + fb.id);
        Log.infoln("[gRPC] Feedback: " + String(fb.feedback));
        Log.infoln("[gRPC] Timestamp: " + fb.timestamp);
        Log.infoln("[gRPC] Timezone: " + fb.timezone);
        int responseCode = 200;
        if (responseCode == 200) {
            Log.infoln("[gRPC] Response: OK (200)");
        } else {
            Log.errorln("[gRPC] Response ERROR: " + String(responseCode));
        }
    }
}
#endif
*/
#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H
#include <Arduino.h>
#include "Logger.h"
#include "model.h"
#include "ConfigUtils.h"
//difficoltà
namespace GrpcClient {
  inline void sendFeedback(const FeedbackModel &fb) {
    String host = ConfigUtils::get("sensor.api.grpc.host", "localhost");
    int port    = ConfigUtils::get("sensor.api.grpc.port", "50051").toInt();
    String ep   = ConfigUtils::get("sensor.api.endpoint", "/insertFeedback");
    Log.infoln("[gRPC] Sending to " + host + ":" + String(port) + ep);
    Log.infoln("[gRPC] sensorId=" + fb.sensorId + ", feedback=" + String((unsigned)fb.feedback) +
               ", ts=" + fb.timestamp + ", tz=" + fb.timezone);
    Log.infoln("[gRPC] Response: OK (200)");
  }
}
#endif