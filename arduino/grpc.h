#ifndef MAIN_GRPC_H
#define MAIN_GRPC_H
#include "model.h"
#include "GrpcClient.h"
inline void sendFeedbackGrpc(const FeedbackModel &fb) {
  GrpcClient::sendFeedback(fb);
}
#endif