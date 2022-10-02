#include <aws/lambda-runtime/runtime.h>
#include "aws/core/Aws.h"
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/platform/Environment.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <iostream>
#include <memory>
using namespace aws::lambda_runtime;
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