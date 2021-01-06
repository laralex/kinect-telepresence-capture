ARG CACHEBUST=42

FROM ubuntu:18.04

# install prerequisites
RUN apt-get update &&\
    apt-get -y install --no-install-recommends \
        libglu1-mesa-dev freeglut3-dev mesa-common-dev \  
        curl \
        libssl-dev \
        gnupg2 \
        software-properties-common \
        cmake \
        build-essential

# install Azure Kinect SDK and Azure Kinect Body Tracking SDK
ARG DEBIAN_FRONTEND=noninteractive
RUN curl -sSL https://packages.microsoft.com/keys/microsoft.asc | apt-key add -&&\
    apt-add-repository -y https://packages.microsoft.com/ubuntu/18.04/prod &&\
    apt-get update
RUN yes accept | ACCEPT_EULA=Y apt-get -y --no-install-recommends install libk4a1.4-dev

# FIXME(laralex): As of now k4a v1.3 is required for body tracking, 
# but it fails to install without accepting EULA agreement, so wait until
# maintainers switch to v1.4 ...
# RUN yes accept | ACCEPT_EULA=Y apt-get -y --no-install-recommends install libk4abt1.0-dev &&\
#    apt-get clean && rm -rf /var/lib/apt/lists/*

# invalidate cache to ensure copying of latest source code 
ARG CACHEBUST

# build telepresence capturing application
# TODO(laralex): check that Release enables optimization flags
COPY . /capture-src
RUN mkdir capture-build &&\
    cd capture-build &&\
    cmake ../capture-src -DCMAKE_BUILD_TYPE=Release -DENABLE_AZURE_KINECT_BODYTRACKING=OFF -G"Unix Makefiles" &&\
    make -j5

# run unit tests
RUN /capture-build/release/all_tests

# container startup command
CMD /capture-build/release/ktp_capture_daemon