#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Frame {
public:
    std::string id_;
    size_t size_;
    std::pair<char, char> flags_;
    std::string frame_description_;
    Frame(const std::string& id, size_t size, const std::pair<char, char>& flags);

    virtual void print();
};

class TextFrame : public Frame {
private:
    char encoding_;
    std::string data_;
public:
    TextFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};

const std::unordered_set<std::string> text_frames{
        "TIT1", "TIT2", "TIT3", "TALB", "TOAL", "TRCK", "TPOS", "TSST", "TSRC", "TPE1", "TPE2", "TPE3", "TPE4", "TOPE",
        "TEXT", "TOLY", "TCOM", "TMCL", "TIPL", "TENC", "TBPM", "TLEN", "TKEY", "TLAN", "TCON", "TMOO", "TCOP", "TPRO",
        "TPUB", "TOWN", "TRSN", "TRSO", "TOFN", "TDLY", "TDEN", "TDOR", "TDRC", "TDRL", "TDTG", "TSSE", "TSOA", "TSOP",
        "TSOT", "TXXX"
};


class UFIDFrame : public Frame {
public:
    std::string owner_identifier_;
    std::string identifier_;

    UFIDFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};



class UrlFrame : public Frame {
public:
    UrlFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string url_;
};

class WXXXFrame : public Frame {
public:
    WXXXFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char encoding_;
    std::string desription_;
    std::string url_;
};

const std::unordered_set<std::string> url_frames{
    "WCOM", "WCOP", "WOAF", "WOAR", "WOAS", "WORS", "WPAY", "WPUB"
};

class ETCOFrame : public Frame {
private:
    char time_stamp_format_;
    std::vector<std::pair<char, std::string>> events_;
public:
    ETCOFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};


class USLTFrame : public Frame {
public:
    USLTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char encoding_;
    std::string language_;
    std::string content_descriptor_;
    std::string lyrics_;
};

class SYLTFrame : public Frame {
private:
    char encoding_;
    std::string language_;
    char time_stamp_format_;
    char content_type_;
    std::string content_descriptor_;
public:
    SYLTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};

class RVA2Frame : public Frame {
private:
    std::string identification_;
    std::vector<std::string> channels_;
public:
    RVA2Frame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};

class EQU2Frame : public Frame {
private:
    char interpolation_method_;
    std::string identificator_;
    std::vector<std::pair<int16_t, int16_t>> points_;
public:
    EQU2Frame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
};

class CommentFrame : public Frame {
public:
    CommentFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char encoding_;
    std::string language_;
    std::string short_description_;
    std::string text_;
};

class APICFrame : public Frame {
public:
    APICFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char encoding_;
    std::string mime_type_;
    char picture_type_;
    std::string description_;
    std::string picture_data_;
};

class PCNTFrame : public Frame {
public:
    PCNTFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    long long counter_;
};

class POPMFrame : public Frame {
public:
    POPMFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string email_;
    char rating_;
    long long counter_;
};

class RBUFFrame : public Frame {
public:
    RBUFFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    uint32_t buffer_size_;
    char embedded_info_flag_;
    int32_t offset_to_next_tag_;
};

class LINKFrame : public Frame {
public:
    LINKFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string frame_id_;
    std::string url_;
    std::string additional_data_;
};

class POSSFrame : public Frame {
public:
    POSSFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char time_stamp_format_;
    std::string position_;
};

class USERFrame : public Frame {
public:
    USERFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char text_encoding_;
    std::string language_;
    std::string text_;
};

class OWNEFrame : public Frame {
public:
    OWNEFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char encoding_;
    std::string price_paid_;
    std::string date_of_purchase_;
    std::string seller_;
};

class COMRFrame : public Frame {
public:
    COMRFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
   char encoding_;
   std::string price_;
   std::string valid_until_;
   std::string contact_url_;
   char recieved_as_;
   std::string name_of_seller_;
   std::string descriprion_;
   std::string picture_mime_type_;
   std::string seller_logo_;
};

class ENCRFrame : public Frame {
public:
    ENCRFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string owner_identifier_;
    char method_symbol_;
    std::string encryption_data_;
};

class GRIDFrame : public Frame {
public:
    GRIDFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string owner_identifier_;
    char group_symbol_;
    std::string dependent_data_;
};

class PRIVFrame : public Frame {
public:
    PRIVFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    std::string owner_identifier_;
    std::string dependent_data_;
};

class SEEKFrame : public Frame {
public:
    SEEKFrame(const std::string& id, size_t size, const std::pair<char, char>& flags, char* data);
    void print() override;
private:
    char minimum_offset_to_next_tag_[4];
};
