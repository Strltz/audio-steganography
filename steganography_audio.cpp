// steganography_audio.cpp
//

#include <iostream>
#include <fstream>

int main()
{
    setlocale(LC_ALL, "ru");
    std::ofstream file("Departure_METRO_2033.mp3", std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    file.seekp(65536 + 5 * 4096);
    file.put(72);
    file.put(69);
    file.put(76);
    file.put(76);
    file.put(79);
    
    file.close();

    std::cout << "Ок" << std::endl;

    std::ifstream file2("Departure_METRO_2033.mp3", std::ios::in | std::ios::binary);

    file2.seekg(65536 + 5*4096);
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();
    std::cout << (char)file2.get();


    return 0;
}
