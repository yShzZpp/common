#ifndef _CTI_ROBOT_HTTP_UNIVERSAL_SERVICE_mClientHPP_
#define _CTI_ROBOT_HTTP_UNIVERSAL_SERVICE_mClientHPP_

#include <utility>
#include <cpprest/http_client.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <pplx/pplxtasks.h>

namespace cti_robot
{
namespace http
{
    class UniversalResult
    {
        private:

            bool success_ {false};
            int http_code_;
            std::string method_, url_, token_, error_, message_;
            web::json::value context_;
            std::vector<unsigned char> data_;

        public:

            UniversalResult(std::string method,
                            std::string url,
                            std::string token)
            {
                method_ = method;
                url_ = url;
                token_ = token;
            }

            void setContext(bool success,
                            int http_code,
                            std::string message,
                            std::string error,
                            web::json::value context);
            
            void setContext(bool success,
                            int http_code,
                            std::string message,
                            std::string error,
                            std::vector<unsigned char> data);

            bool isSuccess() { return success_; }

            int getHttpCode() { return http_code_; }

            const std::string& getMethod() { return method_; }

            const std::string& getUrl() { return url_; }

            const std::string& getAuthorizationToken() { return token_; }

            const std::string& getError() { return error_; }

            const std::string& getMessage() { return message_; }

            const web::json::value& getContext() { return context_; }

            const char* what() { return message_.data(); }

            const std::vector<unsigned char> getData() { return data_; }
    };

    class VerifyHttpResponse
    {
        public:

            void VerifyJsonResponse(UniversalResult & result, 
                                    web::http::http_response response);
            
            void VerifyDataResponse(UniversalResult & result,
                                    web::http::http_response response);
    };

    class ClientConfig : public web::http::client::http_client_config
    {
        public:
            ClientConfig();
    };

    class UniversalServiceClient
    {
        private:

            ClientConfig mClientConfig;
        
            std::string mDomain, mAuthorization;

            web::http::client::http_client mClient;
        
        public:

            UniversalServiceClient(std::string domain, std::string authorization)
                : mDomain(domain), mAuthorization(authorization), mClient(domain, mClientConfig)
            {
            }
            
            UniversalResult 
            requestWithJson(web::http::method method,
                    web::uri path,
                    std::unordered_map<std::string, std::string> queries = {},
                    web::json::value value = web::json::value::null(),
                    const pplx::cancellation_token& cancellationToken = pplx::cancellation_token::none());

            UniversalResult
            requestWithString(web::http::method method,
                    web::uri path,
                    std::unordered_map<std::string, std::string> queries = {},
                    std::string body = std::string(),
                    std::string type = std::string(),
                    const pplx::cancellation_token& cancellationToken = pplx::cancellation_token::none());
            
            UniversalResult 
            downloadFile(web::uri path,
                        std::array<long, 2> range = {-1, -1},
                        const pplx::cancellation_token& cancellation_token = pplx::cancellation_token::none());
    };

    class DownloadFileServiceClient
    {
        // private:

            // bool mStop {false};

        public:

            // void Stop() { mStop = true; }

            /* 不支持断点续传的下载方式*/
            bool Download(const std::string& uri, 
                          const std::string& path, 
                          const pplx::cancellation_token& cancellation_token = pplx::cancellation_token::none());

            bool SysDownload(const std::string& uri, const std::string& path);

            /* 支持断点续传的下载方式 */
            bool Download(const std::string& uri,
                          const std::string& file_save_path,
                          const std::string& md5,
                          const unsigned long& file_size,
                          const bool& stop,
                          const std::string& token = std::string()
                          );
    };
}
}

#endif //_CTI_ROBOT_HTTP_UNIVERSAL_SERVICE_mClientHPP_