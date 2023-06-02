#include "Frame.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

Frame::Frame(const std::string& id, size_t size, const std::pair<char, char>& flags) : id_(id), size_(size), flags_(flags) {}
void Frame::print() { std::cout << "FRAME: " << id_ << '\n'; }

std::unordered_map<std::string, std::string> text_frame_description = {
        {"TIT1", "Content group description"},
        {"TIT2", "Title/Songname/Content description"},
        {"TIT3", "Subtitle/Description refinement"},
        {"TALB", "Album/Movie/Show title"},
        {"TOAL", "Original album/movie/show title"},
        {"TRCK", "Track number/Position in set"},
        {"TPOS", "Part of a set"},
        {"TSST", "Set subtitle'"},
        {"TSRC", "International Standard Recording Code"},
        {"TPE1", "Lead artist/Lead performer/Soloist/Performing group"},
        {"TPE2", "Band/Orchestra/Accompaniment"},
        {"TPE3", "Conductor"},
        {"TPE4", "Interpreted, remixed, or otherwise modified by"},
        {"TOPE", "Original artist/performer"},
        {"TEXT", "Lyricist/Text writer"},
        {"TOLY", "Original lyricist/text writer"},
        {"TCOM", "Composer"},
        {"TMCL", "Musician credits list"},
        {"TIPL", "Involved people list"},
        {"TENC", "Encoded by"},
        {"TBPM", "BPM"},
        {"TLEN", "Length"},
        {"TKEY", "Initial key"},
        {"TLAN", "Language"},
        {"TCON", "Content type"},
        {"TMOO", "Mood"},
        {"TCOP", "Copyright message"},
        {"TPRO", "Produced notice"},
        {"TPUB", "Publisher"},
        {"TOWN", "File owner/licensee"},
        {"TRSN", "Internet radio station name"},
        {"TRSO", "Internet radio station owner"},
        {"TOFN", "Original filename"},
        {"TDLY", "Playlist delay"},
        {"TDEN", "Encoding time"},
        {"TDOR", "Original release time"},
        {"TDRC", "Recording time"},
        {"TDRL", "Release time"},
        {"TDTG", "Tagging time"},
        {"TSSE", "Software/Hardware and settings used for encoding"},
        {"TSOA", "Album sort order"},
        {"TSOP", "Performer sort order"},
        {"TSOT", "Title sort order"},
};


TextFrame::TextFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    encoding_ = data[0];
    if (id_ == "TXXX") {
        int i = 0;
        for (;data[i] != 0; ++i);
        frame_description_.assign(data + 1, data + i);
        data_.assign(data + i + 1, data + size);
    } else {
        frame_description_ = text_frame_description[id_];
        data_.assign(data + 1, data + size);
    }
}
void TextFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", data: " << data_ << std::endl; }


UFIDFrame::UFIDFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    int i = 0;
    for (; i < size && data[i] != 0; ++i);
    owner_identifier_.assign(data, data + i);
    identifier_.assign(data + i + 1, data + size);
}
void UFIDFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", Owner_id: " << owner_identifier_ << " Id: " << identifier_ << std::endl; }

std::unordered_map<std::string, std::string> url_frame_desription_ = {

};

UrlFrame::UrlFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data)
    : Frame(id, size, flags),
      url_(data, data + size) {
    frame_description_ = url_frame_desription_[id_];
}

void UrlFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", data: " << url_ << std::endl; }

WXXXFrame::WXXXFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "User defined url frame";
    encoding_ = data[0];
    int i = 0;
    for (; data[i] != 0; ++i);
    desription_.assign(data + 1, data + i);
    url_.assign(data + i + 1, data + size);
}

void WXXXFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", description:" << desription_ << ", url: " << url_ << std::endl; }


USLTFrame::USLTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    encoding_ = data[0];
    language_.assign(data + 1, data + 4);
    int i = 4;
    for (; i < size && data[i] != 0; ++i);
    content_descriptor_.assign(data + 4, data + i);
    lyrics_.assign(data + i + 1, data + size);
}
void USLTFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", language: " << language_  << ", lyrics: " << lyrics_ << std::endl; }

CommentFrame::CommentFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Comment";
    encoding_ = data[0];
    language_.assign(data + 1, data + 4);
    int i = 4;
    for (; i < size && data[i] != 0; ++i);
    short_description_.assign(data + 4, data + i);
    text_.assign(data + i + 1, data + size);
}

void CommentFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", language: " << language_ << ", text: " << text_ << std::endl; }

