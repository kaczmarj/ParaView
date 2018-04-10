FROM debian:buster-slim as build

RUN apt-get -qq update \
    && apt-get install -yq --no-install-recommends \
        autoconf \
        bison \
        build-essential \
        cmake \
        flex \
        freeglut3-dev \
        git \
        libhdf5-serial-dev \
        libtool \
        libqt5x11extras5-dev \
        mesa-common-dev \
        mesa-utils \
        ninja-build \
        qt5-default \
        qttools5-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY . /tmp/paraview

ARG NPROC=1
WORKDIR /tmp/paraview-build
RUN cmake \
        -GNinja \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja \
        /tmp/paraview \
    && ninja -j"$NPROC" \
    && ninja install

FROM debian:buster-slim

COPY --from=build /usr/local /usr/local
