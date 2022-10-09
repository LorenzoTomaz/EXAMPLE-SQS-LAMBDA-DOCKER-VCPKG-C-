cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(hello LANGUAGES CXX)
include_directories(./)
link_directories(./)
file(GLOB SOURCES "*.cpp")

find_package(aws-lambda-runtime REQUIRED)
find_package(AWSSDK COMPONENTS s3 lambda)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES})
aws_lambda_package_target(${PROJECT_NAME})