std::unordered_map<char, std::string> event_type_{
        {0x00, "padding "},
        {0x01, "end of initial silence"},
        {0x02, "intro start"},
        {0x03, "main part start"},
        {0x04, "outro start"},
        {0x05, "outro end"},
        {0x06, "verse start"},
        {0x07, "refrain start"},
        {0x08, "interlude start"},
        {0x09, "theme start"},
        {0x0A, "variation start"},
        {0x0B, "key change"},
        {0x0C, "time change"},
        {0x0D, "momentary unwanted noise (Snap, Crackle & Pop)"},
        {0xE, "sustained noise"},
        {0x0F, "sustained noise end"},
        {0x10, "intro end"},
        {0x11, "main part end"},
        {0x12, "verse end"},
        {0x13, "refrain end"},
        {0x14, "theme end"},
        {0x15, "profanity"},
        {0x16, "profanity end"},
        {0xFD, "audio end"},
        {0xFE, "audio file ends"},
};

ETCOFrame::ETCOFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Event timing codes";
    time_stamp_format_ = data[0];
    for (int i = 1; i < size - 5; i += 5)
        events_.emplace_back(data[i], std::string{data + i + 1, data + i + 5});
}

void ETCOFrame::print() {
    std::cout << "Frame: " << id_ << ", " << frame_description_ << ", time format: ";
    if (time_stamp_format_ == 1)
        std::cout << "using MPEG [MPEG] frames as unit";
    else
        std::cout << "using milliseconds as unit\n";
    for (auto& p : events_)
        std::cout << "    " << p.first << ' ' << p.second << '\n';
}

SYLTFrame::SYLTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Synchronised lyrics/text";
    encoding_ = data[0];
    language_.assign(data + 1, data + 4);
    time_stamp_format_ = data[4];
    content_type_ = data[5];
    content_descriptor_.assign(data + 5, data + size);
}

void SYLTFrame::print() {
    std::cout << "Frame: " << id_ << ", " << frame_description_ << ", data " << content_descriptor_ << std::endl;
}

RVA2Frame::RVA2Frame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Relative volume adjustment (2)";
    int i = 0;
    for (; data[i] != 0; ++i);
    identification_.assign(data + 1, data + i);
    ++i;
    for (; i < size; i += 5)
        channels_.emplace_back(data + i, data + i + 5);
}

void RVA2Frame::print() {
    std::cout << "Frame: " << id_ << ", " << frame_description_ << ", " << identification_ << '\n';
    for (auto& s : channels_)
        std::cout << "    " << s << '\n';
}

EQU2Frame::EQU2Frame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Equalisation (2)";
    interpolation_method_ = data[0];
    int i = 0;
    for (; data[i] != 0; ++i);
    identificator_.assign(data + 1, data + i);
    ++i;
    for (; i < size; i += 4)
        points_.emplace_back(*reinterpret_cast<int16_t*>(data + i), *reinterpret_cast<int16_t*>(data + 2));
}

void EQU2Frame::print() {
    std::cout << "Frame: " << id_ << ", " << frame_description_ << ", " << identificator_ << '\n';
    for (auto& p : points_)
        std::cout << "    " << p.first << ' ' << p.second << '\n';
}


APICFrame::APICFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Attached picture";
    encoding_ = data[0];
    int i = 1;
    for (; data[i] != 0; ++i);
    mime_type_.assign(data + 1, data + i);
    picture_type_ = data[i + 1];
    ++i;
    int j = i;
    for (; data[j] != 0; ++j);
    description_.assign(data + i, data + j);
    std::ofstream pic("../music/icon.png", std::ios_base::binary);
    pic.write(data + j + 1, size - j - 1);
    pic.close();
    picture_data_.assign(data + j + 1, data + size);
}

std::unordered_map<char, std::string> picture_type{
        {0x00, "Other"},
        {0x01, "32x32 pixels 'file icon' (PNG only)"},
        {0x02, "Other file icon"},
        {0x03, "Cover (front)"},
        {0x04, "Cover (back)"},
        {0x05, "Leaflet page"},
        {0x06, "Media"},
        {0x07, "Lead artist/performer/soloist"},
        {0x08, "Artist/permormer"},
        {0x09, "Conductor"},
        {0x0A, "Band/orchestra"},
        {0x0B, "Composer"},
        {0x0C, "Lyricist/text writer"},
        {0x0D, "Recording location"},
        {0xE, "During recording"},
        {0x0F, "During performance"},
        {0x10, "Movie/video screen capture"},
        {0x11, "A bright colored fish"},
        {0x12, "Illustration"},
        {0x13, "Band/artist logotype"},
        {0x14, "Publisher/studio logotype"}
};

void APICFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", " << picture_type[picture_type_] << ", " << "data is written to png file" << std::endl; }

PCNTFrame::PCNTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Play counter";
    counter_ = std::stoll(std::string{data, data + size});
}

void PCNTFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << "counter: " << counter_ << std::endl; }

POPMFrame::POPMFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Popularimeter";
    int i = 0;
    for (; data[i] != 0; ++i);
    email_.assign(data, data + i);
    rating_ = data[i + 1];
    counter_ = std::stoll(std::string{data + i + 2, data + size});
}

void POPMFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << ", email: " << email_ << ", rating:" << rating_ << ", counter" << counter_ << std::endl;}

RBUFFrame::RBUFFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Recommended buffer size";
    buffer_size_ = std::stoi(std::string{data, data + 3});
    embedded_info_flag_ = data[3];
    offset_to_next_tag_ = std::stoi(std::string{data + 4, data + size});
}

void RBUFFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_ << " " << buffer_size_ << std::endl; }

LINKFrame::LINKFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Linked information";
    frame_id_.assign(data, data + 4);
    int i = 0;
    for (; data[i] != 0; ++i);
    url_.assign(data + 4, data + i);
    additional_data_.assign(data + i + 1, data + size);
}

void LINKFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ' ' << frame_id_ << ", url: " << url_ << ", additional data: " << additional_data_ << std::endl; }

POSSFrame::POSSFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Position synchronisation";
    time_stamp_format_ = *data;
    position_.assign(data + 1, data + size);
}

void POSSFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", position: " << position_ <<  std::endl; }

USERFrame::USERFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Terms of use frame";
    text_encoding_ = *data;
    language_.assign(data + 1, data + 4);
    text_.assign(data + 4, data + size);
}

void USERFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", language: " << language_ << ", text: " << text_ << std::endl; }

OWNEFrame::OWNEFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Ownership frame";
    encoding_ = *data;
    int i = 0;
    for (; data[i] != 0; ++i);
    price_paid_.assign(data + 1, data + i);
    date_of_purchase_.assign(data + i + 1, data + i + 9);
    seller_.assign(data + 9, data + size);
}

void OWNEFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", price: " << price_paid_ << ", seller: " << seller_ << ", date: " << date_of_purchase_ << std::endl;}


COMRFrame::COMRFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Commercial frame";
    encoding_ = *data;
    int i = 0;
    for (; data[i] != 0; ++i);
    price_.assign(data + 1, data + i);
    valid_until_.assign(data + i + 1, data + i + 9);
    i += 9;
    int j = i;
    for (; data[j] != 0; ++j);
    contact_url_.assign(data + i, data + j);
    i = j + 1;
    recieved_as_ = data[i];
    i++;
    j = i;
    for (; data[j] != 0; ++j);
    name_of_seller_.assign(data + i, data + j);
    for (; data[j] == 0; ++j);
    i = j;
    for (; data[j] != 0; ++j);
    descriprion_.assign(data + i, data + j);
    for (; data[j] == 0; ++j);
    i = j;
    for (; data[j] != 0; ++j);
    picture_mime_type_.assign(data + i, data + j);
    seller_logo_.assign(data + j + 1, data + size);
}

void COMRFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << std::endl; }

ENCRFrame::ENCRFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Encryption method registration";
    int i = 0;
    for (; data[i] != 0; ++i);
    owner_identifier_.assign(data, data + i);
    method_symbol_ = data[i + 1];
    encryption_data_.assign(data + i + 2, data + size);
}

void ENCRFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", owner id: " << owner_identifier_ << std::endl; }

GRIDFrame::GRIDFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Group ID registration";
    int i = 0;
    for (; data[i] != 0; ++i);
    owner_identifier_.assign(data, data + i);
    group_symbol_ = data[i + 1];
    dependent_data_.assign(data + i + 2, data + size);
}

void GRIDFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", owner id: " << owner_identifier_ << std::endl; }

PRIVFrame::PRIVFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Private frame";
    int i = 0;
    for (; data[i] != 0; ++i);
    owner_identifier_.assign(data, data + i);
    dependent_data_.assign(data + i + 1, data + size);
}

void PRIVFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << ", owner id: " << owner_identifier_ << std::endl; }

SEEKFrame::SEEKFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data) : Frame(id, size, flags) {
    frame_description_ = "Private frame";
    for (int i = 0; i < 4; ++i) minimum_offset_to_next_tag_[i] = data[i];
}

void SEEKFrame::print() { std::cout << "Frame: " << id_ << ", " << frame_description_  << std::endl; }
