#ifndef _CFGREADER_H
#define _CFGREADER_H

#include <stdio.h>
#include <sstream>
#include <map>
#include <string>
#include <functional>

#include "strop.h"

typedef std::map<std::string, std::string> KVMap;

class CfgReader
{
private:
    void PushError(int line, const char *msg)
    {
        std::ostringstream ost;
        ost << "error at line " << line << ": " << msg;
        err_msg_.push_back(ost.str());
    }

public:
    bool Read(const char *file)
    {
        err_msg_.clear();

        FILE *fp = fopen(file, "r");
        if (fp == 0) {
            return false;
        }

        char bom[3] = { 0 };
        fread(bom, 1, sizeof(bom), fp);
        if (memcmp(bom, "\xEF\xBB\xBF", 3) != 0) {
            fseek(fp, 0, SEEK_SET);
        }

        int line_no = 1;

        char line[1000] = { 0 };
        while (fgets(line, sizeof(line), fp)) {
            line_no++;

            std::string l(TrimString(std::string(line)));
            if (l.empty() || l[0] == '#') {
                continue;
            }

            auto kv = SplitString(l, '=');
            if (kv.size() != 2) {
                PushError(line_no, "invalid k/v, skip it.");
                continue;
            }
            content_[kv[0]] = kv[1];
        }

        fclose(fp);
        return true;
    }

    std::string operator[](const std::string& key)
    {
        auto it = content_.find(key);
        if (it == content_.end()) {
            return "";
        }
        return it->second;
    }

    void ForEach(const std::function<void(const std::string&, const std::string&)>& func)
    {
        for (auto it = content_.begin(); it != content_.end(); it++) {
            func(it->first, it->second);
        }
    }

private:
    std::vector<std::string> err_msg_;
    std::map<std::string, std::string> content_;
};

#endif  // _CFGREADER_H
