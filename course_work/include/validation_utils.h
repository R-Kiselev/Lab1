#pragma once
#include <regex>
#include "Errors/CustomExceptions.h"

class ValidationService{
private:
    std::regex re;
public:
    void validate_name(const std::string& name){
        re = "^[A-Za-z]+(?:[ -][A-Za-z]+)*$";
        if (!std::regex_match(name, re)){
            throw ValidationException("Name must contain only alphabetic characters, spaces and hyphens between words.");
        }
    }
    void validate_balance(const int balance){
        if (balance < 0){
            throw ValidationException("Balance must be a positive integer.");
        }
    }
    void validate_id(const int id){
        if (id < 0){
            throw ValidationException("ID must be a positive integer.");
        }
    }
};
