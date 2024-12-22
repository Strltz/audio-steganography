#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// класс Steganography; описывает mp3-файл, а также функции для сокрытия и извлечения скрытой в файле информации
class Steganography {
public:
	Steganography(std::string);
	~Steganography();

	void steganography_lsb(std::string);
	void steganography_change_full(std::string);
	void steganography_header(std::string);
	
	std::pair<std::string, std::string> dechifer();
	
	std::string get_file_name();
	int get_file_size();
	int get_id3v2_tag_size();
	int get_current_position();

private:
	std::string file_name;
	std::ofstream file_change_bytes; // для работы с файлом
	std::ifstream file_for_read_only; // для чтения файла
	int file_size;
	int id3v2_tag_size;
	int сurrent_frame; // счётчик mp3-фреймов
	int current_position;

	std::vector<int> bitrate_indexes =
	{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };
	std::vector<int> frequency_indexes = { 44100, 48000, 32000, 0 };

	std::vector<unsigned char> secret_information;

	int id3v2_get_size_tf();
	int id3v2_get_size_base_info();

	int frame_get_bitrate();
	int frame_get_frequency();
	int frame_get_size();

	void change_byte_certain(int);
	void seek_zweite(int);

	std::vector<unsigned char> binarization_information(std::string);

	std::vector<bool> dechifer_frame_lsb(int);
	std::vector<unsigned char> dechifer_frame_change_full(int);

	void stego_integration_size_info(int);
	std::vector<bool> stego_frame_lsb(std::vector<bool>);
	std::vector<unsigned char> stego_frame_change_full(std::vector<unsigned char>);
};
