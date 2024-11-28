#include "Steganography.h"

// �������� ����� ����� � ��� �� ����������; ������� ���������� ��� �����

// �����������
Steganography::Steganography(std::string file_name) {
    std::ifstream input_file(file_name, std::ios::binary); // �������� ����
    // �������� ����� ��������� " - copy"
    file_name.insert(file_name.length() - 4, " - copy");
    std::ofstream new_file(file_name, std::ios::binary);
    this->file_name = file_name;

    if (input_file && new_file) {
        // ������ ����� ��������� �����
        new_file << input_file.rdbuf();
        std::cout << "A copy of the file has been created" << std::endl;
    }
    else {
        std::cerr << "File opening error" << std::endl;
    }

    input_file.close();
    new_file.close();

    file_change_bytes.open(file_name, std::ios::in | std::ios::binary);
    file_for_read_only.open(file_name, std::ios::in | std::ios::binary);

    // ����� �� 6-�� ���� - id3v2.hdr.size
    seek_zweite(6);
    id3v2_tag_size = 10;
    for (size_t i = 0; i < 4; ++i) {
        id3v2_tag_size += file_for_read_only.get() * pow(2, (3 - i) * 7);
    }
}

// ����� ��������� ��� ������ � ��� ������ � ����� �� ������� pos
void Steganography::seek_zweite(int pos) {
    this->file_for_read_only.seekg(pos);
    this->file_change_bytes.seekp(pos);
}

// ������ ����� �� ������� pos �� byte
void Steganography::change_byte(std::string file_name, int pos, char byte)
{
    std::fstream bin(file_name, std::ios::in | std::ios::binary);
    bin.seekp(pos, std::ios::beg);
    bin.put(byte);
    bin.close();
}
