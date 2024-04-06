#include "ArgParser.h"
#include <utility>
using namespace ArgumentParser;

ArgParser::ArgParser(std::string _my_parser_name) : parser_name(std::move(_my_parser_name)) {}

bool ArgParser::Parse(std::vector<std::string> split_string) {
    if (split_string.empty()) {
        return true;
    }
    if (split_string.size() == 1) {
        if (arguments.empty()) {
            return true;
        }
        for (auto & arg : arguments) {
            if (!arg->IsDefaultValue()) {
                return false;
            }
        }

        return true;
    }
    for (int i = 1; i < split_string.size(); i++) {
        std::string current_value;
        if (split_string[i][0] == '-') {
            if (split_string.size() == 1) {
                continue;
            }
            if (split_string[i][1] == '-') {
                std::string current_parameter;
                size_t index = split_string[i].find('=');
                if (index == std::string::npos) {
                    current_parameter = split_string[i].substr(2);
                    if (is_help) {
                        if (current_parameter == long_name_help) {
                            use_have_argument = true;
                            std::cout << HelpDescription();
                            return true;
                        }
                    } else {
                        bool flag_found = false;
                        for (auto & arg : arguments) {
                            if (dynamic_cast<FlagArgument*>(arg) && current_parameter == arg->GetLongName()) {
                                dynamic_cast<FlagArgument*>(arg)->value = true;
                                for (int k = 0; k < dynamic_cast<FlagArgument*>(arguments[k])->store_values.size(); k++) {
                                    *(dynamic_cast<FlagArgument*>(arg)->store_values[k])= true;
                                }
                                flag_found = true;
                                break;
                            }
                        }
                        if (flag_found) {
                            continue;
                        } else {
                            return false;
                        }
                    }
                } else {
                    current_parameter = split_string[i].substr(2, index - 2);
                    if (index >= split_string[i].length() - 1) {
                        return false;
                    } else {
                        current_value = split_string[i].substr(index + 1);
                    }
                    for (auto & arg : arguments) {
                        if (dynamic_cast<IntegerArgument*>(arg) && current_parameter == arg->GetLongName()) {
                            if (!arg->IsMultiValue()) {
                                dynamic_cast<IntegerArgument*>(arg)->value = std::stoi(current_value);
                                dynamic_cast<IntegerArgument*>(arg)->is_set = true;
                                for (auto & store_value : dynamic_cast<IntegerArgument*>(arg)->store_values) {
                                    *store_value = std::stoi(current_value);
                                }
                            } else {
                                dynamic_cast<IntegerArgument*>(arg)->multi_values.push_back(std::stoi(current_value));
                                for (auto & multi_store_value : dynamic_cast<IntegerArgument*>(arg)->multi_store_values) {
                                    (*multi_store_value).push_back(std::stoi(current_value));
                                }
                            }
                            break;
                        }
                    }
                    for (auto & arg : arguments) {
                        if (dynamic_cast<StringArgument*>(arg) && current_parameter == arg->GetLongName()) {
                            if (!arg->IsMultiValue()) {
                                dynamic_cast<StringArgument*>(arg)->value = current_value;
                                dynamic_cast<StringArgument*>(arg)->is_set = true;
                                for (auto & store_value : dynamic_cast<StringArgument*>(arg)->store_values) {
                                    *store_value = current_value;
                                }
                            } else {
                                dynamic_cast<StringArgument*>(arg)->multi_values.push_back(current_value);
                                for (auto & multi_store_value : dynamic_cast<StringArgument*>(arg)->multi_store_values) {
                                    (*multi_store_value).push_back(current_value);
                                }
                            }
                            break;
                        }
                    }
                }
            } else {
                char current_parameter = split_string[i][1];
                if (is_help) {
                    if (current_parameter == short_name_help) {
                        use_have_argument = true;
                        std::cout << HelpDescription();
                        return true;
                    } else {
                        return false;
                    }
                }
                bool flag_found = false;
                for (auto & arg : arguments) {
                    if (dynamic_cast<FlagArgument*>(arg) && arg->GetShortName() == current_parameter) {
                        dynamic_cast<FlagArgument*>(arg)->value = true;
                        for (auto & store_value : dynamic_cast<FlagArgument*>(arg)->store_values) {
                            *store_value = true;
                        }
                        flag_found = true;
                        break;
                    }
                }
                if (flag_found) {
                    for (int l = 2; l < split_string[i].length(); l++) {
                        for (auto & arg : arguments) {
                            if (dynamic_cast<FlagArgument*>(arg) && arg->GetShortName() == split_string[i][l]) {
                                dynamic_cast<FlagArgument*>(arg)->value = true;
                                for (auto & store_value : dynamic_cast<FlagArgument*>(arg)->store_values) {
                                    *store_value = true;
                                }
                                break;
                            }
                        }
                    }
                    continue;
                }

                if (split_string[i].length() == 2 && !flag_found) { //формат, когда не задан флаг
                    return false;
                } else if (split_string[i].length() >=3 && split_string[i][2] != '=') { //формат, когда не задано значение (=)
                    return false;
                } else if (split_string[i].length() < 4) { //формат когда задано "=", но не задано значение
                    return false;
                } else {
                    current_value = split_string[i].substr(3);
                }
                for (auto & arg : arguments) {
                    if (dynamic_cast<IntegerArgument*>(arg) && current_parameter == arg->GetShortName()) {
                        if (!arg->IsMultiValue()) {
                            dynamic_cast<IntegerArgument*>(arg)->value = std::stoi(current_value);
                            dynamic_cast<IntegerArgument*>(arg)->is_set = true;
                            for (auto & store_value : dynamic_cast<IntegerArgument*>(arg)->store_values) {
                                *store_value = std::stoi(current_value);
                            }
                        } else {
                            dynamic_cast<IntegerArgument*>(arg)->multi_values.push_back(std::stoi(current_value));
                            for (auto & multi_store_value : dynamic_cast<IntegerArgument*>(arg)->multi_store_values) {
                                (*multi_store_value).push_back(std::stoi(current_value));
                            }
                        }
                        break;
                    }
                }
                for (auto & arg : arguments) {
                    if (dynamic_cast<StringArgument*>(arg) && current_parameter == arg->GetShortName()) {
                        if (!arg->IsMultiValue()) {
                            dynamic_cast<StringArgument*>(arg)->value = current_value;
                            dynamic_cast<StringArgument*>(arg)->is_set = true;
                            for (auto & store_value : dynamic_cast<StringArgument*>(arg)->store_values) {
                                *store_value = current_value;
                            }
                        } else {
                            dynamic_cast<StringArgument*>(arg)->multi_values.push_back(current_value);
                            for (auto & multi_store_value : dynamic_cast<StringArgument*>(arg)->multi_store_values) {
                                (*multi_store_value).push_back(current_value);
                            }
                        }
                        break;
                    }
                }

            }
        } else {
            for (auto & arg : arguments) {
                if (dynamic_cast<IntegerArgument*>(arg) && arg->IsPositional()) {
                    if (arg->IsMultiValue()) {
                        dynamic_cast<IntegerArgument*>(arg)->multi_values.push_back(std::stoi(split_string[i]));
                        for (auto & multi_store_value : dynamic_cast<IntegerArgument*>(arg)->multi_store_values) {
                            (*multi_store_value).push_back(std::stoi(split_string[i]));
                        }
                    } else {
                        dynamic_cast<IntegerArgument*>(arg)->value = std::stoi(split_string[i]);
                        for (auto & store_value : dynamic_cast<IntegerArgument*>(arg)->store_values) {
                            *store_value = std::stoi(split_string[i]);
                        }
                    }
                }
                break;
            }
        }
    }
    for (auto & arg : arguments) {
        if (dynamic_cast<IntegerArgument*>(arg) && arg->IsMultiValue()) {
            if (dynamic_cast<IntegerArgument*>(arg)->multi_values.size() < dynamic_cast<IntegerArgument*>(arg)->GetArgsCounter()) {
                return false;
            }
        }
    }
    for (auto & arg : arguments) {
        if (dynamic_cast<StringArgument*>(arg) && arg->IsMultiValue()) {
            if (dynamic_cast<StringArgument*>(arg)->multi_values.size() < dynamic_cast<StringArgument*>(arg)->GetArgsCounter()) {
                return false;
            }
        }
    }
    for (auto& arg : arguments) {
        if (!dynamic_cast<IntegerArgument*>(arg)) {
            continue;
        }
        if (!dynamic_cast<IntegerArgument*>(arg)->GetDefaultValue() && !dynamic_cast<IntegerArgument*>(arg)->is_set) {
            return false;
        }
        if (arg->IsMultiValue() && dynamic_cast<IntegerArgument*>(arg)->GetArgsCounter() > dynamic_cast<IntegerArgument*>(arg)->GetMultiValues().size()) {
            return false;
        }
    }
    for (auto& arg : arguments) {
        if (!dynamic_cast<StringArgument*>(arg)) {
            continue;
        }
        if (!arg->IsDefaultValue() && !dynamic_cast<StringArgument*>(arg)->is_set) {
            return false;
        }
        if (arg->IsMultiValue() && dynamic_cast<StringArgument*>(arg)->GetArgsCounter() > dynamic_cast<StringArgument*>(arg)->GetMultiValues().size()) {
            return false;
        }
    }
    return true;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> parser;
    for (int i = 1; i < argc; i++) {
        parser.emplace_back(argv[i]);
    }

    return Parse(parser);
}

