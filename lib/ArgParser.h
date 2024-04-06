#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace ArgumentParser {
    struct BaseArgument {
        virtual bool IsMultiValue() = 0;
        virtual bool IsDefaultValue() = 0;
        virtual bool IsStoreValues() = 0;
        virtual bool IsPositional() = 0;
        virtual std::string GetLongName() = 0;
        virtual char GetShortName() = 0;
        virtual std::string GetDescription() = 0;
    };
    class ArgParser;
    class StringArgument : public BaseArgument {
        friend ArgParser;
    private:
        const char short_name = '\0';
        const std::string long_name;
        std::string description;
        bool is_multi_value = false;
        size_t args_counter = 0;
        std::vector<std::string> multi_values;
        bool is_default_value = false;
        std::string default_value;
        std::string value;
        bool is_store_value = false;
        bool is_set = false;
        std::vector<std::string*> store_values;
        std::vector<std::vector<std::string>*> multi_store_values;
        bool is_positional = false;

    public:
        explicit StringArgument(std::string _name) : long_name(std::move(_name)) {};
        StringArgument(char _s_name, std::string _l_name, std::string _description) : short_name(_s_name), long_name(std::move(_l_name)), description(std::move(_description)) {};
        StringArgument& MultiValue(size_t min_count = 0) {
            is_multi_value = true;
            args_counter = min_count;

            return *this;
        }
        StringArgument& Default(const std::string& arg_name) {
            is_default_value = true;
            default_value = arg_name;

            return *this;
        }
        StringArgument& StoreValue(std::string& arg_name) {
            is_store_value = true;
            is_set = true;
            arg_name = value;
            store_values.push_back(&arg_name);

            return *this;
        }
        StringArgument& Positional() {
            is_positional = true;

            return *this;
        }
        bool IsMultiValue() override {

            return is_multi_value;
        }
        bool IsDefaultValue() override {

            return is_default_value;
        }
        bool IsStoreValues() override {

            return is_store_value;
        }
        bool IsPositional() override {

            return is_positional;
        }
        std::string GetLongName() override {

            return long_name;
        }
        char GetShortName() override {

            return short_name;
        }
        std::vector<std::string> GetMultiValues() {

            return multi_values;
        }
        size_t GetArgsCounter() {

            return args_counter;
        }
        std::string GetDescription() override {

            return description;
        }
    };
    class IntegerArgument : public BaseArgument {
        friend ArgParser;
    private:
        const char short_name = '\0';
        const std::string long_name;
        std::string description;
        bool is_multi_value = false;
        size_t args_counter = 0;
        std::vector<int> multi_values;
        bool is_default_value = false;
        int value;
        int default_value;
        bool is_store_value = false;
        bool is_set = false;
        std::vector<int*> store_values;
        std::vector<std::vector<int>*> multi_store_values;
        bool is_positional = false;

    public:
        explicit IntegerArgument(std::string _name) : long_name(std::move(_name)) {};
        IntegerArgument(char _s_name, std::string _l_name, std::string _description) : short_name(_s_name), long_name(std::move(_l_name)), description(std::move(_description)) {};

        IntegerArgument& MultiValue(size_t min_count = 0) {
            is_multi_value = true;
            args_counter = min_count;

            return *this;
        }
        IntegerArgument& Positional() {
            is_positional = true;

            return *this;
        }
        IntegerArgument& StoreValues(std::vector<int>& args) {
            is_store_value = true;
            is_set = true;
            multi_store_values.push_back(&args);

            return *this;
        }
        bool IsMultiValue() override {

            return is_multi_value;
        }
        bool IsDefaultValue() override {

            return is_default_value;
        }
        bool IsPositional() override {

            return is_positional;
        }
        bool IsStoreValues() override {

            return is_store_value;
        }

        std::string GetLongName() override {

            return long_name;
        }
        char GetShortName() override {

            return short_name;
        }
        int GetDefaultValue() {

            return default_value;
        }
        int GetValue() {

            return value;
        }
        std::vector<int> GetMultiValues()  {

            return multi_values;
        }
        size_t GetArgsCounter() {

            return args_counter;
        }
        std::string GetDescription() override {

            return description;
        }
    };
    class FlagArgument : public BaseArgument {
        friend ArgParser;
    private:
        const char short_name = '\0';
        const std::string long_name;
        std::string description;
        bool is_default_value = false;
        bool value;
        bool is_store_value = false;
        bool is_set = false;
        std::vector<bool*> store_values;

    public:
        FlagArgument(std::string _name) : long_name(_name) {};
        FlagArgument(char _s_name, std::string  _l_name, std::string  _description) : short_name(_s_name), long_name(std::move(_l_name)), description(std::move(_description)) {};

        FlagArgument& Default(bool arg_name) {
            is_default_value = true;
            value = arg_name;

            return *this;
        }
        FlagArgument& StoreValue(bool& arg_name) {
            is_store_value = true;
            is_set = true;
            store_values.push_back(&arg_name);

            return *this;
        }
        bool IsDefaultValue() override {

            return is_default_value;
        }
        bool IsMultiValue() override {

            return false;
        }
        bool IsStoreValues() override {

            return is_store_value;
        }
        bool IsPositional() override {

            return false;
        }
        std::string GetLongName() override {

            return long_name;
        }
        char GetShortName() override {

            return short_name;
        }
        std::string GetDescription() override {

            return description;
        }
    };
    class ArgParser {
    public:
            std::vector<BaseArgument*> arguments;
//        std::vector<StringArgument*> string_arguments;
//        std::vector<IntegerArgument*> int_arguments;
//        std::vector<FlagArgument*> flag_arguments;

        std::string long_name_help;
        char short_name_help;
        std::string description_help;
        bool is_help = false;
        bool use_have_argument = false;

        std::string parser_name;
        ArgParser(std::string  _my_parser_name);

        bool Parse(int argc, char** argv);
        bool Parse(std::vector<std::string> split_string);

        StringArgument& AddStringArgument(const std::string& add_argument, const std::string& description = "");
        StringArgument& AddStringArgument(const char& short_name, const std::string& long_name, const std::string& description = "");

        IntegerArgument& AddIntArgument(const std::string& add_argument, const std::string& description = "");
        IntegerArgument& AddIntArgument(const char& short_name, const std::string& long_name, const std::string& description = "");

        FlagArgument& AddFlag(const std::string& add_argument, const std::string& description = "");
        FlagArgument& AddFlag(const char& short_name, const std::string& long_name, const std::string& description = "");

        std::string& GetStringValue(const std::string& arg);
        std::string& GetStringValue(const std::string &arg, size_t index);

        int GetIntValue(const std::string& arg);
        int GetIntValue(const std::string& arg, size_t index);

        bool GetFlag(const std::string& arg);

        void AddHelp(const char& short_name, const std::string& long_name, const std::string& description = "");
        bool Help();
        const std::string HelpDescription();
    };
} // namespace ArgumentParser
