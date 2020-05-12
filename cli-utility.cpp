#include <iostream>
#include <vector>
#include <CLI/CLI.hpp>

int
main(int argc, char **argv) 
{
    int  myint  = 0;
    bool mybool = false;
    std::string mystr = "";
    std::vector<int> int_vec;

    CLI::App app("Chassis Management Utility");
    app.add_option("--bool, -b", mybool, "This is a bool option");
    app.add_option("--int, -i", myint,   "This is an int option");
    app.add_option("--str, -s", mystr,   "This is an str option");
    app.add_option("--vec, -v", int_vec, "My vector option");
    app.failure_message(CLI::FailureMessage::help);
    
    CLI11_PARSE(app, argc, argv);

    for (size_t i = 0 ; i < int_vec.size(); i++)
    {
        std::cout << int_vec[i] << "\n";
    }
    
    return 0;
}