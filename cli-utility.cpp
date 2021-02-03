#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <CLI/CLI.hpp>

typedef int (*FnPtr)(CLI::Option*);

struct util_cmd_option
{
    CLI::Option *option;
    FnPtr option_func;
};

class MCTP_Formatter : public CLI::Formatter 
{
    protected:
        std::string get_option_name(std::string str) const
        {
            std::map<std::string, std::string> cmd_table;
            cmd_table["--help"]        = "";
            cmd_table["--raw"]         = " <var1> <var2> <var3> [data]";
            cmd_table["--get-version"] = " <device>";
            return cmd_table[str];
        }

    public:
        MCTP_Formatter() : Formatter() {}
        std::string make_option_opts(const CLI::Option * opt) const override 
        { 
            return get_option_name(opt->get_name()); 
        }
};

static int
option_raw_command(CLI::Option* opt)
{
    std::vector<int> arg;
    opt->results(arg);
    
    for (const auto& data : arg)
        std::cout << data << " ";
    std::cout << std::endl;

    return 0;
}

static void
option_init_raw_command(std::vector<uint8_t> *rawData, CLI::App* app, std::vector<struct util_cmd_option>* opts)
{
    CLI::Option *option;
    std::string optionDescription = "Send raw command.";
    option = app->add_option("--raw", *rawData, optionDescription);
    option->expected(-3);
    opts->push_back({option, option_raw_command});
    return;
}

static int
option_get_version(CLI::Option* opt)
{
    std::cout << "Hello" << std::endl;
    return 0;
}

static void 
option_init_get_version (std::string *strData, CLI::App* app, std::vector<struct util_cmd_option>* opts)
{
    CLI::Option *option;
    std::string optionDescription;
    std::set<std::string> validate_devices = {"nic", "other"};

    optionDescription = "Get device mctp version.\n\t<device>: {";
    for (const auto& device : validate_devices)
        optionDescription += ", " + device;
    optionDescription += " }";
    optionDescription.erase(optionDescription.begin()+37);

    option = app->add_option("--get-version", *strData, optionDescription);
    option->check(CLI::IsMember(validate_devices));
    opts->push_back({option, option_get_version});

    return;
}

int 
main(int argc, char **argv) 
{
    CLI::App app("MCTP Utility");
    std::vector< struct util_cmd_option > opts;
    int ret = 0;
    
    // Setting "--help" message
    auto fmt = std::make_shared<MCTP_Formatter>();
    fmt->column_width(37);
    app.failure_message(CLI::FailureMessage::help);
    app.formatter(fmt);
    
    // Option init
    std::vector<uint8_t> rawCommand;
    option_init_raw_command(&rawCommand, &app, &opts);
    std::string deviceName;
    option_init_get_version(&deviceName, &app, &opts);

    // Parse and execute
    CLI11_PARSE(app, argc, argv);
    for (const auto& opt : opts)
        if (*(opt.option))
            ret |= opt.option_func(opt.option);

    return ret;
}