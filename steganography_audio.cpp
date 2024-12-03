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
    Steganography st("Rammstein_-_RADIO_63782865.mp3");
    st.seek_zweite(st.get_id3v2_tag_size());


    std::string info = "Guten Tag";
    for (size_t i = 0; i < 1000; ++i) {
        info += "ag";
    }
    st.steganography_change_full(st.binarization_information(info));
    
    /*for (size_t j = 0; j < 1000; ++j) {
        v.push_back((unsigned char)(rand() % 256));
    }

    for (size_t i = 0; i < 500; ++i) {

        st.stego_frame_change_smthng(v);
    }*/

    /*for (size_t i = 0; i < 600; ++i) {
        std::cout << i << " ";
        st.seek_zweite(st.get_current_position() + st.frame_get_size() + 4);
        std::cout << std::endl;
    }*/
}
