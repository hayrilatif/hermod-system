#include "yonerge.h"
#include <iostream>

Yonerge::Yonerge(std::string yol) : dosyaYolu(yol) {}

void Yonerge::dosyaBilgisi() {
    std::cout << "Seçilen yönerge dosyası: " << dosyaYolu << std::endl;
}
