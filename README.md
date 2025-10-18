# -MULTITHREADED-FILE-COMPRESSION-TOOL

COMPANY: CODETECH IT SOLUTIONS

NAME: SANCHITA SACHIN SAKHARE

INTERN ID: CT04DY2689

DOMAIN: C++ PROGRAMMING

DURATION: 4 WEEKS

MENTOR: NEELA SANTHOSH KUMAR

#description
This project is a multithreaded file compression and decompression tool written in C++, designed to demonstrate performance optimization through parallel processing. At its core, the tool leverages the zlib library to perform lossless compression, meaning it reduces the size of files without sacrificing any data integrity. The application is built around a chunk-based architecture, where large files are divided into manageable segments—typically 1MB each—and each segment is processed independently using C++11 threads. This design allows the program to take full advantage of modern multi-core processors, significantly improving speed and efficiency compared to single-threaded implementations.

The compression process begins by reading the input file in binary mode and slicing it into chunks. Each chunk is then passed to a separate thread, which invokes zlib’s compress() function to encode the data. Before compression, the tool uses compressBound() to estimate the maximum size of the compressed output, ensuring that memory allocation is sufficient. Once compressed, each chunk is stored in a vector, and its original size is recorded for future decompression. After all threads complete their tasks, the tool writes metadata—including the number of chunks and their original sizes—followed by the compressed data itself into a single output file (compressed.bin). This metadata is crucial for accurate decompression, as it allows the program to reconstruct the original file structure.

Decompression follows a similar multithreaded approach. The tool reads the metadata from the compressed file to determine how many chunks were stored and their respective original sizes. It then reads each compressed chunk, spawns a thread to decompress it using zlib’s uncompress() function, and stores the results in memory. Once all threads finish, the decompressed chunks are written sequentially to a new output file (output.txt). This ensures that the original file is restored exactly as it was, byte-for-byte.

One of the most compelling aspects of this tool is its ability to demonstrate real-world performance gains. By timing both the compression and decompression phases using std::chrono, the program provides clear metrics that quantify the benefits of multithreading. For example, compressing a large text file might take several seconds in a single-threaded model, but only a fraction of that time when parallelized. This makes the tool especially useful for applications involving large datasets, such as log files, backups, or scientific data.

Beyond performance, the project showcases several advanced programming concepts. It involves thread synchronization using mutexes to prevent race conditions during console output. It handles binary file I/O with precision, ensuring that data is read and written correctly across platforms. It also demonstrates robust error handling, logging failures in compression or decompression without crashing the program. These features make the tool not only efficient but also reliable and maintainable.

From an educational standpoint, this project is an excellent introduction to systems programming, concurrency, and data compression. It bridges the gap between theoretical knowledge and practical implementation, giving learners hands-on experience with threading, memory management, and algorithmic optimization. It also opens the door to further enhancements, such as implementing thread pools, adding command-line arguments for flexibility, or supporting alternative compression formats like LZMA or Brotli.


In summary, this multithreaded compression tool is a powerful and elegant solution for efficient file handling. It combines the speed of parallel processing with the reliability of lossless compression, all wrapped in a clean and modular C++ design. Whether used for personal file management, educational purposes, or as a foundation for more complex systems, it stands as a testament to the power of modern C++ and thoughtful software engineering.

<img width="644" height="323" alt="Screenshot 2025-10-18 215337" src="https://github.com/user-attachments/assets/7a8ecdc9-bc73-4367-abcd-3f28760dcec5" />

