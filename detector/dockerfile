FROM python:3.10.11-buster

ENV PYTHONUNBUFFERED=1

RUN apt-get update \
    && apt-get install ffmpeg libsm6 libxext6  -y \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD ["python", "manage.py", "runserver" ,"0.0.0.0:8000"]
