#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// класс Steganography; описывает mp3-файл, а также функции для сокрытия и извлечения скрытой в файле информации
class Steganography {
private:
	std::string file_name;
    std::ofstream file_change_bytes; // для работы с файлом
    std::ifstream file_for_read_only; // для чтения файла
	int id3v2_tag_size;

public:
	Steganography(std::string);

    void seek_zweite(int);

	void change_byte(std::string, int, char);

	void print_all() {
		std::cout << file_name << std::endl;
		std::cout << id3v2_tag_size << std::endl;
	}
};