StringArgument& ArgParser::AddStringArgument(const std::string& add_argument, const std::string& description) {
    auto* new_argument = new StringArgument(add_argument);
    new_argument->description = description;
    arguments.push_back(new_argument);

    return dynamic_cast<StringArgument &>(*arguments[arguments.size() - 1]);
}

StringArgument& ArgParser::AddStringArgument(const char& short_name, const std::string& long_name, const std::string& description) {
    auto* new_argument = new StringArgument(short_name, long_name, description);
    arguments.push_back(new_argument);

    return dynamic_cast<StringArgument &>(*arguments[arguments.size() - 1]);
}

IntegerArgument& ArgParser::AddIntArgument(const std::string& add_argument, const std::string& description) {
    auto* new_argument = new IntegerArgument(add_argument);
    new_argument->description = description;
    arguments.push_back(new_argument);

    return dynamic_cast<IntegerArgument &>(*arguments[arguments.size() - 1]);
}

IntegerArgument& ArgParser::AddIntArgument(const char& short_name, const std::string& long_name, const std::string& description) {
    auto* new_argument = new IntegerArgument(short_name, long_name, description);
    arguments.push_back(new_argument);

    return dynamic_cast<IntegerArgument &>(*arguments[arguments.size() - 1]);
}

FlagArgument& ArgParser::AddFlag(const std::string& add_argument, const std::string& description) {
    auto* new_argument = new FlagArgument(add_argument);
    new_argument->description = description;
    arguments.push_back(new_argument);

    return dynamic_cast<FlagArgument &>(*arguments[arguments.size() - 1]);
}

