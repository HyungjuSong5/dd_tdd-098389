#pragma once
#include <stdexcept>

class ReadFailException : public std::runtime_error {
 public:
  ReadFailException() : std::runtime_error("Read consistency check failed") {}
};

class WriteFailException : public std::runtime_error {
 public:
  WriteFailException() : std::runtime_error("Write to non-empty memory") {}
};
