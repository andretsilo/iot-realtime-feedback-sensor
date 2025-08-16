from app.config.logger import logger
from app.ingest.models import SensorFeedback, FeedbackEnum
from kafka import KafkaProducer

async def stream_to_kafka(feedback: SensorFeedback):
    logger.info("Streaming to kafka: " + str(feedback))
    producer = KafkaProducer(bootstrap_servers=['broker:29092'], max_block_ms=5000)

    try:
        logger.info(feedback.model_dump_json(indent=4, exclude_none=False))
        producer.send('feedback_sensor', feedback.model_dump_json(indent=4, exclude_none=True).encode(encoding='utf-8'))
    except Exception as e:
        logger.error("Raised exception: " + e)