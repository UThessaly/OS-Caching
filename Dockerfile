FROM conanio/gcc9 AS builder

USER root

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y build-essential cmake libgtest-dev libboost-test-dev && rm -rf /var/lib/apt/lists/* 

ARG SRC=/src
ARG BUILD=/src/build

RUN mkdir -p ${SRC} 

ADD . ${SRC}

RUN cd ${SRC} \
    && mkdir ${BUILD} \
    && cd build

RUN cd ${BUILD} \
    && cmake ${SRC} \
    -DCMAKE_BUILD_TYPE=Release 

RUN /usr/bin/cmake --build ${BUILD} --config Debug --target all -- -j 14 ${SRC}

RUN ls ${BUILD}

FROM ubuntu:20.04

USER root

ENV DEBIAN_FRONTEND noninteractive

WORKDIR /usr/app

ARG BUILD=/src/build

COPY --from=builder ${BUILD}/bin/caching_server /usr/app/caching_server
COPY test.yaml /usr/app/test.yaml
COPY config.yaml /usr/app/config.yaml

ENTRYPOINT [ "/usr/app/caching_server" ]
