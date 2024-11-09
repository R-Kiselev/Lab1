#pragma once

#include <stdexcept>
#include <string>

class CustomException : public std::runtime_error {
public:
    explicit CustomException(const std::string& message)
            : std::runtime_error(message) {}
};

class DatabaseException : public CustomException {
public:
    explicit DatabaseException(const std::string& message)
            : CustomException("Database Error: " + message) {}
};

class NotFoundException : public CustomException {
public:
    explicit NotFoundException(const std::string& message)
            : CustomException("Not Found: " + message) {}
};

class ValidationException : public CustomException {
public:
    explicit ValidationException(const std::string& message)
            : CustomException("Validation Error: " + message) {}
};

class TransactionException : public CustomException {
public:
    explicit TransactionException(const std::string& message)
            : CustomException("Transaction Error: " + message) {}
};