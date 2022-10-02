## RUN PROJECT

```shell
docker-compose up --build
```

## GET LAMBDA LOGS

```shell
aws --endpoint-url=http://localhost:4566 logs tail /aws/lambda/exampleLambdaCPP_3 --follow
```

## GET LOG STREAMS

```shell
aws --endpoint-url=http://localhost:4566 logs describe-log-streams --log-group-name /aws/lambda/exampleLambdaCPP_3
```

## INVOKE LAMBDA

```shell
aws --endpoint-url=http://localhost:4566 lambda invoke --function-name exampleLambdaCPP_3 --cli-binary-format raw-in-base64-out --payload file://input.json response.json
```

## CALL SQS

```shell
aws sqs send-message --endpoint-url=http://localhost:4566 --queue-url http://localhost:4576/000000000000/testQueue --region us-east-1 --message-body 'text'
```

## CALL SNS

```shell
aws sns publish --endpoint-url=http://localhost:4566 --topic-arn arn:aws:sns:us-east-1:000000000000:testTopic --region us-east-1 --message 'text'
```

## INSTALL DEPENDENCIES (NOT MANDATORY - USE DOCKER INSTEAD)

```shell
./vcpkg/vcpkg install "aws-sdk-cpp[*]" --recurse
./vcpkg/vcpkg install aws-lambda-cpp
```
