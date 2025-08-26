FROM python:3.11

WORKDIR /kafka/app

COPY ./requirements.txt /code/requirements.txt

RUN pip install --no-cache-dir --upgrade -r /code/requirements.txt

COPY ./kafka/app /code/app

CMD ["fastapi", "run", "/code/app/ingest/main.py", "--port", "80"]