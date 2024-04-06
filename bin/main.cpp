#include <lib/ArgParser.h>
#include <iostream>
#include <numeric>

struct Mode {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char** argv) {
    Mode mode;
    std::vector<int> values;

    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag("sum", "add args").StoreValue(mode.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(mode.mult);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    if (!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    if (mode.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(mode.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        std::cout << parser.HelpDescription();
        return 1;
    }

    return 0;

}
