// steganography_audio.cpp
//

#include <iostream>
#include <fstream>

int main()
{
    setlocale(LC_ALL, "ru");
    std::ofstream file("Текст.txt", std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    char byte = 35;
    file.seekp(2);
    file.put(byte);
    file.close();

    std::cout << "Ок" << std::endl;

    return 0;
}
