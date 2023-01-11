#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <limits>
#include <mutex>
#include <sstream>


int get_int();
short get_short();
short get_unsigned_short();
std::string get_ip4();
bool in_range(int value, int first);

template <typename T, typename... Args>
bool in_range(int value, T first, Args... args);

template <typename... Args>
int get_int_in_range(Args... args);


enum class Command
{
    NONE,
    SEND,
    SEND_ALL,
    STOP_SERVER,
    PRINT_CONNECTIONS,
    DISCONNECT,
};



Command get_command_type(const std::string & str);
bool parse_send_command(const std::string & str , unsigned int & res_id , std::string & res_path);
bool parse_send_all_command(const std::string & str , std::string & res_path);
bool parse_disconnect_command(const std::string & str , unsigned int & res_id);

static std::mutex s_output_mutex;

#define LOG(header, value)                  \
    s_output_mutex.lock();                  \
    std::cout << "[" << header << "]"       \
              << " " << value << std::endl; \
    s_output_mutex.unlock();

#define PRINT(value)                 \
    s_output_mutex.lock();           \
    std::cout << value << std::endl; \
    s_output_mutex.unlock();
