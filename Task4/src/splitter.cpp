#include "splitter.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

// ekstrak nama file dari path
std::string FileSplitter::extractFilename(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

FileSplitter::FileSplitter(const std::string &path, std::size_t chunkSize)
    : _path(path), _chunkSize(chunkSize), _filename(extractFilename(path)) {}

// Load file JSON ke _fileData/biner
bool FileSplitter::loadFile() {
    std::ifstream file(_path, std::ios::binary);
    if (!file) return false;

    _fileData.assign((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();
    return !_fileData.empty();
}

// Validasi isi JSON
bool FileSplitter::validateJson(std::string &errorMsg) const {
    std::string text(_fileData.begin(), _fileData.end());

    // field dengan regex sederhana 
    std::regex reNama("\"nama\"\\s*:\\s*\"([^\"]+)\"");
    std::regex reJurusan("\"jurusan\"\\s*:\\s*\"([^\"]+)\"");
    std::regex reUmur("\"umur\"\\s*:\\s*(\\d+)");
    std::regex reDeskripsi("\"deskripsi\"\\s*:\\s*\"([^\"]+)\"");

    std::smatch m;
    
    if (!std::regex_search(text, m, reNama)) {
        errorMsg = "Field 'nama' tidak ditemukan.";
        return false;
    }
    if (!std::regex_search(text, m, reJurusan)) {
        errorMsg = "Field 'jurusan' tidak ditemukan.";
        return false;
    }
    if (!std::regex_search(text, m, reUmur)) {
        errorMsg = "Field 'umur' tidak ditemukan.";
        return false;
    }
    if (!std::regex_search(text, m, reDeskripsi)) {
        errorMsg = "Field 'deskripsi' tidak ditemukan.";
        return false;
    }

    std::string deskripsi = m[1];
    // hitung jumlah kata
    std::istringstream iss(deskripsi);
    std::string word;
    int count = 0;
    while (iss >> word) count++;
    if (count < 25) {
        errorMsg = "Deskripsi kurang dari 25 kata.";
        return false;
    }
    return true;
}

// Bagi file ke beberapa chunk
bool FileSplitter::splitFile() {
    if (_fileData.empty()) return false;

    _chunks.clear();
    size_t totalSize = _fileData.size();
    for (size_t i = 0; i < totalSize; i += _chunkSize) {
        size_t len = std::min(_chunkSize, totalSize - i);
        _chunks.emplace_back(_fileData.begin() + i, _fileData.begin() + i + len);
    }
    return !_chunks.empty();
}

// Getter
const std::vector<std::vector<uint8_t>>& FileSplitter::getChunks() const {
    return _chunks;
}

std::string FileSplitter::getFilename() const {
    return _filename;
}

// CRC32 
uint32_t FileSplitter::computeCRC32() const {
    uint32_t crc = 0xFFFFFFFF;
    for (auto b : _fileData) {
        crc ^= b;
        for (int i = 0; i < 8; i++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
}
