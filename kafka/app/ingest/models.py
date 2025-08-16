from pydantic import BaseModel, Field
from enum import IntEnum

class FeedbackEnum(IntEnum):
    bad = 1
    mid = 2
    good = 3

class SensorFeedback(BaseModel):
    sensorId: str = Field(description = "The feedback sensor unique Id.")
    feedback: FeedbackEnum = Field(description = "The feedback received from the user.")
    generationTime: str = Field(description = "The time of generation for the data.")
    timeZone: str = Field(description = "The timezone for the feedback sensor.")