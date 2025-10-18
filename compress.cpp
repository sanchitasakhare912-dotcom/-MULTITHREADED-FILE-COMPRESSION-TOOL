#include <iostream>
#include <fstream>
#include <string>

std::string compress(const std::string& data) {
    std::string result;
    int count = 1;
    for (size_t i = 1; i <= data.size(); ++i) {
        if (i < data.size() && data[i] == data[i - 1]) {
            ++count;
        } else {
            result += std::to_string(count) + data[i - 1];
            count = 1;
        }
    }
    return result;
}

int main() {
    std::ifstream input("input.txt");
    if (!input) {
        std::cerr << "Error: Could not open input.txt\n";
        return 1;
    }

    std::string data((std::istreambuf_iterator<char>(input)),
                      std::istreambuf_iterator<char>());
    input.close();

    std::string compressed = compress(data);

    std::ofstream output("compressed.txt");
    output << compressed;
    output.close();

    std::cout << "Compression complete. Output saved to compressed.txt\n";
    return 0;
}
