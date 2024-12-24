// steganography_audio.cpp
//
#include "Steganography.h"

int main() {
    // Departure_METRO_2033.mp3

    setlocale(LC_ALL, "ru");

    std::string work;
    std::string file;
    std::cout << "Сокрытие или извлечение? Введите s или d" << std::endl;
    std::cin >> work;
    std::cout << "Введите название файла" << std::endl;
    std::cin >> file;
    Steganography st(file);
    if (work == "s") {
        std::string method;
        std::cout << "Метод lsb или полной замены? Введите lsb или fc" << std::endl;
        std::cin >> method;
        std::cout << "Введите информацию" << std::endl;
        std::string info;
        std::cin >> info;
        std::string info2;
        getline(std::cin, info2);
        info += info2;
        if (method == "lsb") {
            st.steganography_lsb(info);
        }
        else {
            st.steganography_change_full(info);
        }
    }
    else if (work == "d") {
        std::pair<std::string, std::string> p = st.dechifer();
        std::cout << "lsb: " << p.first << std::endl;
        std::cout << " fc: " << p.second << std::endl;
    }
    else {
        st.steganography_header("guter Text");
    }
}
