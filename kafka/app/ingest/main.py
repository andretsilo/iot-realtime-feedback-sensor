from fastapi import FastAPI, status
import logging
from app.ingest.models import SensorFeedback, FeedbackEnum
from app.stream.producer import stream_to_kafka
from app.config.logger import logger

app = FastAPI()

@app.post("/sensor")
async def root(feedback: SensorFeedback):
    logger.info("Received data")
    await stream_to_kafka()
