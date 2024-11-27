// steganography_audio.cpp
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    



    /*std::ofstream file("Departure_METRO_2033 — копия.mp3", std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    std::ifstream file2("Departure_METRO_2033_right — копия.mp3", std::ios::in | std::ios::binary);

    file.seekp(65536 + 5 * 4096 + 16);
    for (size_t i = 0; i < 65536; ++i) {
        file2.seekg(65536 + 5 * 4096 + 16 + i);
        file.put(file2.get() - file2.get() % 2);
    }


    std::cout << "Ок" << std::endl;

    std::ifstream file2("Departure_METRO_2033.mp3", std::ios::in | std::ios::binary);

    file2.seekg(65536 + 5 * 4096);
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();

    file.close();
    file2.close();*/
    return 0;
}
