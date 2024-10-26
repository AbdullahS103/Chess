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

#endif