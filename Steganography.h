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
	int current_position;
	int file_size;
	int id3v2_tag_size;


public:
	void seek_zweite(int);
	void change_byte(char);

	Steganography(std::string);

	std::string get_file_name();

	int get_file_size();

	int get_id3v2_tag_size();

	int get_current_position();

	void print_all() {
		std::cout << file_name << std::endl;
		std::cout << "size: " << file_size << " Байт" << std::endl;
		std::cout << "id3v2 size: " << id3v2_tag_size << std::endl;
	}
};
