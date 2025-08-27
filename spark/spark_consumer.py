import logging
from cassandra.auth import PlainTextAuthProvider
from cassandra.cluster import Cluster
from pyspark.sql import SparkSession
from pyspark.sql.functions import from_json, col
from pyspark.sql.types import StructType, StructField, StringType, ByteType

def create_df_from_kafka(spark_df):
    schema = StructType([
        StructField("sensorId", StringType(), False),
        StructField("feedback", ByteType(), False),
        StructField("generationTime", StringType(), False),
        StructField("timeZone", StringType(), False)
    ])

    selected_df = spark_df.selectExpr("CAST(value AS STRING)") \
        .select(from_json(col('value'), schema).alias('data')).select("data.*")
    
    return selected_df

def create_kafka_connection(spark: SparkSession):
    spark_df = None
    
    try:
        spark_df = spark.readStream \
            .format('kafka') \
            .option('kafka.bootstrap.servers', 'localhost:9092') \
            .option('subscribe', 'feedback_sensor') \
            .option('startingOffsets', 'earliest') \
            .load()
        logging.info("kafka dataframe created successfully")
    except Exception as e:
        logging.warning("Kafka dataframe could not be created: " + str(e))

    return spark_df

def create_spark_session():
    spark = None

    try:
        spark = SparkSession.builder \
            .appName('SensorFeedbackStream') \
            .config("spark.jars.packages", "com.datastax.spark:spark-cassandra-connector_2.13:3.5.1," "org.apache.spark:spark-sql-kafka-0-10_2.13:4.0.0") \
            .config("spark.cassandra.connection.host", "localhost") \
            .getOrCreate()
        spark.sparkContext.setLogLevel("ERROR")
        logging.info("Session created succesfully: " + str(spark))
    except Exception as e:
        logging.error("Could not create spark session: " + str(e))

    return spark

if __name__ == "__main__":
    spark_session = create_spark_session()

    if spark_session is not None:
        spark_df = create_kafka_connection(spark_session)
        df = create_df_from_kafka(spark_df)
        logging.info(f"Found data: {df}")

