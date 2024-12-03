//
// Created by awing on 26/11/24.
//
#pragma once
#include <memory>
#include <U8g2lib.h>

namespace screen {

    class Screen {
    public:
        Screen(char d0, char d1, char d2, char d3, char d4, char d5, char d6, char d7, bool enable, char cs, char dc);
        void setFont(const uint8_t * font);
        void writeTextAndSend(const char * text);
        std::shared_ptr<U8G2_ST7920_128X64_1_6800> getU8G2Instance();
        U8G2_ST7920_128X64_1_6800 screen;
    };
}
