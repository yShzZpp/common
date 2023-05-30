#include "share_handle_storage/UniversalServiceClient.hpp"
#include "share_handle_storage/CommonTool.hpp"
#include "share_handle_storage/BoostTool.hpp"
#include "share_handle_storage/MD5.hpp"
#include "spdlog/spdlog.h"

using namespace std::literals;

namespace cti_robot
{
namespace http
{
    void UniversalResult::setContext(bool success,
                                     int http_code,
                                     std::string message,
                                     std::string error,
                                     web::json::value context)
    {
        success_ = success;
        http_code_ = http_code;
        message_ = message;
        error_ = error;
        context_ = context;
        if (http_code_ == web::http::status_codes::Unauthorized)
        {
            message_ = "执行 "s + method_ + " " + url_ + " 时遇到[" + message_ + "] (" + std::to_string(http_code_) + " " + error_ + "), Token: [" + token_ + "], 更多: " + context_.serialize();
        }
        else
        {
            message_ = "执行 "s + method_ + " " + url_ + " 时遇到[" + message_ + "] (" + std::to_string(http_code_) + " " + error_ + "), 更多: " + context_.serialize();
        }
    }

    void UniversalResult::setContext(bool success,
                                     int http_code,
                                     std::string message,
                                     std::string error,
                                     std::vector<unsigned char> data)
    {
        success_ = success;
        http_code_ = http_code;
        message_ = message;
        error_ = error;
        data_ = data;

        message_ = "执行 "s + method_ + " " + url_ + " 时遇到[" + message_ + "] (" + std::to_string(http_code_) + " " + error_ + ")";
    }

    void VerifyHttpResponse::VerifyJsonResponse(UniversalResult & result,
                                                web::http::http_response response)
    {
        if (response.status_code() >= 500)
        {
            result.setContext(false, response.status_code(), "远程服务异常", response.reason_phrase(), response.extract_json().get());
        }
        else if (response.status_code() >= 400)
        {
            result.setContext(false, response.status_code(), "服务接口请求错误", response.reason_phrase(), response.extract_json().get());
        }
        else if (response.status_code() >= 300)
        {
            result.setContext(false, response.status_code(), "不支持的请求重定向", response.reason_phrase(), web::json::value(response.extract_string(true).get()));
        }
        else
        {
            if (response.headers()["Content-Type"].find("application/json") == std::string::npos)
            {
                auto content = response.extract_string(true).get();
                if (response.status_code() == 200) 
                {
                    result.setContext(false, 400, "响应内容无效", "无效的body类型", web::json::value(content));
                }
                else
                {
                    result.setContext(false, response.status_code(), "未知错误", "无效的body类型", web::json::value(content));
                }
            }
            else
            {
                result.setContext(true, response.status_code(), "请求成功", response.reason_phrase(), response.extract_json().get());
            }
        }
        return;
    }

    void VerifyHttpResponse::VerifyDataResponse(UniversalResult & result,
                                                web::http::http_response response)
    {
        if (response.status_code() >= 300)
        {
            result.setContext(false, response.status_code(), "请求失败", response.reason_phrase(), web::json::value(response.extract_string(true).get()));
        }
        else
        {
            result.setContext(true, response.status_code(), "请求成功", response.reason_phrase(), response.extract_vector().get());
        }
        return;
    }

    ClientConfig::ClientConfig()
    {
        set_timeout(std::chrono::seconds(15));
    }

    UniversalResult 
    UniversalServiceClient::requestWithJson(
                                    web::http::method method,
                                    web::uri path,
                                    std::unordered_map<std::string, std::string> queries,
                                    web::json::value body,
                                    const pplx::cancellation_token& cancellation_token)
    {
        web::uri_builder ub(path);
        for (const auto& item : queries)
        {
            if (!item.first.empty())
            {
                ub.append_query(item.first, item.second);
            }
        }

        UniversalResult result(method, ub.to_string(), mAuthorization);

        try 
        {
            web::http::http_request request(method);
            request.set_request_uri(ub.to_uri());

            if (not mAuthorization.empty())
            {
                request.headers().add("Authorization", mAuthorization);
            }
            if (body.is_object() || body.is_array())
            {
                request.set_body(body);
            }

            web::http::http_response response = mClient.request(request, cancellation_token).get();

            VerifyHttpResponse vr;
            vr.VerifyJsonResponse(result, std::move(response));
        }
        catch (std::exception & e)
        {
            result.setContext(false, 98, "catch捕获异常: ", e.what(), web::json::value::null());
        }

        return result;
    }

    UniversalResult 
    UniversalServiceClient::requestWithString(
                                    web::http::method method,
                                    web::uri path,
                                    std::unordered_map<std::string, std::string> queries,
                                    std::string body,
                                    std::string type,
                                    const pplx::cancellation_token& cancellation_token)
    {
        web::uri_builder ub(path);
        for (const auto& item : queries)
        {
            if (!item.first.empty())
            {
                ub.append_query(item.first, item.second);
            }
        }

        UniversalResult result(method, ub.to_string(), mAuthorization);

        try 
        {
            web::http::http_request request(method);
            request.set_request_uri(ub.to_uri());

            if (not mAuthorization.empty())
            {
                request.headers().add("Authorization", mAuthorization);
            }

            request.set_body(body, type);

            web::http::http_response response = mClient.request(request, cancellation_token).get();

            VerifyHttpResponse vr;
            vr.VerifyJsonResponse(result, std::move(response));
        }
        catch (std::exception & e)
        {
            result.setContext(false, 98, "catch捕获异常: ", e.what(), web::json::value::null());
        }

        return result;
    }

