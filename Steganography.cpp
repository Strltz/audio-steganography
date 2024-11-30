#include "Steganography.h"

// создание копии файла в той же директории; функция возвращает имя копии

// конструктор
Steganography::Steganography(std::string file_name) {
    std::ifstream input_file(file_name, std::ios::binary); // исходный файл
    // помечаем копию припиской " - copy"
    file_name.insert(file_name.length() - 4, " - copy");
    std::ofstream new_file(file_name, std::ios::binary);
    this->file_name = file_name;

    if (input_file && new_file) {
        // создаём копию исходного файла
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

    // получение размера файла 
    file_for_read_only.seekg(0, std::ios::end);
    file_size = file_for_read_only.tellg();

    // получение размера тега id3v2 (6-ой байт)
    seek_zweite(6);
    id3v2_tag_size = 10;
    for (size_t i = 0; i < 4; ++i) {
        id3v2_tag_size += file_for_read_only.get() * pow(2, (3 - i) * 7);
    }

    seek_zweite(0);
    current_position = file_for_read_only.tellg();
    k = 0;
}

// сдвиг указателя для чтения и для работы в файле на позицию pos
void Steganography::seek_zweite(int pos) {
    this->file_for_read_only.seekg(pos);
    this->file_change_bytes.seekp(pos);
    current_position = file_change_bytes.tellp();
}

// замена байта на текущей позиции
void Steganography::change_byte() {
    int this_byte = file_for_read_only.get();
    if (this_byte % 2 == 0) {
        file_change_bytes.put(this_byte + 1);
    }
    else {
        file_change_bytes.put(this_byte - 1);
    }
    ++current_position;
    seek_zweite(current_position);
}

void Steganography::stego_frame_change_smthng(int smthng_mf, int smthng_bt)
{

    file_for_read_only.seekg(current_position + 2);
    int third_byte = file_for_read_only.get();
    int size_frame = 1040 + third_byte % 4 - third_byte % 2;

    // перемещение на начало аудиоданных фрейма
    current_position += 3;
    seek_zweite(current_position);

    if (k % smthng_mf == 0) {
        for (size_t i = 0; i < size_frame; ++i) {
            if (i % smthng_bt == 0) {
                change_byte();
            }
            else {
                ++current_position;
                seek_zweite(current_position);
            }
        }
    }
    else {
        current_position += size_frame;
        seek_zweite(current_position);
    }
    ++k;
    /*
    current_position += size_frame + 1;
    seek_zweite(current_position);
    ++k;*/
}

// геттеры
std::string Steganography::get_file_name() {
    return file_name;
}

int Steganography::get_file_size() {
    return file_size;
}

int Steganography::get_id3v2_tag_size() {
    return id3v2_tag_size;
}

int Steganography::get_current_position() {
    int pos_read = file_for_read_only.tellg();
    int pos_change = file_change_bytes.tellp();
    if (pos_change == pos_read) {
        return file_for_read_only.tellg();
    }
    return -1;
}
