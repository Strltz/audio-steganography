#include "Steganography.h"

// создание копии файла в той же директории; функция возвращает имя копии

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

void Steganography::print_all() {
    std::cout << file_name << std::endl;
    std::cout << "size: " << file_size << " Байт" << std::endl;
    std::cout << "id3v2 size: " << id3v2_tag_size << std::endl;
}

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
    file_for_read_only.seekg(0, std::ios::end);
    file_size = file_for_read_only.tellg(); // получение размера файла
    seek_zweite(6);
    id3v2_tag_size = 10;
    for (size_t i = 0; i < 4; ++i) { // получение размера тега id3v2 (6-ой байт)
        id3v2_tag_size += file_for_read_only.get() * pow(2, (3 - i) * 7);
    }
    seek_zweite(0);
    current_position = file_for_read_only.tellg();
    сurrent_frame = 0;
}

// возвращает значение битрейта (в соответствии с индексом)
int Steganography::frame_get_bitrate() {
    file_for_read_only.seekg(current_position + 2);
    int bit_ind = bitrate_indexes[file_for_read_only.get() / 16];
    seek_zweite(current_position);
    return bit_ind;
}

// возвращает значение частоты дискретизации (в соответствии с индексом)
int Steganography::frame_get_frequency()
{
    file_for_read_only.seekg(current_position + 2);
    int freq_ind = frequency_indexes[(file_for_read_only.get() % 16) / 4];
    seek_zweite(current_position);
    return freq_ind;
}

// сдвиг указателя для чтения и для работы в файле на позицию pos
void Steganography::seek_zweite(int pos) {
    this->file_for_read_only.seekg(pos);
    this->file_change_bytes.seekp(pos);
    current_position = file_change_bytes.tellp();
}

// замена на конкретный байт
void Steganography::change_byte_certain(int b) {
    seek_zweite(current_position);
    file_change_bytes.put(b);
    ++current_position;
    seek_zweite(current_position);
}

// размер аудиоданных фрейма
int Steganography::frame_get_size()
{
    file_for_read_only.seekg(current_position + 2);
    int third_byte = file_for_read_only.get();
    seek_zweite(current_position);
    int size_frame = 1000 * 1152 / 8 * frame_get_bitrate() / frame_get_frequency() +
        (third_byte % 4) / 2 - 4;
    seek_zweite(current_position);
    return size_frame;
}

// внедрение информации во фрейма (метод lsb); возвращает не поместившуюся информацию
std::vector<bool> Steganography::stego_frame_lsb(std::vector <bool> secret_info) {
    // здесь в secret_info хранятся уже 0 и 1, нам нужно их лишь в правильном порядке поставить 
    int size_frame = frame_get_size();
    current_position += 4;
    seek_zweite(current_position); // перемещение на начало аудиоданных фрейма
    int end_position = current_position + size_frame;
    int size_info = secret_info.size();
    for (size_t i = 0; i < std::min(size_frame, size_info); ++i) {
        char x = file_for_read_only.get();
        change_byte_certain(x - x % 2 + (int)secret_info[0]);
        secret_info.erase(secret_info.begin());
    }
    current_position = end_position;
    seek_zweite(current_position);
    ++сurrent_frame;
    std::cout << current_position - 80000 << std::endl;
    return secret_info;
}

// внедрение информации во фрейм (замена всех байт подряд); возвращает не поместившуюся информацию
std::vector<unsigned char> Steganography::stego_frame_change_full(std::vector<unsigned char> secret_info) {
    int size_frame = frame_get_size();
    current_position += 4;
    seek_zweite(current_position); // перемещение на начало аудиоданных фрейма
    int end_position = current_position + size_frame;
    int size_info = secret_info.size();
    for (size_t i = 0; i < std::min(size_frame, size_info); ++i) {
        char x = file_for_read_only.get();
        change_byte_certain(secret_info[0]);
        secret_info.erase(secret_info.begin());
    }
    current_position = end_position;
    seek_zweite(current_position);
    ++сurrent_frame;
    return secret_info;
}

// для тетсирования и экспериментов
std::vector<unsigned char> Steganography::stego_frame_change_smthng(std::vector<unsigned char> secret_info) {
    int size_frame = frame_get_size();
    current_position += 4;
    seek_zweite(current_position); // перемещение на начало аудиоданных фрейма
    int end_position = current_position + size_frame;
    int size_info = secret_info.size();
    if (сurrent_frame % 100 == 0) {
        for (size_t i = 0; i < std::min(size_frame, size_info); ++i) {
            char x = file_for_read_only.get();
            change_byte_certain(secret_info[0]);
            secret_info.erase(secret_info.begin());
        }
    }
    current_position = end_position;
    seek_zweite(current_position);
    ++сurrent_frame;
    return secret_info;
}

// функции стеганографии
void Steganography::steganography_lsb(std::vector<unsigned char> information) {
    std::vector<bool> vec;
    for (size_t i = 0; i < information.size(); ++i) {
        int this_byte = information[i];
        for (size_t j = 0; j < 8; ++j) {
            vec.push_back((bool)((int)this_byte / (int)pow(2, (7 - j))));
            this_byte = this_byte - ((int)this_byte / (int)pow(2, (7 - j)))* (int)pow(2, (7 - j));
        }
    }
    secret_information = information;
    while (vec.size() != 0) {
        vec = stego_frame_lsb(vec);
    }
}

void Steganography::steganography_change_full(std::vector<unsigned char> information) {
    secret_information = information;
    while (information.size() != 0) {
        information = stego_frame_change_full(information);
    }
}

std::vector<unsigned char> Steganography::binarization_information(std::string information) {
    std::vector<unsigned char> vec;
    for (size_t i = 0; i < information.size(); ++i) {
        vec.push_back((char)information[i]);
    }
    return vec;
}
















// график фрейма
void Steganography::frame_graphic() {
    file_for_read_only.seekg(current_position + 2);
    int third_byte = file_for_read_only.get();
    seek_zweite(current_position);
    int size_frame = 1000 * 1152 / 8 * frame_get_bitrate() / frame_get_frequency() +
        (third_byte % 4) / 2 - 4;
    current_position += 4;
    seek_zweite(current_position);
    for (size_t i = 0; i < size_frame; ++i) {
        int x = (double)file_for_read_only.get() / 1.25;
        std::string this_point = "";
        for (size_t j = 0; j < x; ++j) {
            this_point += " ";
        }
        std::cout << this_point + "*" << std::endl;
    }
    current_position += size_frame;
    seek_zweite(current_position);
}


void Steganography::test_change() {
    file_for_read_only.seekg(current_position + 2);
    int third_byte = file_for_read_only.get();
    int size_frame = 1040 + third_byte % 4 - third_byte % 2;
    current_position += 4;
    seek_zweite(current_position);
    if (сurrent_frame % 1 == 0) {
        for (size_t i = 0; i < size_frame; ++i) {
            if (i < 1020) {
                change_byte_certain(85);
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
    ++сurrent_frame;
}
