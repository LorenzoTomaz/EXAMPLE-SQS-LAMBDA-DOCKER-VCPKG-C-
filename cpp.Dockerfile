FROM ubuntu:latest as cpp_lambda

ARG PORT=8000
ENV PORT=$PORT
WORKDIR /usr/src
COPY . .

RUN apt-get update -y
RUN apt-get install wget -y
RUN apt-get install curl zip unzip tar -y
RUN apt-get install git -y
RUN apt-get install build-essential -y
RUN apt-get install libcurl4-openssl-dev libssl-dev uuid-dev zlib1g zlib1g-dev libpulse-dev -y
RUN apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils -y

RUN export CC=gcc64
RUN export CXX=g++64

RUN apt-get install cmake -y

RUN rm -rf vcpkg && git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh && ./vcpkg/vcpkg install "aws-sdk-cpp[core,s3,lambda]" --recurse

RUN ./vcpkg/vcpkg install aws-lambda-cpp

RUN cd cpp-src && rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release  -DCMAKE_TOOLCHAIN_FILE=/usr/src/vcpkg/scripts/buildsystems/vcpkg.cmake && make && make aws-lambda-package-hello

# INSTALLING NODE DEPS

RUN apt-get install nodejs -y
RUN apt-get install npm -y
RUN cd node-src && npm install && zip -r node_lambdas.zip .

FROM localstack/localstack
# Copy lambdas.zip into the localstack directory
COPY --from=cpp_lambda /usr/src/cpp-src/build/hello.zip ./lambdas.zip
COPY --from=cpp_lambda /usr/src/node-src/node_lambdas.zip ./node_lambdas.zip