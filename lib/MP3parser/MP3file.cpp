#include "MP3file.h"

#include <fstream>

const size_t kHeaderSize = 10;

bool MP3file::read(const std::string& path) {
    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open())
        return false;

    char tag_header[10];
    file.read(tag_header, 10);
    parseTagHeader(tag_header);

    char frame_header[10];
    while (true) {
        file.read(frame_header, 10);
        if (frame_header[0] == 0)
            break;
        std::string id(frame_header, frame_header + 4);
        size_t size = desynchSize(&frame_header[4]);
        char flags[2];
        for (int i = 0; i < 2; flags[i] = frame_header[8 + i], ++i);
        char* data = new char[size];
        file.read(data, size);

        if (text_frames.find(id) != text_frames.end())
            frameList_.push_back(new TextFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "UFID")
            frameList_.push_back(new UFIDFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (url_frames.find(id) != url_frames.end())
            frameList_.push_back(new UrlFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "USLT")
            frameList_.push_back(new USLTFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "COMM")
            frameList_.push_back(new CommentFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "APIC")
            frameList_.push_back(new APICFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "WXXX")
            frameList_.push_back(new WXXXFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "ETCO")
            frameList_.push_back(new ETCOFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "USLT")
            frameList_.push_back(new USLTFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "SYLT")
            frameList_.push_back(new SYLTFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "RVA2")
            frameList_.push_back(new RVA2Frame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "EQU2")
            frameList_.push_back(new EQU2Frame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "PCNT")
            frameList_.push_back(new PCNTFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "POPM")
            frameList_.push_back(new POPMFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "RBUF")
            frameList_.push_back(new RBUFFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "LINK")
            frameList_.push_back(new LINKFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "POSS")
            frameList_.push_back(new POSSFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "USER")
            frameList_.push_back(new USERFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "OWNE")
            frameList_.push_back(new OWNEFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "COMR")
            frameList_.push_back(new COMRFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "ENCR")
            frameList_.push_back(new ENCRFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "GRID")
            frameList_.push_back(new GRIDFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "PRIV")
            frameList_.push_back(new PRIVFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        else if (id == "SEEK")
            frameList_.push_back(new SEEKFrame(id, size, std::make_pair(flags[0], flags[1]), data));
        delete[] data;
    }
    file.close();

    return true;
}

void MP3file::clear() {
    for (Frame* frame : frameList_)
        delete frame;
}

MP3file::~MP3file() {
    clear();
}

size_t MP3file::desynchSize(char* size_ptr) {
    size_t size = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            if ((size_ptr[3 - i] >> j) & 1)
                size += (1 << (i * 7 + j));
        }
    }

    return size;
}

bool MP3file::getFlagInfo(char flagVar, int bit) { return flagVar >> (7 - bit); }

void MP3file::parseTagHeader(char *header) {
    if (!(header[0] == 'I' && header[1] == 'D' && header[2] == '3' & header[3] == 4))
        throw std::runtime_error{ "Not an ID3v2.4 tag" };
    hasUnsynch_ = getFlagInfo(header[5], 0);
    hasExtHeader_ = getFlagInfo(header[5], 1);
    hasFooter_ = getFlagInfo(header[5], 3);
    frameSize_ = desynchSize(&header[6]) - kHeaderSize;
    if (hasFooter_) frameSize_ -= kHeaderSize;
}

const std::vector<Frame*>& MP3file::frames() {
    return frameList_;
}
