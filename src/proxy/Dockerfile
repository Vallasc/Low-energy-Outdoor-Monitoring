FROM python:3.9.13-buster

# Create app directory
RUN mkdir -p /usr/src/app/
WORKDIR /usr/src/app

COPY . .
# Install app dependencies
RUN pip install -r requirements.txt

EXPOSE 9090/udp
EXPOSE 8082

CMD python ./src/main.py