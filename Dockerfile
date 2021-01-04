ARG CACHEBUST=42
FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

# install prerequisites
RUN apt-get update &&\
    apt-get -y install --no-install-recommends \
        #apt-utils \
        libglu1-mesa-dev freeglut3-dev mesa-common-dev \  
        curl \
        libssl-dev \
        gnupg2 \
        software-properties-common \
        cmake \
        build-essential \
        ninja-build

# install Azure Kinect SDK
RUN curl -sSL https://packages.microsoft.com/keys/microsoft.asc | apt-key add -&&\
    apt-add-repository -y https://packages.microsoft.com/ubuntu/18.04/prod &&\
    apt-get update &&\
    ACCEPT_EULA=Y apt-get -y install libk4a1.4-dev

# invalidate cache to ensure copying of latest source code 
ARG CACHEBUST

# build telepresence capturing application
# TODO(laralex): check that Release enables optimization flags
COPY . /capture-src
RUN mkdir capture-build &&\
    cd capture-build &&\
    cmake ../capture-src -DCMAKE_BUILD_TYPE=Release -GNinja &&\
    ninja

# run unit tests
RUN /capture-build/release/all_tests
# container startup command
CMD /capture-build/release/capture-daemon