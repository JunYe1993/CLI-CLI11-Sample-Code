#include <iostream>
#include <vector>
#include "CLI11.hpp"
// #include <CLI/CLI.hpp>


#define UTIL_VERSION "0.0.1"


typedef int (*FnPtr)(CLI::Option*);

struct util_cmd_options
{
    CLI::Option *option;
    FnPtr option_func;
};

struct util_cmd_inits
{
    const char* opt_name;
    const char* opt_descript;
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
cmd_init(CLI::App* app, std::vector<struct util_cmd_options>* opts)
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
    CLI::App app("Calculator Utility");
    std::vector<struct util_cmd_options> opts;

    cmd_init(&app, &opts);

    CLI11_PARSE(app, argc, argv);
    
    for (size_t i = 0; i < opts.size(); i++) {
        if (*(opts[i].option)) {
            opts[i].option_func(opts[i].option);
        }
    }
    return 0;
}