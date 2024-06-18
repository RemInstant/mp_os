#include <gtest/gtest.h>

#include <fraction.h>
#include <client_logger.h>
#include <operation_not_supported.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

TEST(positive_tests, test1)
{
    
}

int main(
    int argc,
    char **argv)
{
    fraction eps("1", "10000000000000000000000000000000000000000000000000000000000000000000");
    
	
	std::cout << (fraction("1", "2").ln(eps)) << std::endl;
	
	// std::cout << (fraction("01", "4").root(2, eps)) << std::endl;
	// std::cout << (fraction("03", "4").root(4, eps)) << std::endl;
	// std::cout << (fraction("04", "4").root(100, eps)) << std::endl;
	// std::cout << (fraction("07", "4").root(5, eps)) << std::endl;
	// std::cout << (fraction("-7", "3").root(3, eps)) << std::endl;
    
    // std::cout << (fraction("005", "4").ln(eps)) << std::endl;
	// std::cout << (fraction("013", "4").ln(eps)) << std::endl;
	// std::cout << (fraction("004", "4").ln(eps)) << std::endl;
	// std::cout << (fraction("133", "4").ln(eps)) << std::endl;
    
    // std::cout << (fraction("005", "4").log2(eps)) << std::endl;
	// std::cout << (fraction("013", "4").log2(eps)) << std::endl;
	// std::cout << (fraction("004", "4").log2(eps)) << std::endl;
	// std::cout << (fraction("133", "4").log2(eps)) << std::endl;
    
    // std::cout << (fraction("005", "4").lg(eps)) << std::endl;
	// std::cout << (fraction("013", "4").lg(eps)) << std::endl;
	// std::cout << (fraction("004", "4").lg(eps)) << std::endl;
	// std::cout << (fraction("133", "4").lg(eps)) << std::endl;
    
	// std::cout << (fraction("01", "4").sin(eps)) << std::endl;
	// std::cout << (fraction("03", "4").sin(eps)) << std::endl;
	// std::cout << (fraction("04", "4").sin(eps)) << std::endl;
	// std::cout << (fraction("07", "4").sin(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").sin(eps)) << std::endl;
    
	// std::cout << (fraction("01", "4").cos(eps)) << std::endl;
	// std::cout << (fraction("03", "4").cos(eps)) << std::endl;
	// std::cout << (fraction("04", "4").cos(eps)) << std::endl;
	// std::cout << (fraction("07", "4").cos(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").cos(eps)) << std::endl;
    
	// std::cout << (fraction("01", "4").tg(eps)) << std::endl;
	// std::cout << (fraction("03", "4").tg(eps)) << std::endl;
	// std::cout << (fraction("04", "4").tg(eps)) << std::endl;
	// std::cout << (fraction("07", "4").tg(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").tg(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").ctg(eps)) << std::endl;
	// std::cout << (fraction("03", "4").ctg(eps)) << std::endl;
	// std::cout << (fraction("04", "4").ctg(eps)) << std::endl;
	// std::cout << (fraction("07", "4").ctg(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").ctg(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").sec(eps)) << std::endl;
	// std::cout << (fraction("03", "4").sec(eps)) << std::endl;
	// std::cout << (fraction("04", "4").sec(eps)) << std::endl;
	// std::cout << (fraction("07", "4").sec(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").sec(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").cosec(eps)) << std::endl;
	// std::cout << (fraction("03", "4").cosec(eps)) << std::endl;
	// std::cout << (fraction("04", "4").cosec(eps)) << std::endl;
	// std::cout << (fraction("07", "4").cosec(eps)) << std::endl;
	// std::cout << (fraction("-7", "3").cosec(eps)) << std::endl;
    
    
    // std::cout << (fraction("01", "4").arcsin(eps)) << std::endl;
	// std::cout << (fraction("03", "4").arcsin(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arcsin(eps)) << std::endl;
	// std::cout << (fraction("-3", "4").arcsin(eps)) << std::endl;
	// std::cout << (fraction("-1", "3").arcsin(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").arccos(eps)) << std::endl;
	// std::cout << (fraction("03", "4").arccos(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arccos(eps)) << std::endl;
	// std::cout << (fraction("-3", "4").arccos(eps)) << std::endl;
	// std::cout << (fraction("-1", "3").arccos(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").arctg(eps)) << std::endl;
	// std::cout << (fraction("03", "4").arctg(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arctg(eps)) << std::endl;
	// std::cout << (fraction("-3", "4").arctg(eps)) << std::endl;
	// std::cout << (fraction("-1", "3").arctg(eps)) << std::endl;
    
    // std::cout << (fraction("01", "4").arcctg(eps)) << std::endl;
	// std::cout << (fraction("03", "4").arcctg(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arcctg(eps)) << std::endl;
	// std::cout << (fraction("-3", "4").arcctg(eps)) << std::endl;
	// std::cout << (fraction("-1", "3").arcctg(eps)) << std::endl;
    
    // std::cout << (fraction("05", "4").arcsec(eps)) << std::endl;
	// std::cout << (fraction("13", "4").arcsec(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arcsec(eps)) << std::endl;
	// std::cout << (fraction("-5", "4").arcsec(eps)) << std::endl;
	// std::cout << (fraction("-8", "3").arcsec(eps)) << std::endl;
    
    // std::cout << (fraction("05", "4").arccosec(eps)) << std::endl;
	// std::cout << (fraction("13", "4").arccosec(eps)) << std::endl;
	// std::cout << (fraction("04", "4").arccosec(eps)) << std::endl;
	// std::cout << (fraction("-5", "4").arccosec(eps)) << std::endl;
	// std::cout << (fraction("-8", "3").arccosec(eps)) << std::endl;
    
	
    // testing::InitGoogleTest(&argc, argv);
    
    // return RUN_ALL_TESTS();
}