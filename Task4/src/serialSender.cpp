#include "serialSender.hpp"
#include <serial/serial.h>
#include <iostream>

struct SerialSender::SerialImpl {
    std::unique_ptr<serial::Serial> port;
};

SerialSender::SerialSender() : _impl(nullptr) {}
SerialSender::~SerialSender() { close(); }

bool SerialSender::connect(const std::string &portName, unsigned int baudRate, std::string &errorMsg) {
    try {
        _impl = std::make_unique<SerialImpl>();
        _impl->port = std::make_unique<serial::Serial>(
            portName, baudRate, serial::Timeout::simpleTimeout(1000));

        if (!_impl->port->isOpen()) {
            errorMsg = "Gagal membuka port " + portName;
            _impl.reset();
            return false;
        }

        std::cout << "[INFO] Port " << portName << " terbuka di " << baudRate << " baud\n";
        return true;
    } catch (const std::exception &e) {
        errorMsg = e.what();
        _impl.reset();
        return false;
    }
}

bool SerialSender::sendBytes(const std::vector<uint8_t> &data, std::string &errorMsg) {
    if (!_impl || !_impl->port || !_impl->port->isOpen()) {
        errorMsg = "Port belum dibuka.";
        return false;
    }

    try {
        size_t sent = _impl->port->write(data);
        return sent == data.size();
    } catch (const std::exception &e) {
        errorMsg = e.what();
        return false;
    }
}

void SerialSender::close() {
    if (_impl && _impl->port && _impl->port->isOpen()) {
        _impl->port->close();
        std::cout << "[INFO] Port ditutup.\n";
    }
    _impl.reset();
}

bool SerialSender::isConnected() const {
    return _impl && _impl->port && _impl->port->isOpen();
}
