#include <fstream>
#include <vector>
#include <stdexcept>

#include <file_cannot_be_opened.h>

#include "../include/extra_utility.h"

std::string read_json_value(std::ifstream &stream)
{
    std::string result_data;
    
    char check_char;
    stream >> check_char;
    result_data.push_back(check_char);
    
    if (check_char == '"')
    {
        char prev_ch = '\0', ch = '\"';
        
        do
        {
            prev_ch = ch;
            stream >> std::noskipws >> ch;
            
            result_data.push_back(ch);
            
        } while (prev_ch == '\\' || ch != '\"');
        
        stream >> std::skipws;
    }
    else
    {
        bool in_word = false;
        int nesting = 1;
        char prev_ch = '{', ch;
        
        do
        {
            stream >> ch;
            
            result_data.push_back(ch);
            
            if (!in_word)
            {
                if (ch == '{')
                {
                    ++nesting;
                }
                else if (ch == '}')
                {
                    --nesting;
                }
            }
            
            if (prev_ch != '\\' && ch == '"')
            {
                in_word = !in_word;
            }
            
        } while (nesting);
    }
    
    stream >> check_char;
    
    if (check_char != ',')
    {
        stream.putback(check_char);
    }
    
    return result_data;
}

std::string extra_utility::get_from_json(std::string file_path, std::string data_path)
{
    std::ifstream stream(file_path);
    
    if (!stream.is_open())
    {
        throw file_cannot_be_opened(file_path);
    }
    
    std::vector<std::string> data_path_components;
    size_t ind = 0;
    
    while (ind < data_path.size())
    {
        size_t tmp_ind = std::min(data_path.find(':', ind), data_path.size());
        
        std::string component = "\"" + data_path.substr(ind, tmp_ind - ind) + "\":";
        if (component.size() == 0)
        {
            throw std::runtime_error("Invalid JSON data path");
        }
        
        data_path_components.push_back(std::move(component));
        
        
        ind = tmp_ind + 1;
    }
    
    for (size_t i = 0; i < data_path_components.size(); ++i)
    {
        char bracket;
        stream >> bracket;
        
        std::string data_word;
        stream >> data_word;
        
        while (data_word != data_path_components[i])
        {
            read_json_value(stream);
            
            stream >> data_word;
            
            if (data_word == "}")
            {
                throw std::runtime_error("JSON data not found");
            }
        }
    }
    
    return read_json_value(stream);
}
