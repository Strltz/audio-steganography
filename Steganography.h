#pragma once

#include <cmath>
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
	int file_size;
	int id3v2_tag_size;
	int сurrent_frame; // счётчик mp3-фреймов
	std::vector<int> bitrate_indexes =
		{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };
	std::vector<int> frequency_indexes = { 44100, 48000, 32000, 0 };

	std::vector<unsigned char> secret_information;

	int frame_get_bitrate();
	int frame_get_frequency();
	void change_byte_certain(int);

public:
	int current_position;

	int frame_get_size();
	std::vector<bool> stego_frame_lsb(std::vector<bool>);
	std::vector<unsigned char> stego_frame_change_full(std::vector<unsigned char>);
	std::vector<unsigned char> stego_frame_change_smthng(std::vector<unsigned char>);
	void frame_graphic();
	void seek_zweite(int);
	void test_change();
	void steganography_lsb(std::vector<unsigned char>);
	void steganography_change_full(std::vector<unsigned char>);
	std::vector<unsigned char> binarization_information(std::string);
	

	//
	Steganography(std::string);

	std::string get_file_name();
	int get_file_size();
	int get_id3v2_tag_size();
	int get_current_position();
	void print_all();
};
