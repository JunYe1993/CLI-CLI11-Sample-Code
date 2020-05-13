#include <iostream>
#include <vector>
#include <map>
#include "CLI11.hpp"
// #include <CLI/CLI.hpp>


#define UTIL_VERSION "0.0.1"

typedef int (*FnPtr)(CLI::Option*);

struct util_cmd_option
{
    CLI::Option *option;
    FnPtr option_func;
};

class MyFormatter : public CLI::Formatter 
{
    protected:
        std::string get_option_name(std::string str) const
        {
            std::map<std::string, std::string> cmd_table;
            cmd_table["--help"]    = "";
            cmd_table["--version"] = "";
            cmd_table["--add"]     = " <a> <b>";
            return cmd_table[str];
        }

    public:
        MyFormatter() : Formatter() {}
        std::string make_option_opts(const CLI::Option * opt) const override 
        { 
            return get_option_name(opt->get_name()); 
        }
};

static int
version(CLI::Option* opt)
{
    std::cout << UTIL_VERSION << "\n";
    return 0;
}

static int 
add(CLI::Option* opt)
{
    std::vector<std::string> arg = opt->results();
    std::cout << std::stoi(arg[0]) + std::stoi(arg[1]) << "\n";
    return 0;
}

static int
cmd_init(CLI::App* app, std::vector<struct util_cmd_option>* opts)
{
    CLI::Option *option;

    option = app->add_flag("-v, --version", "Show program version");
    opts->push_back({option, version});

    option = app->add_option("-a, --add", "Add two inputs");
    option->expected(2);
    option->check(CLI::Validator(CLI::Range(0, 100)).application_index(0));
    option->check(CLI::Validator(CLI::Range(0, 200)).application_index(1));
    opts->push_back({option, add});

    return 0;
}

int 
main(int argc, char **argv) 
{
    auto fmt = std::make_shared<MyFormatter>();
    fmt->column_width(20);

    CLI::App app("Calculator Utility");
    app.failure_message(CLI::FailureMessage::help);
    app.formatter(fmt);

    std::vector< struct util_cmd_option > opts;
    cmd_init(&app, &opts);

    CLI11_PARSE(app, argc, argv);

    for (size_t i = 0; i < opts.size(); i++) {
        if (*(opts[i].option)) {
            opts[i].option_func(opts[i].option);
        }
    }
    return 0;
}
