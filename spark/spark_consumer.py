import logging
from cassandra.auth import PlainTextAuthProvider
from cassandra.cluster import Cluster
from pyspark.sql import SparkSession
from pyspark.sql.functions import from_json, col

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

