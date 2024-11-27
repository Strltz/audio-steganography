#pragma once

#include <fstream>
#include <iostream>
#include <string>

void copy_file_function(std::string file_name) {
    std::ifstream input_file(file_name, std::ios::binary);
    file_name.insert(file_name.length() - 4, " - copy"); // помечаем копию припиской " - copy"
    std::ofstream new_file(file_name, std::ios::binary);

    if (input_file && new_file) {
        new_file << input_file.rdbuf();
        std::cout << "A copy of the file has been created" << std::endl;
    }
    else {
        std::cerr << "File opening error" << std::endl;
    }

    input_file.close();
    new_file.close();
}