#pragma once
/*
    Simple property file reader.

    # comment line
    key = value
    
    # empty lines are ignored
*/

#include <functional>

namespace core {

class PropertyFile final {
    std::unordered_map<std::string,std::string> hash;
public:
    PropertyFile(const std::wstring& filename) {
        load(filename);
    }
    const std::string& get(const std::string& key, const std::string& defaultValue) {
        auto v = hash.find(key);
        if(v==hash.end()) return defaultValue;
        return v->second;
    }
    slong get(const std::string& key, slong defaultValue) {
        auto v = hash.find(key);
        if(v == hash.end()) return defaultValue;
        return _atoi64(v->second.c_str());
    }
    double get(const std::string& key, double defaultValue) {
        auto v = hash.find(key);
        if(v == hash.end()) return defaultValue;
        return atof(v->second.c_str());
    }
    template<typename T, typename FUNCTOR>
    auto get(const std::string& key, T defaultValue, const FUNCTOR& parser) {
        auto v = hash.find(key);
        if(v == hash.end()) return defaultValue;
        return parser(v->second);
    }
private:
    void load(const std::wstring& filename) {
        FileReader<> reader{filename};
        while(!reader.eof()) {
            auto line = reader.readLine();
            line = String::trimBoth(line);
            if(line.empty()) continue;
            if(String::startsWith(line, "#")) continue;

            auto vec = String::split(line,'=');
            if(vec.size()!=2) throw std::runtime_error("Badly formatted property file");

            auto key   = String::trimBoth(vec[0]);
            auto value = String::trimBoth(vec[1]);
            hash[key] = value;
        }
    }
};

}