FlagArgument& ArgParser::AddFlag(const char& short_name, const std::string& long_name, const std::string& description) {
    auto* new_argument = new FlagArgument(short_name, long_name, description);
    arguments.push_back(new_argument);

    return dynamic_cast<FlagArgument &>(*arguments[arguments.size() - 1]);
}

std::string& ArgParser::GetStringValue(const std::string& arg) {
    std::string base;
    for (auto & argument : arguments) {
        if (argument->GetLongName() == arg) {
            if (argument->IsDefaultValue()) {
                return dynamic_cast<StringArgument*>(argument)->default_value;
            } else {
                return dynamic_cast<StringArgument*>(argument)->value;
            }
        }
    }

    return base;
}

std::string& ArgParser::GetStringValue(const std::string& arg, size_t index) {
    std::string base;
    for (auto & argument : arguments) {
        if (argument->GetLongName() == arg) {
            return dynamic_cast<StringArgument*>(argument)->multi_values[index];
        }
    }

    return base;
}

int ArgParser::GetIntValue(const std::string& arg) {
    int base = 0;
    for (auto & argument : arguments) {
        if (argument->GetLongName() == arg) {
            if (argument->IsDefaultValue()) {
                return dynamic_cast<IntegerArgument*>(argument)->GetDefaultValue();
            } else {
                return dynamic_cast<IntegerArgument*>(argument)->GetValue();
            }
        }
    }

    return base;
}

int ArgParser::GetIntValue(const std::string& arg, size_t index) {
    int base = 0;
    for (auto & argument : arguments) {
        if (argument->GetLongName() == arg) {
            return dynamic_cast<IntegerArgument*>(argument)->multi_values[index];
        }
    }

    return base;
}

bool ArgParser::GetFlag(const std::string& arg) {
    for (auto & argument : arguments) {
        if (argument->GetLongName() == arg) {
            return true;
        }
    }

    return false;
}

void ArgParser::AddHelp(const char& short_name, const std::string& long_name, const std::string& description) {
    is_help = true;
    long_name_help = long_name;
    short_name_help = short_name;
    description_help = description;
}

const std::string ArgParser::HelpDescription() {
    std::string result;
    result += parser_name;
    result += "\n";
    result += "Some Description about program\n";
    result += "\n";
    for (auto & arg : arguments) {
        if (dynamic_cast<IntegerArgument*>(arg)) {
            result += "     ";
            result += "--";
            result += dynamic_cast<IntegerArgument*>(arg)->GetLongName();
            result += ",  ";
            result += dynamic_cast<IntegerArgument*>(arg)->GetDescription();
            result += "\n";
        } else if (dynamic_cast<StringArgument*>(arg)) {
            result += '-';
            result += dynamic_cast<StringArgument*>(arg)->GetShortName();
            result += ",  ";
            result += "--";
            result += dynamic_cast<StringArgument*>(arg)->GetLongName();
            result += ",  ";
            result += dynamic_cast<StringArgument*>(arg)->GetDescription();
            result += "\n";
        } else if (dynamic_cast<FlagArgument*>(arg)) {
            result += '-';
            result += dynamic_cast<FlagArgument*>(arg)->GetShortName();
            result += ",  ";
            result += "--";
            result += dynamic_cast<FlagArgument*>(arg)->GetLongName();
            result += ",  ";
            result += dynamic_cast<FlagArgument*>(arg)->GetDescription();
            result += "\n";
        }
    }

    return result;
}

bool ArgParser::Help() {

    return true;
}