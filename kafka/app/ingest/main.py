from fastapi import FastAPI, HTTPException
import logging
from app.ingest.models import SensorFeedback, FeedbackEnum
from app.stream.producer import stream_to_kafka
from app.config.logger import logger
import re

app = FastAPI()

@app.post("/sensor")
async def root(feedback: SensorFeedback):
    logger.info("Received data")

    timestampRegex = "^(0[1-9]|[12][0-9]|3[01])\.(0[1-9]|1[0-2])\.(\d{4})\s(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])\.(\d{4})$"
    timeZoneRegex = "^\+(?:[0-9]|1[0-9]|2[0-3])$"
    
    timestampMatch = re.search(timestampRegex, feedback.generationTime)
    timeZoneMatch = re.search(timeZoneRegex, feedback.timeZone)

    if (timestampMatch == None):
        raise HTTPException(status_code=400, detail="Timestamp not formatted correctly")
    if (timeZoneMatch == None):
        raise HTTPException(status_code=400, detail="Time zone not formatted correctly")

    await stream_to_kafka()
