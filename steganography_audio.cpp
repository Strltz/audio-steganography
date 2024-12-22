// steganography_audio.cpp
//

#include "Steganography.h"

int main()
{
    // Departure_METRO_2033
    // Antonio_Vivaldi_-_Koncert_No_1_mi_mazhor_soch_8_RV_269_Vesna_I_Allegro_71556559
    // Rammstein_-_RADIO_63782865
    // silence-sound-effect - как минимум один некорректный фрейм
    // tishyna-1m

    setlocale(LC_ALL, "ru");

    std::cout << "Сокрытие или извлечение? Введите s или d";

    Steganography st("Departure_METRO_2033.mp3");

    std::string info = "Guten Tag";
    
    

    st.steganography_lsb(info);
    

    
    
    std::pair<std::string, std::string> p = st.dechifer();
    std::cout << p.first << std::endl;
    std::cout << p.second;
    
}
