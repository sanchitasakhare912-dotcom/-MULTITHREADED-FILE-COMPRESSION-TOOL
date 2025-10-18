#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

std::string decompress(const std::string& data) {
    std::string result;
    std::string countStr;

    for (char ch : data) {
        if (std::isdigit(ch)) {
            countStr += ch;
        } else {
            int count = std::stoi(countStr);
            result.append(count, ch);
            countStr.clear();
        }
    }

    return result;
}

int main() {
    std::ifstream input("compressed.txt");
    if (!input) {
        std::cerr << "Error: Could not open compressed.txt\n";
        return 1;
    }

    std::string data((std::istreambuf_iterator<char>(input)),
                      std::istreambuf_iterator<char>());
    input.close();

    std::string decompressed = decompress(data);

    std::ofstream output("decompressed.txt");
    output << decompressed;
    output.close();

    std::cout << "Decompression complete. Output saved to decompressed.txt\n";
    return 0;
}
