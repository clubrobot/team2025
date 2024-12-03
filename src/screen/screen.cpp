//
// Created by awing on 26/11/24.
//
#include "screen.h"

#include <memory>

namespace screen {

    Screen::Screen(char d0, char d1, char d2, char d3, char d4, char d5, char d6, char d7, bool enable, char cs, char dc): screen(U8G2_ST7920_128X64_1_6800(U8G2_R0, d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc)) {
        this->screen.begin();
    }

    void Screen::setFont(const uint8_t * font) {
        this->screen.setFont(font);
    }

    void Screen::writeTextAndSend(const char * text) {
        this->screen.clearBuffer();
        this->screen.drawStr(0, 0, text);
        this->screen.sendBuffer();
    }

    std::shared_ptr<U8G2_ST7920_128X64_1_6800> Screen::getU8G2Instance() {
        return std::make_shared<U8G2_ST7920_128X64_1_6800>(this->screen);
    }




}
