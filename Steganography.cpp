#include "Steganography.h"

// создание копии файла в той же директории; функция возвращает имя копии

// конструктор
Steganography::Steganography(std::string file_name) {
    // исходный файл
    std::ifstream input_file(file_name, std::ios::binary); 
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
    // получение размера файла
    file_size = file_for_read_only.tellg();
    seek_zweite(6);
    id3v2_tag_size = 10;
    // получение размера тега id3v2 (6-ой байт)
    for (size_t i = 0; i < 4; ++i) {
        id3v2_tag_size += file_for_read_only.get() * pow(2, (3 - i) * 7);
    }
    seek_zweite(0);
    current_position = file_for_read_only.tellg();
    сurrent_frame = 0;
}

// деструктор
Steganography::~Steganography() {
    file_change_bytes.close();
    file_for_read_only.close();
}

//
// стеганография методом LSB
void Steganography::steganography_lsb(std::string info) {
    std::vector<unsigned char> information = binarization_information(info);
    stego_integration_size_info(information.size());
    std::vector<bool> vec;
    for (size_t i = 0; i < information.size(); ++i) {
        int this_byte = information[i];
        for (size_t j = 0; j < 8; ++j) {
            vec.push_back((bool)((int)this_byte / (int)pow(2, (7 - j))));
            this_byte = this_byte -
                ((int)this_byte / (int)pow(2, (7 - j))) * (int)pow(2, (7 - j));
        }
    }
    secret_information = information;
    while (vec.size() != 0) {
        vec = stego_frame_lsb(vec);
    }
}

// стеганография методом полной замены байт
void Steganography::steganography_change_full(std::string info) {
    std::vector<unsigned char> information = binarization_information(info);
    stego_integration_size_info(information.size());
    secret_information = information;
    while (information.size() != 0) {
        information = stego_frame_change_full(information);
    }
}

// стеганография в заголовке
void Steganography::steganography_header(std::string info) {
    std::vector<unsigned char> information = binarization_information(info);
    seek_zweite(id3v2_get_size_base_info());
    std::cout << 1 << std::endl;
    int i = 0;
    while (i < information.size()) {
        change_byte_certain(information[i]);
        ++i;
    }
}

//
// дешифровка полная
std::pair<std::string, std::string> Steganography::dechifer() {
    // lsb
    int start_dechifer = id3v2_tag_size;
    seek_zweite(id3v2_tag_size);
    start_dechifer += frame_get_size();
    seek_zweite(id3v2_tag_size + 4);
    int size_info = 0;
    size_info += (int)file_for_read_only.get() * pow(2, 8);
    seek_zweite(id3v2_tag_size + 5);
    size_info += (int)file_for_read_only.get();
    std::vector<bool> full_information_lsb;
    seek_zweite(start_dechifer + 4);
    std::string info_lsb;
    while (size_info * 8 != full_information_lsb.size()) {
        std::vector<bool> vec =
            dechifer_frame_lsb(size_info - full_information_lsb.size());
        full_information_lsb.insert(full_information_lsb.end(), vec.begin(),
            vec.end());
    }
    for (size_t i = 0; i < full_information_lsb.size() / 8; ++i) {
        char symb = 0;

        for (size_t j = 0; j < 8; ++j) {
            symb += full_information_lsb[i * 8 + j] * pow(2, 7 - j);
        }
        info_lsb += symb;
    }
    // fc
    std::string info_full_change;
    std::vector<unsigned char> full_information_fc;
    seek_zweite(start_dechifer + 4);
    while (size_info != full_information_fc.size()) {
        std::vector<unsigned char> vec =
            dechifer_frame_change_full(size_info - full_information_fc.size());
        full_information_fc.insert(full_information_fc.end(), vec.begin(),
            vec.end());
    }
    for (size_t i = 0; i < full_information_fc.size(); ++i) {

        info_full_change += full_information_fc[i];
    }
    return std::pair<std::string, std::string>(info_lsb, info_full_change);
}

//
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

//
// получение размера tf из id3v2 тега
int Steganography::id3v2_get_size_tf() {
    int start_pos = current_position;
    seek_zweite(current_position + 4);
    int size_tf = 10;
    for (size_t i = 0; i < 4; ++i) {
        size_tf += file_for_read_only.get() * pow(2, 8 * (3 - i));
        seek_zweite(current_position + 1);
    }
    seek_zweite(start_pos + size_tf);
    std::cout << size_tf << std::endl;
    return size_tf;
}

