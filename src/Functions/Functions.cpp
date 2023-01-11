#include "Functions.h"

int get_int()
{
    bool wrong_answer = 1;
    int value;
    std::string tmp;

    std::getline(std::cin, tmp);

    try
    {
        value = std::stoi(tmp);
        wrong_answer = 0;
    }
    catch (std::invalid_argument const &ex)
    {
        wrong_answer = 1;
    }
    catch (std::out_of_range const &ex)
    {
        wrong_answer = 1;
    }

    while (wrong_answer)
    {
        tmp = "";
        std::cout << "You entered wrong value, please try again : ";
        std::getline(std::cin, tmp);
        try
        {
            value = std::stoi(tmp);
            wrong_answer = 0;
        }
        catch (std::invalid_argument const &ex)
        {
            wrong_answer = 1;
        }
        catch (std::out_of_range const &ex)
        {
            wrong_answer = 1;
        }
    }

    return value;
}

short get_short()
{
    bool wrong_answer = 1;
    int value;
    std::string tmp;

    std::getline(std::cin, tmp);

    try
    {
        value = std::stoi(tmp);
        wrong_answer = 0;
    }
    catch (std::invalid_argument const &ex)
    {
        wrong_answer = 1;
    }
    catch (std::out_of_range const &ex)
    {
        wrong_answer = 1;
    }

    while (wrong_answer)
    {
        tmp = "";
        std::cout << "You entered wrong value, please try again : ";
        std::getline(std::cin, tmp);
        try
        {
            value = std::stoi(tmp);
            wrong_answer = 0;
        }
        catch (std::invalid_argument const &ex)
        {
            wrong_answer = 1;
        }
        catch (std::out_of_range const &ex)
        {
            wrong_answer = 1;
        }
        if (!(value >= std::numeric_limits<short>::min() && value <= std::numeric_limits<short>::max()))
        {
            wrong_answer = 1;
        }
    }

    return value;
}

short get_unsigned_short()
{
    bool wrong_answer = 1;
    int value;
    std::string tmp;

    std::getline(std::cin, tmp);

    try
    {
        value = std::stoi(tmp);
        wrong_answer = 0;
    }
    catch (std::invalid_argument const &ex)
    {
        wrong_answer = 1;
    }
    catch (std::out_of_range const &ex)
    {
        wrong_answer = 1;
    }

    while (wrong_answer)
    {
        tmp = "";
        std::cout << "You entered wrong value, please try again : ";
        std::getline(std::cin, tmp);
        try
        {
            value = std::stoi(tmp);
            wrong_answer = 0;
        }
        catch (std::invalid_argument const &ex)
        {
            wrong_answer = 1;
        }
        catch (std::out_of_range const &ex)
        {
            wrong_answer = 1;
        }
        if (!(value >= std::numeric_limits<unsigned short>::min() && value <= std::numeric_limits<unsigned short>::max()))
        {
            wrong_answer = 1;
        }
    }

    return value;
}

std::string get_ip4()
{
    std::string tmp;
    std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    std::getline(std::cin, tmp);

    if (std::regex_match(tmp, ipv4))
    {
        return tmp;
    }

    while (!std::regex_match(tmp, ipv4))
    {
        tmp = "";
        std::cout << "You entered wrong value, please try again : ";
        std::getline(std::cin, tmp);
    }
    return tmp;
}

bool in_range(int value, int first)
{
    return value == first;
}

template <typename T, typename... Args>
bool in_range(int value, T first, Args... args)
{
    if (value == first || in_range(value, args...))
        return 1;
    return 0;
}

template <typename... Args>
int get_int_in_range(Args... args)
{
    int answer = get_int();
    while (!in_range(answer, args...))
    {
        std::cout << "You entered wrong value, please try again : ";
        answer = get_int();
    }

    return answer;
}

Command get_command_type(const std::string &str)
{
    std::stringstream stream(str);

    std::string cmd;
    std::getline(stream, cmd, ' ');
    if (cmd == "send")
        return Command::SEND;
    else if (cmd == "sendall")
        return Command::SEND_ALL;
    else if (cmd == "stop")
        return Command::STOP_SERVER;
    else if (cmd == "print")
        return Command::PRINT_CONNECTIONS;
    else if (cmd == "disconnect")
        return Command::DISCONNECT;
    else
        return Command::NONE;
}

bool parse_send_command(const std::string &str, unsigned int &res_id, std::string &res_path)
{
    std::stringstream stream(str);
    std::string tmp;

    std::getline(stream, tmp, ' ');
    if (tmp != "send")
    {
        return 0;
    }

    std::regex integer_expr("(\\+|-)?[[:digit:]]+");

    std::getline(stream, tmp, ' ');
    if (!std::regex_match(tmp, integer_expr))
    {
        return 0;
    }
    res_id = atoi(tmp.c_str());

    std::getline(stream, tmp, ' ');
    if (!tmp.length())
    {
        return 0;
    }
    res_path = std::move(tmp);

    return 1;
}

bool parse_send_all_command(const std::string & str , std::string & res_path)
{
    std::stringstream stream(str);
    std::string tmp;

    std::getline(stream, tmp, ' ');
    if (tmp != "sendall")
    {
        return 0;
    }
    tmp="";
    std::getline(stream, tmp);
    if (!tmp.length())
    {
        return 0;
    }
    res_path = std::move(tmp);

    return 1;

}

bool parse_disconnect_command(const std::string &str, unsigned int &res_id)
{
    std::stringstream stream(str);
    std::string tmp;

    std::getline(stream, tmp, ' ');
    if (tmp != "disconnect")
    {
        return 0;
    }

    std::regex integer_expr("(\\+|-)?[[:digit:]]+");

    std::getline(stream, tmp);
    if (!std::regex_match(tmp, integer_expr))
    {
        return 0;
    }
    res_id = atoi(tmp.c_str());

    return 1;
}
