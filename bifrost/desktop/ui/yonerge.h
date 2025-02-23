#ifndef YONERGE_H
#define YONERGE_H

#include <string>

class Yonerge {
public:
    Yonerge(std::string yol);
    void dosyaBilgisi();

private:
    std::string dosyaYolu;
};

#endif 
