// steganography_audio.cpp
//

#include "Steganography.h"

int main()
{
    // Departure_METRO_2033
    // Antonio_Vivaldi_-_Koncert_No_1_mi_mazhor_soch_8_RV_269_Vesna_I_Allegro_71556559
    // Rammstein_-_RADIO_63782865

    setlocale(LC_ALL, "ru");
    Steganography st("Rammstein_-_RADIO_63782865.mp3");
    st.seek_zweite(st.get_id3v2_tag_size());
    
    for (size_t i = 0; i < 2000; ++i) {
        st.stego_frame_change_smthng(1, 1);
    }
}
