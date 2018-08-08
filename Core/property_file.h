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
    std::wstring filename;
    std::unordered_map<std::string,std::string> hash;
    bool modified = false;
public:
    PropertyFile(const std::wstring& filename) {
        this->filename = filename;
        load();
    }
    ~PropertyFile() {
        save();
    }
    ulong size() const { return hash.size(); }
    bool isModified() const { return modified; }
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
    auto& set(const std::string& key, const std::string& value) {
        hash[key] = value;
        modified = true;
        return *this;
    }
    auto& set(const std::string& key, const slong value) {
        return set(key, String::format("%lld",value));
    }
    auto& set(const std::string& key, const double value) {
        return set(key, String::format("%f", value));
    }
private:
    void load() {
        if(!File::exists(filename)) return;
        FileReader<> reader{filename};
        int i=0;
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
            i++;
        }
    }
    void save() {
        if(!modified) return;
        FileWriter writer{filename};
        for(auto& kv : hash) {
            writer.writeFmt("%s = %s\n", kv.first.c_str(), kv.second.c_str());
        }
        modified = false;
    }
};

}
