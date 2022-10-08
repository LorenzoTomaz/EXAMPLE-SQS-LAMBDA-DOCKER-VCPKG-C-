#include <aws/lambda-runtime/runtime.h>
#include "aws/core/Aws.h"
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/core/http/curl/CurlHttpClient.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/ratelimiter/DefaultRateLimiter.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/platform/Environment.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <iostream>
#include <memory>
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/DeleteFunctionRequest.h>
#include <aws/lambda/model/GetFunctionRequest.h>
#include <aws/lambda/model/CreateFunctionRequest.h>
#include <aws/lambda/model/ListFunctionsRequest.h>
#include <aws/lambda/model/InvokeRequest.h>
#include <aws/lambda/model/AddPermissionRequest.h>
#include <aws/lambda/model/GetPolicyRequest.h>
#include <aws/lambda/model/GetFunctionConfigurationRequest.h>
#include <aws/lambda/model/RemovePermissionRequest.h>
#include <aws/lambda/model/CreateEventSourceMappingRequest.h>
#include <aws/lambda/model/GetEventSourceMappingRequest.h>
#include <aws/lambda/model/UpdateEventSourceMappingRequest.h>
#include <aws/lambda/model/DeleteEventSourceMappingRequest.h>
#include <aws/lambda/model/ResourceNotFoundException.h>
using namespace aws::lambda_runtime;
//using namespace Aws::Lambda;
//using namespace Aws::Lambda::Model;
std::string encode(Aws::String const& filename, Aws::String& output);
char const TAG[] = "LAMBDA_ALLOC";
invocation_response my_handler(invocation_request const& request)
{
    using namespace Aws::Utils::Json;
    JsonValue json(request.payload);
    AWS_LOGSTREAM_INFO(TAG, "LAMBDA ACTIVATED!");
    if (!json.WasParseSuccessful()) {
        return invocation_response::failure("Failed to parse input JSON", "InvalidJSON");
    }
    auto v = json.View();
    
    if (!v.ValueExists("Records")) {
        return invocation_response::failure("Invalid body", "InvalidJSON");
    }
    auto records = v.GetArray("Records");
    auto fRecord = records[0];
    auto body = fRecord.GetString("body");

    AWS_LOGSTREAM_INFO(TAG, "Body: " << body);
    
    Aws::Utils::Json::JsonValue json_response = Aws::Utils::Json::JsonValue();
    Aws::Utils::Json::JsonValue msg_response = Aws::Utils::Json::JsonValue();
    msg_response.WithString("message", "success!");
    json_response.WithObject("body", msg_response);

    return invocation_response::success(json_response.View().WriteCompact(), "application/json");
}

std::function<std::shared_ptr<Aws::Utils::Logging::LogSystemInterface>()> GetConsoleLoggerFactory()
{
    return [] {
        return Aws::MakeShared<Aws::Utils::Logging::ConsoleLogSystem>(
            "console_logger", Aws::Utils::Logging::LogLevel::Debug);
    };
}

int main()
{
    using namespace Aws;
    SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    options.loggingOptions.logger_create_fn = GetConsoleLoggerFactory();
    InitAPI(options);
    run_handler(my_handler);
    ShutdownAPI(options);
    return 0;
}



// try {
    //     const char ALLOCATION_TAG[] = "FunctionTest";
    //     std::shared_ptr<Aws::Lambda::LambdaClient> m_client;
    //     std::shared_ptr<Aws::Utils::RateLimits::RateLimiterInterface> m_limiter;
    //     m_limiter = Aws::MakeShared<Aws::Utils::RateLimits::DefaultRateLimiter<>>(ALLOCATION_TAG, 200000);
    //     Aws::Client::ClientConfiguration config;
    //     config.region = Aws::Region::US_EAST_1;
    //     config.scheme = Aws::Http::Scheme::HTTP;
    //     config.connectTimeoutMs = 30000;
    //     config.requestTimeoutMs = 30000;
    //     config.readRateLimiter = m_limiter;
    //     config.writeRateLimiter = m_limiter;
    //     m_client = Aws::MakeShared<Aws::Lambda::LambdaClient>(ALLOCATION_TAG, config);
    //     Aws::String simpleFunctionName = "exampleLambdaNode";
    //     //Aws::String simpleFunctionName = "arn:aws:lambda:us-east-1:000000000000:function:exampleLambdaNode"
    //     Aws::Lambda::Model::InvokeRequest invokeRequest;
    //     invokeRequest.SetFunctionName(simpleFunctionName);
    //     invokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);
        

    //         //to test proxy functionality, uncomment the next two lines.
    //     Aws::Lambda::Model::InvokeOutcome invokeOutcome = m_client->Invoke(invokeRequest);
    //     auto invoke_status = invokeOutcome.GetResult().GetStatusCode();
    //     std::stringstream response_payload;
    //     response_payload << invokeOutcome.GetResult().GetPayload().rdbuf();
    //     msg_response.WithInteger("invoke_status", invoke_status);
    //     msg_response.WithString("invoke_response", response_payload.str());
    // } 
    // catch(Aws::Client::AWSError<Aws::Lambda::LambdaErrors>& msg) {
    //     AWS_LOGSTREAM_ERROR(TAG, "error calling the node js lambda")
    //     std::cout << "error ???????????????<><><><><><><><<";
    //     msg_response.WithString("message", msg.GetMessage());
    //     json_response.WithObject("body", msg_response);
    //     return invocation_response::success(json_response.View().WriteCompact(), "application/json");
    // }
    // catch(...) {
    //     AWS_LOGSTREAM_ERROR(TAG, "error calling the node js lambda")
    //     std::cout << "error ???????????????<><><><><><><><<";
    //     msg_response.WithString("message", "error");
    //     json_response.WithObject("body", msg_response);
    //     return invocation_response::success(json_response.View().WriteCompact(), "application/json");
    // }