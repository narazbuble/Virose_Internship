#include "splitter.hpp"
#include "packet.hpp"
#include "serialSender.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector> 

const uint8_t SOP_BYTE = 0xAA; // Start Of Packet
const uint8_t EOP_BYTE = 0x55; // End Of Packet

int main(){
    std::string filePath = "../../data/myprofile.json"; 
    FileSplitter splitter(filePath, 220); // Chunk size 220 bytes

    if (!splitter.loadFile()) {
        std::cerr << "Gagal membaca file JSON.\n";
        return 1;
    }

    std::string err;
    if (!splitter.validateJson(err)) {
        std::cerr << "Validasi gagal: " << err << "\n";
        return 1;
    }

    if (!splitter.splitFile()) {
        std::cerr << "Gagal membagi file.\n";
        return 1;
    }

    uint32_t crc = splitter.computeCRC32();
    auto &chunks = splitter.getChunks();
    uint16_t total = static_cast<uint16_t>(chunks.size());

    SerialSender sender;
    if (!sender.connect("COM6", 115200, err)) { 
        std::cerr << "Tidak bisa membuka serial port: " << err << "\n";
        return 1;
    }

    for (uint16_t i = 0; i < total; ++i) {
        
        // Header C++ + Payload
        Packet p(i, total, crc, splitter.getFilename(), chunks[i]);
        auto bytes = p.serialize(); 
        
        // bingkai SOP dan EOP
        std::vector<uint8_t> finalPacket;
        // memo +2
        finalPacket.reserve(bytes.size() + 2); 
        
        // +SOP
        finalPacket.push_back(SOP_BYTE); 
        
        // paket
        finalPacket.insert(finalPacket.end(), bytes.begin(), bytes.end());
        
        // +EOP
        finalPacket.push_back(EOP_BYTE); 
        
        // kirim paket
        if (!sender.sendBytes(finalPacket, err)) {
            std::cerr << "Gagal kirim paket " << i << ": " << err << "\n";
            break;
        }
        std::cout << "Paket " << i + 1 << "/" << total << " terkirim. Total size: " << finalPacket.size() << " bytes.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    sender.close();
    std::cout << "Selesai mengirim semua paket.\n";
    return 0;
}