    UniversalResult 
    UniversalServiceClient::downloadFile(web::uri path,
                                         std::array<long, 2> range,
                                         const pplx::cancellation_token& cancellation_token)
    {
        std::string range_s;
        if (range[0] != -1 || range[1] != -1)
        {
            range_s = "bytes=";
            if (range[0] != -1)
            {
                range_s += std::to_string(range[0]);
            }
            range_s += '-';
            if (range[1] != -1)
            {
                range_s += std::to_string(range[1]);
            }
        }

        UniversalResult result(web::http::methods::GET, path.to_string(), mAuthorization);

        try 
        {
            web::http::http_request request(web::http::methods::GET);
            request.set_request_uri(path);

            if (not mAuthorization.empty())
            {
                request.headers().add("Authorization", mAuthorization);
            }

            if (!range_s.empty()) request.headers().add(web::http::header_names::range, range_s);

            web::http::http_response response = mClient.request(request, cancellation_token).get();

            VerifyHttpResponse vr;
            vr.VerifyDataResponse(result, std::move(response));
        }
        catch (std::bad_alloc& ex)
        {
            result.setContext(false, 96, "内容申请失败: ", ex.what(), web::json::value::null());
        }
        catch (std::invalid_argument& ex)
        {
            result.setContext(false, 96, "无效参数: ", ex.what(), web::json::value::null());
        }
        catch (std::exception & ex)
        {
            result.setContext(false, 98, "catch捕获异常: ", ex.what(), web::json::value::null());
        }
    }

    bool DownloadFileServiceClient::Download(const std::string& uri, 
                    const std::string& path, 
                    const pplx::cancellation_token& token)
    {
        if (token.is_canceled())
        {
            // throw pplx::task_canceled();
            return false;
        }

        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(uri);

        web::http::client::http_client client(uri.substr(0, uri.find('/', uri.find("://") + 3)));
        auto response = client.request(request, token).get();
        if (response.status_code() >= 300)
        {
            // throw std::runtime_error("response error - "s + response.reason_phrase());
            return false;
        }

        auto content = response.extract_vector().get();
        std::ofstream ofs(path, std::ios_base::binary);
        ofs.write(reinterpret_cast<char*>(&content[0]), content.size());
        ofs.close();
        return true;
    }

    bool DownloadFileServiceClient::SysDownload(const std::string& uri, const std::string& path)
    {
        std::string cmd = "curl -o "s + path + " \""s + uri + "\""s;
        return system(cmd.c_str()) == 0;
    }

    bool DownloadFileServiceClient::Download(const std::string& uri,
                                             const std::string& file_save_path,
                                             const std::string& md5,
                                             const unsigned long& file_total_size,
                                             const bool& stop,
                                             const std::string& token
                                             )
    {
        if (md5.empty())
        {
            SPDLOG_ERROR("[Download] md5值为空...");
            return false;
        }

        if (file_total_size == 0)
        {
            SPDLOG_ERROR("[Download] 文件总大小为0...");
            return false;
        }

        unsigned int file_current_size = 0;

        //检查文件
        if (cti_robot::shared_tool::boost_tool::isFileExist(file_save_path))
        {
            file_current_size = cti_robot::shared_tool::boost_tool::FileSize(file_save_path);
            if (file_current_size >= file_total_size)
            {
                //校验md5
                auto file_md5 = cti_robot::md5_tool::Generate(file_save_path);
                if (file_md5 != md5)
                {
                    SPDLOG_INFO("[Download] 已存在的文件md5校验失败,删除");
                    cti_robot::shared_tool::boost_tool::rmFile(file_save_path);
                    file_current_size = 0;
                }
                else
                {
                    SPDLOG_INFO("[Download] 文件已存在并且校验ok,无需下载");
                    return true;
                }
            }
        }
        
        SPDLOG_INFO("[Download] 文件需要继续下载, 当前已下载的文件大小 {}/{}", file_current_size, file_total_size);

        unsigned int part = 2 * 1024 * 1024;

        UniversalServiceClient client(uri.substr(0, uri.find('/', uri.find("://") + 3)), token);

        while (not stop)
        {
            if (cti_robot::shared_tool::boost_tool::isFileExist(file_save_path))
            {
                file_current_size = cti_robot::shared_tool::boost_tool::FileSize(file_save_path);
                if (file_current_size >= file_total_size)
                {
                    SPDLOG_INFO("[Download] 文件下载完成");
                    auto file_md5 = cti_robot::md5_tool::Generate(file_save_path);
                    if (file_md5 == md5)
                    {
                        SPDLOG_INFO("[Download] 文件校验成功");
                        return true;
                    }
                    else
                    {
                        SPDLOG_INFO("[Download] 文件校验失败: {} : {}", file_md5, md5);
                        return false;
                    }
                }
            }

            int download_part = file_current_size + part;
            if (download_part > file_total_size)
            {
                download_part = -1;
            }
            auto result = client.downloadFile(uri, {file_current_size, download_part});
            if (result.isSuccess())
            {
                cti_robot::shared_tool::common::AddContextToFile(file_save_path, result.getData());
            }
            else
            {
                SPDLOG_ERROR("[Download] 文件尝试下载失败, 原因: {}", result.getMessage());
                return false;
            }
        }

        SPDLOG_INFO("[Download] 下载过程中被打断.");
        return false;
    }
}
}
