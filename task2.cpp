#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <zlib.h>

const size_t CHUNK_SIZE = 1024 * 1024; // 1MB
std::mutex io_mutex;

// Compress a chunk of data
void compress_chunk(const std::vector<char>& input, std::vector<char>& output, int chunk_id) {
    uLongf compressed_size = compressBound(input.size());
    output.resize(compressed_size);

    int res = compress(reinterpret_cast<Bytef*>(output.data()), &compressed_size,
                       reinterpret_cast<const Bytef*>(input.data()), input.size());

    if (res != Z_OK) {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cerr << "Compression failed for chunk " << chunk_id << "\n";
        return;
    }

    output.resize(compressed_size);
    std::lock_guard<std::mutex> lock(io_mutex);
    std::cout << "Chunk " << chunk_id << " compressed to " << compressed_size << " bytes\n";
}

// Decompress a chunk of data
void decompress_chunk(const std::vector<char>& input, std::vector<char>& output, uLongf original_size, int chunk_id) {
    output.resize(original_size);

    int res = uncompress(reinterpret_cast<Bytef*>(output.data()), &original_size,
                         reinterpret_cast<const Bytef*>(input.data()), input.size());

    if (res != Z_OK) {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cerr << "Decompression failed for chunk " << chunk_id << "\n";
        return;
    }

    std::lock_guard<std::mutex> lock(io_mutex);
    std::cout << "Chunk " << chunk_id << " decompressed\n";
}

// Compress entire file
void compress_file(const std::string& input_path, const std::string& output_path) {
    std::ifstream infile(input_path, std::ios::binary);
    std::ofstream outfile(output_path, std::ios::binary);
    std::vector<std::thread> threads;
    std::vector<std::vector<char>> compressed_chunks;
    std::vector<uLongf> original_sizes;

    int chunk_id = 0;
    while (!infile.eof()) {
        std::vector<char> buffer(CHUNK_SIZE);
        infile.read(buffer.data(), CHUNK_SIZE);
        size_t bytes_read = infile.gcount();
        if (bytes_read == 0) break;
        buffer.resize(bytes_read);

        compressed_chunks.emplace_back();
        original_sizes.push_back(bytes_read);
        threads.emplace_back(compress_chunk, buffer, std::ref(compressed_chunks.back()), chunk_id++);
    }

    for (auto& t : threads) t.join();

    // Write metadata: number of chunks and original sizes
    size_t num_chunks = compressed_chunks.size();
    outfile.write(reinterpret_cast<const char*>(&num_chunks), sizeof(size_t));
    for (uLongf size : original_sizes) {
        outfile.write(reinterpret_cast<const char*>(&size), sizeof(uLongf));
    }

    // Write compressed chunks
    for (const auto& chunk : compressed_chunks) {
        size_t chunk_size = chunk.size();
        outfile.write(reinterpret_cast<const char*>(&chunk_size), sizeof(size_t));
        outfile.write(chunk.data(), chunk_size);
    }

    std::cout << "Compression complete. Output written to " << output_path << "\n";
}

// Decompress entire file
void decompress_file(const std::string& input_path, const std::string& output_path) {
    std::ifstream infile(input_path, std::ios::binary);
    std::ofstream outfile(output_path, std::ios::binary);
    std::vector<std::thread> threads;
    std::vector<std::vector<char>> decompressed_chunks;

    // Read metadata
    size_t num_chunks;
    infile.read(reinterpret_cast<char*>(&num_chunks), sizeof(size_t));
    std::vector<uLongf> original_sizes(num_chunks);
    for (size_t i = 0; i < num_chunks; ++i) {
        infile.read(reinterpret_cast<char*>(&original_sizes[i]), sizeof(uLongf));
    }

    for (size_t i = 0; i < num_chunks; ++i) {
        size_t chunk_size;
        infile.read(reinterpret_cast<char*>(&chunk_size), sizeof(size_t));
        std::vector<char> compressed_data(chunk_size);
        infile.read(compressed_data.data(), chunk_size);

        decompressed_chunks.emplace_back();
        threads.emplace_back(decompress_chunk, compressed_data, std::ref(decompressed_chunks.back()), original_sizes[i], i);
    }

    for (auto& t : threads) t.join();

    for (const auto& chunk : decompressed_chunks) {
        outfile.write(chunk.data(), chunk.size());
    }

    std::cout << "Decompression complete. Output written to " << output_path << "\n";
}

int main() {
    std::string input = "input.txt";
    std::string compressed = "compressed.bin";
    std::string decompressed = "output.txt";

    auto start = std::chrono::high_resolution_clock::now();
    compress_file(input, compressed);
    auto mid = std::chrono::high_resolution_clock::now();
    decompress_file(compressed, decompressed);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> comp_time = mid - start;
    std::chrono::duration<double> decomp_time = end - mid;

    std::cout << "Compression time: " << comp_time.count() << " seconds\n";
    std::cout << "Decompression time: " << decomp_time.count() << " seconds\n";

    return 0;
}
