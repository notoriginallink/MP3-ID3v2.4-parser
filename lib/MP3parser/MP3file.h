#pragma once

#include "Frame.h"

class MP3file {
public:
    bool read(const std::string& path);
    void clear();
    const std::vector<Frame*>& frames();
    ~MP3file();

private:
    size_t frameSize_;
    bool hasUnsynch_;
    bool hasExtHeader_;
    bool hasFooter_;
    std::vector<Frame*> frameList_;

    size_t desynchSize(char* size_ptr);
    bool getFlagInfo(char flagVar, int n);
    void parseTagHeader(char* header);
};


