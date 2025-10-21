#pragma once
#include <string>
#include <vector>
#include <memory>

class SerialSender {
public:
    SerialSender();
    ~SerialSender();

    bool connect(const std::string &portName, unsigned int baudRate, std::string &errorMsg);
    bool sendBytes(const std::vector<uint8_t> &data, std::string &errorMsg);
    void close();
    bool isConnected() const;

private:
    struct SerialImpl;
    std::unique_ptr<SerialImpl> _impl;
};
