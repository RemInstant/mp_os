#include <sys/unistd.h>
#include <sys/msg.h>
#include <fstream>

#include <server_logger_builder.h>
#include <server_logger.h>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#define LINUX_SERVER_KEY 100
#define MAX_MSG_TEXT_SIZE 1024
#define LOG_PRIOR 2

struct msg_t
{
	long mtype;
	pid_t pid;
	size_t packet_id;
	size_t packet_cnt;
	char file_path[256];
	int severity;
	char mtext[MAX_MSG_TEXT_SIZE];
};

int main(
    int argc,
    char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);

    //return RUN_ALL_TESTS();
    
    logger_builder *builder = new server_logger_builder();
    
    logger* logger = builder
            ->add_console_stream(logger::severity::information)
            ->build();
    
    logger->information("INFO!!!");
    
    delete builder;
    delete logger;
    
    return 0;
    
    int mq_descryptor = msgget(LINUX_SERVER_KEY, 0666);
	
	// if (mq_descryptor == -1)
	// {
    //     std::cout << "Cannot open the message queue" << std::endl;
	// 	return 1;
	// }
	
    std::string config_path = "/home/remi/Code/VSCode/mp_os/logger/client_logger/tests/config_example.json";
    
    std::ifstream stream(config_path);
    
    nlohmann::json json_obj = nlohmann::json::parse(stream);
    
    json_obj = *(json_obj.find("wrapper")->find("wrapper"));
    
    std::string json_path = "wrapper:client_logger_builder.config";
    
    std::cout << json_obj << std::endl;
    
    return 0;
    
    msg_t msg;
    size_t const msg_max_size = sizeof(msg_t);
    
    msg.mtype = LOG_PRIOR;
    msg.pid = getpid();
    msg.packet_id = 1;
    msg.packet_cnt = 2;
    msg.severity = 2;
    
    strcpy(msg.file_path, "");
    strcpy(msg.mtext, "Chipi chipi");
    
	msgsnd(mq_descryptor, &msg, msg_max_size, 0);
    
    sleep(2);
    
    msg.packet_id = 2;
    strcpy(msg.mtext, " chapa chapa");
    
	msgsnd(mq_descryptor, &msg, msg_max_size, 0);
    
}