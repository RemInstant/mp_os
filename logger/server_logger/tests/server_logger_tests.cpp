#include <fstream>

#include <server_logger_builder.h>
#include <server_logger.h>

#include <gtest/gtest.h>

int main(
    int argc,
    char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);

    //return RUN_ALL_TESTS();
    
    std::string path = "aboba";
    
    #ifdef _WIN32
    path = "D:\\Code\\VSCode\\mp_os\\logger\\server_logger\\tests\\win32_test_log.txt";
    #endif
    
    #ifdef __linux__
    path = "/home/remi/Code/VSCode/mp_os/logger/server_logger/tests/linux_test_log.txt";
    #endif
    
    logger_builder *builder = new server_logger_builder();
    
    logger* logger = builder
            ->add_console_stream(logger::severity::information)
            ->add_file_stream(path, logger::severity::information)
            ->add_file_stream(path, logger::severity::trace)
            ->add_file_stream("/", logger::severity::information)
            ->transform_with_configuration("cfg.json", "client_logger_builder.config")
            ->build();
    
    logger->information("INFO!!!");
    logger->trace("TRACE!!!");
    
    delete builder;
    delete logger;
}