#ifndef ERRORS_H
#define ERRORS_H

#include "CommonHeaders.h"

class UnsupportedFunctionException : public std::runtime_error {
public:
  UnsupportedFunctionException(const std::string &message) : std::runtime_error(message) {}
};

class InvalidMoveException : public std::runtime_error {
public:
  InvalidMoveException(const std::string &message) : std::runtime_error(message) {}
};

class InvalidIndexException : public std::runtime_error {
public:
  InvalidIndexException(const std::string &message): std::runtime_error(message) {}
};

class InvalidBoardException : public std::runtime_error {
public:
  InvalidBoardException(const std::string &message): std::runtime_error(message) {}
};

class InvalidInputException : public std::runtime_error {
public:
  InvalidInputException(const std::string &message): std::runtime_error(message) {}
};

#endif