#include <filesystem>

#include <not_implemented.h>
#include <client_logger.h>
#include <extra_utility.h>

#include "../include/client_logger_builder.h"

client_logger_builder::client_logger_builder():
    _format_string("[%s]: %m")
{ }

client_logger_builder::client_logger_builder(std::string const &format_string):
    _format_string(format_string)
{ }

logger_builder *client_logger_builder::set_format_string(
        std::string const& format_string)
{
    _format_string = format_string;
    
    return this;
}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    std::string abs_path = std::filesystem::weakly_canonical(stream_file_path).string();
    _configuration[abs_path].insert(severity);
    
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _configuration["console"].insert(severity);
    
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{   
    _format_string = extra_utility::get_from_json(
            configuration_file_path, configuration_path + ":format_string");
    
    std::string logger_files = extra_utility::get_from_json(
            configuration_file_path, configuration_path + ":logger_files");
    
    logger_files.pop_back();
    clear();
    
    size_t ind = 1;
    while (ind < logger_files.size())
    {
        size_t tmp_ind = std::min(logger_files.find(',', ind), logger_files.size());
        
        std::string record = logger_files.substr(ind, tmp_ind - ind);
        
        size_t delim_pos = record.find(':', 0);
        
        std::string path = record.substr(1, delim_pos - 2);
        logger::severity severity = client_logger::string_to_severity(
                record.substr(delim_pos + 2, record.size() - delim_pos - 3));    
        
        if (path == "console")
        {
            add_console_stream(severity);
        }
        else
        {
            add_file_stream(path, severity);
        }
        
        ind = tmp_ind + 1;
    }
    
    return this;
}

logger_builder *client_logger_builder::clear()
{
    _format_string = "[%s]: %m";
    _configuration.clear();
    
    return this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(_format_string, _configuration);
}
