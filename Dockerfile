FROM ubuntu:16.04

ENV DEBIAN_FRONTEND=noninteractive \
    HPGCC3=/hpgcc3 \
    PATH=$PATH:/hpgcc3/bin

RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y --force-yes \
    build-essential \
    gcc-arm-none-eabi \
    libelf-dev \
    make \
    python \
    python-pip \
    runit \
    unzip \
    wget &&\
    apt-get clean

COPY entrypoint.sh install_hpgcc.sh /hpgcc3/
COPY build.sh /

RUN bash /hpgcc3/install_hpgcc.sh && mkdir /work

WORKDIR /work
ENTRYPOINT ["/hpgcc3/entrypoint.sh"]
