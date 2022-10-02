FROM node:15 as lambda

ARG PORT=8000
ENV PORT=$PORT
WORKDIR /usr/src
COPY . .
# Install zip in container
RUN apt-get update
RUN apt-get install zip
# Enter the src directory, install dependencies, and zip the src directory in the container
RUN cd src && npm install && zip -r lambdas.zip .

RUN cd cpp-src

# RUN apt-get install gcc64-c++
# RUN apt-get install libcurl-devel
# RUN apt-get install cmake

RUN rm -rf aws-lambda-cpp

RUN git clone https://github.com/awslabs/aws-lambda-cpp.git
RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0.tar.gz &&  tar -zxvf cmake-3.20.0.tar.gz && cd cmake-3.20.0 && ./bootstrap && make && make install
RUN cd aws-lambda-cpp && rm -rf build && mkdir  build  && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF \
   -DCMAKE_INSTALL_PREFIX=~/out

FROM localstack/localstack
# Copy lambdas.zip into the localstack directory
COPY --from=lambda /usr/src/src/lambdas.zip ./lambdas.zip