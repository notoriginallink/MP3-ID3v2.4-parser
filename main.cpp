#include "lib/MP3parser/MP3file.h"

int main() {
    MP3file file;
    file.read("../music/Black Math - Suck City.mp3");
    for (auto frame : file.frames())
        frame->print();
    file.clear();

    return 0;
}