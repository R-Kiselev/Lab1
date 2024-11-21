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
    void validate_balance(const int balance) const{
        if (balance < 0){
            throw ValidationException("Balance must be a positive integer.");
        }
    }
    void validate_id(const int id) const{
        if (id < 0){
            throw ValidationException("ID must be a positive integer.");
        }
    }
    void validate_username(const std::string& username){
        if(username == "MishaKorduban"){
            throw ValidationException("Вам нельзя.");
        }

        re = "^[a-zA-Z0-9._-]{3,20}$";
        if (!std::regex_match(username, re)){
            throw ValidationException("Username must contain only alphanumeric characters, hyphens and underscores.");
        }
    }
    void validate_password(const std::string& password){
        re = "^.{6,20}$";
        if(!std::regex_match(password, re)){
            throw ValidationException("The password must be between 6 and 20 characters long");
        }
    }
};
