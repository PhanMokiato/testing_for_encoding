FROM ubuntu

#git
RUN apt update
RUN apt install -y git

#cmake
RUN apt-get update \
  && apt-get -y install build-essential \
  && apt-get install -y wget \
  && rm -rf /var/lib/apt/lists/* \
  && wget https://github.com/Kitware/CMake/releases/download/v3.27.9/cmake-3.27.9-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /opt/cmake-3.27.9 \
      && /tmp/cmake-install.sh --skip-license --prefix=/opt/cmake-3.27.9 \
      && rm /tmp/cmake-install.sh \
      && ln -s /opt/cmake-3.27.9/bin/* /usr/local/bin

#ninja
RUN apt-get update
RUN apt-get install -y ninja-build

#GCC compiler
RUN apt update
RUN apt install -y build-essential

#Clang compiler
RUN apt-get update
RUN apt-get install -y clang

#Valgrind
RUN apt-get update
RUN apt-get install -y valgrind

#Perf
RUN apt-get update
RUN apt install -y linux-tools-generic

#clang-tidy
RUN apt-get update
RUN apt install -y clang-tidy

#GTest
RUN apt-get update 
RUN apt install -y libgtest-dev