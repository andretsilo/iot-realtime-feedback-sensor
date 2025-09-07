# iot-realtime-feedback-sensor

This repository contains a full-stack data processing pipeline for a real-time IoT feedback sensor. The system captures user feedback from an Arduino-based device, ingests it through a FastAPI endpoint, processes it in real-time using Kafka and Spark, and stores it in a Cassandra database.

## Architecture

The data flows through the system as follows:

1.  **Arduino Device**: An IoT sensor with three buttons captures user feedback (LOW, MID, HIGH). Upon a button press, it sends a JSON payload containing the sensor ID, feedback level, timestamp, and timezone to a REST API.
2.  **Ingestion API (FastAPI)**: A Python-based API, running in a Docker container, receives the data from the sensor. It validates the request and publishes the data as a message to a Kafka topic.
3.  **Kafka**: A Kafka broker streams the feedback data from the ingestion API to the processing layer. The setup includes Confluent Schema Registry and Control Center for management and monitoring.
4.  **Spark Streaming**: A PySpark application consumes the data stream from the Kafka topic. It parses the JSON messages and prepares them for storage.
5.  **Cassandra**: The Spark job writes the processed data into a Cassandra database, making it available for further analysis and querying.

  <img width="1357" height="277" alt="diagram(1)" src="https://github.com/user-attachments/assets/3c3fb636-370b-436b-b141-8d4955ad2078" />


## Components

### Arduino Firmware (`/arduino`)

The firmware for the IoT device is responsible for:
-   Reading input from three physical buttons corresponding to different feedback levels.
-   Connecting to a WiFi network.
-   Synchronizing time with an NTP server to ensure accurate timestamps.
-   Loading configuration (e.g., WiFi credentials, API endpoint) from a `sensor.properties` file on an SD card.
-   Sending captured feedback data via an HTTP POST request to the ingestion API.
-   Storing the unique sensor ID in EEPROM for persistence.

### Ingestion API (`/kafka/app`)

A lightweight data ingestion service built with FastAPI.
-   Exposes a `/sensor` endpoint to receive `POST` requests from the IoT device.
-   Validates the structure and format of the incoming JSON payload.
-   Acts as a Kafka producer, sending the validated feedback data to the `feedback_sensor` topic.

### Spark Consumer (`/spark`)

A real-time data processing job using PySpark's Structured Streaming.
-   Consumes data from the `feedback_sensor` Kafka topic.
-   Parses the JSON data into a structured DataFrame.
-   Writes the processed data stream directly to a Cassandra table.

## Getting Started

### Prerequisites

-   Docker and Docker Compose
-   An Arduino board with WiFi capabilities (e.g., Arduino UNO R4 WiFi, or a board compatible with `WiFiS3.h`)
-   An SD card reader module for the Arduino
-   Three push buttons and three LEDs for the sensor hardware
-   Arduino IDE or an equivalent platform for flashing the firmware

### 1. Arduino Setup

1.  **Prepare the SD Card**: Create a file named `sensor.properties` in a `/config` directory on your SD card. Populate it with your specific configuration.

    **`/config/sensor.properties` Example:**
    ```properties
    # WiFi Credentials
    sensor.wifi.ssid=your_ssid
    sensor.wifi.password=your_password

    # Unique identifier for the sensor
    sensor.id=sensor-001

    # Ingestion API endpoint
    # Replace with the local IP of the machine running Docker
    sensor.api.host=192.168.1.100
    sensor.api.port=80
    sensor.api.endpoint=/sensor

    # Sensor behavior
    # LED on-time in seconds after press
    sensor.led.uptime=1
    # Cooldown period in seconds before another press is registered
    sensor.button.reset=4
    ```

2.  **Hardware Connection**: Connect the buttons, LEDs, and SD card reader to your Arduino according to the pin definitions in `arduino/main.ino`.

3.  **Flash Firmware**: Open the `arduino/main.ino` sketch in your Arduino IDE, install any required libraries (e.g., `ArduinoLog`, `WiFiS3`, `NTPClient`), and upload the code to your board.

### 2. Backend Infrastructure Setup

The entire backend stack (API, Kafka, Spark, Cassandra) is orchestrated using Docker Compose.

1.  **Clone the repository:**
    ```sh
    git clone https://github.com/andretsilo/iot-realtime-feedback-sensor.git
    cd iot-realtime-feedback-sensor
    ```

2.  **Start the services:**
    ```sh
    docker-compose up -d --build
    ```
    This command will build the ingestion API image and start all the services defined in `docker-compose.yml`.

### 3. Verify the Setup

-   **Ingestion API**: Should be accessible at `http://localhost:80`.
-   **Kafka Broker**: Listening on `localhost:9092`.
-   **Confluent Control Center**: View and manage your Kafka cluster at `http://localhost:9021`.
-   **Spark Master UI**: Monitor your Spark cluster at `http://localhost:9090`.
-   **Cassandra**: The database is available on `localhost:9042`.

Once the backend is running and the Arduino is powered on, pressing a feedback button will send data through the entire pipeline, ultimately landing in the Cassandra `feedback_streams.feedbacks` table.

## Project Structure

```
.
├── arduino/              # Arduino firmware and source files
├── kafka/                # FastAPI ingestion service and Kafka producer
│   └── app/
│       ├── config/       # Logger configuration
│       ├── ingest/       # FastAPI endpoint logic and models
│       └── stream/       # Kafka producer logic
├── spark/                # PySpark streaming application
│   └── spark_consumer.py # Consumes from Kafka and writes to Cassandra
├── Dockerfile            # Dockerfile for the FastAPI ingestion API
├── docker-compose.yml    # Defines the multi-container backend stack
└── requirements.txt      # Python dependencies for the ingestion API
