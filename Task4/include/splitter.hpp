#pragma once
#include <string>
#include <vector>
#include <cstdint>

class FileSplitter {
public:
    // Constructor: path file dan ukuran chunk (default 220 byte)
    FileSplitter(const std::string &path, std::size_t chunkSize = 220);

    // Load isi file ke buffer internal
    bool loadFile();

    // Validasi isi JSON (cek field wajib & jumlah kata deskripsi)
    bool validateJson(std::string &errorMsg) const;

    // Bagi file jadi beberapa chunk kecil
    bool splitFile();

    // Dapatkan hasil split
    const std::vector<std::vector<uint8_t>>& getChunks() const;

    // Hitung checksum (CRC32) dari seluruh file
    uint32_t computeCRC32() const;

    // Dapatkan nama file (tanpa path)
    std::string getFilename() const;

private:
    std::string _path;
    std::string _filename;
    std::size_t _chunkSize;
    std::vector<uint8_t> _fileData;
    std::vector<std::vector<uint8_t>> _chunks;

    static std::string extractFilename(const std::string &path);
    bool jsonHasRequiredFields(std::string &errorMsg) const;
};