// получение размера основной информации id3v2
int Steganography::id3v2_get_size_base_info() {
    int s = 10;
    seek_zweite(10);
    while (file_for_read_only.get() != 0) {
        seek_zweite(current_position);
        s += id3v2_get_size_tf();
    }
    seek_zweite(current_position);
    return s;
}

//
// возвращает значение битрейта (в соответствии с индексом)
int Steganography::frame_get_bitrate() {
    file_for_read_only.seekg(current_position + 2);
    int bit_ind = bitrate_indexes[file_for_read_only.get() / 16];
    seek_zweite(current_position);
    return bit_ind;
}

// возвращает значение частоты дискретизации (в соответствии с индексом)
int Steganography::frame_get_frequency() {
    file_for_read_only.seekg(current_position + 2);
    int freq_ind = frequency_indexes[(file_for_read_only.get() % 16) / 4];
    seek_zweite(current_position);
    return freq_ind;
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

//
// замена на конкретный байт
void Steganography::change_byte_certain(int b) {
    seek_zweite(current_position);
    file_change_bytes.put(b);
    ++current_position;
    seek_zweite(current_position);
}

// сдвиг указателя для чтения и для работы в файле на позицию pos
void Steganography::seek_zweite(int pos) {
    this->file_for_read_only.seekg(pos);
    this->file_change_bytes.seekp(pos);
    current_position = file_change_bytes.tellp();
}

//
// представление информации в удобном для преобраование в бинарник виде
std::vector<unsigned char>
Steganography::binarization_information(std::string information) {
    std::vector<unsigned char> vec;
    for (size_t i = 0; i < information.size(); ++i) {
        vec.push_back((char)information[i]);
    }
    return vec;
}

//
// внедрение размера информации в начало первого фрейма
void Steganography::stego_integration_size_info(int a) {
    seek_zweite(get_id3v2_tag_size());
    int end_pos = get_current_position() + frame_get_size();
    seek_zweite(current_position + 4);
    int first_part = a / 256;
    int second_part = a % 256;
    change_byte_certain(first_part);
    change_byte_certain(second_part);
    seek_zweite(end_pos + 4);
}

// внедрение информации во фрейм (lsb); возвращает не поместившуюся информацию
std::vector<bool> Steganography::stego_frame_lsb
(std::vector<bool> secret_info) {
    // в secret_info хранятся уже 0 и 1, нужно их лишь правильно поставить 
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
    return secret_info;
}

// внедрение информации во фрейм (fc); возвращает не поместившуюся информацию
std::vector<unsigned char>Steganography::stego_frame_change_full
(std::vector<unsigned char> secret_info) {
    int size_frame = frame_get_size();
    current_position += 4;
    // перемещение на начало аудиоданных фрейма
    seek_zweite(current_position);
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

//
// дешифровка фрейма lsb
std::vector<bool> Steganography::dechifer_frame_lsb(int size_info) {
    int size_frame = frame_get_size();
    std::vector<bool> info_frame;
    current_position += 4;
    // перемещение на начало аудиоданных фрейма
    seek_zweite(current_position);
    int end_position = current_position + size_frame;
    for (size_t i = 0; i < std::min(size_frame, size_info * 8); ++i) {
        info_frame.push_back((bool)((char)file_for_read_only.get() % 2));
        seek_zweite(current_position + 1);
    }
    current_position = end_position;
    seek_zweite(current_position);
    ++сurrent_frame;
    return info_frame;
}

// дешифровка фрейма методом полной замены
std::vector<unsigned char> Steganography::dechifer_frame_change_full
(int size_info)
{
    int size_frame = frame_get_size();
    std::vector<unsigned char> info_frame;
    current_position += 4;
    // перемещение на начало аудиоданных фрейма
    seek_zweite(current_position);
    int end_position = current_position + size_frame;
    for (size_t i = 0; i < std::min(size_frame, size_info); ++i) {
        info_frame.push_back((int)file_for_read_only.get());
        seek_zweite(current_position + 1);
    }
    current_position = end_position;
    seek_zweite(current_position);
    ++сurrent_frame;
    return info_frame;
}
