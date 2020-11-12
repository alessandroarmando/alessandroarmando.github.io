FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    gpg \
    nano \
    less \
    wget

RUN adduser --uid 1001 --disabled-password --gecos '' armando
USER armando
WORKDIR /home/armando
RUN mkdir -p /home/armando/ImportedKeys && chown armando:armando /home/armando/ImportedKeys

