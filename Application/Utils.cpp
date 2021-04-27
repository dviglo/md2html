#include "Utils.h"
#include "Utf8Str.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;


string NormalizePath(string path)
{
    replace(path.begin(), path.end(), '\\', '/');

    while (!path.empty() && path.back() == '/')
        path.pop_back();

    return path;
}


void ReadDir(const string& dirPath, vector<string>& outFiles, vector<string>& outSubdirs)
{
    outFiles.clear();
    outSubdirs.clear();

    for (auto& entry : fs::directory_iterator(dirPath))
    {
        if (entry.is_directory())
            outSubdirs.push_back(entry.path().filename().string());
        else
            outFiles.push_back(entry.path().filename().string());
    }
}


bool StartsWith(const string& str, const string& value)
{
    return str.rfind(value, 0) == 0;
}


string CutStart(const string& str, const string& value)
{
    if (!StartsWith(str, value))
        return str;

    return str.substr(value.length());
}


string ReplaceStart(const string& str, const string& from, const string& to)
{
    if (!StartsWith(str, from))
        return str;

    return to + CutStart(str, from);
}


bool EndsWith(const string& str, const string& value)
{
    if (value.size() > str.size())
        return false;

    return equal(value.rbegin(), value.rend(), str.rbegin());
}


string CutEnd(const string& str, const string& value)
{
    if (!EndsWith(str, value))
        return str;

    return str.substr(0, str.length() - value.length());
}


string ReplaceEnd(const string& str, const string& from, const string& to)
{
    if (!EndsWith(str, from))
        return str;

    return CutEnd(str, from) + to;
}


string ReplaceAll(const string& src, const string& from, const string& to)
{
    string ret;

    size_t lastPos = 0;
    size_t findPos = src.find(from, lastPos);

    while (findPos != string::npos)
    {
        ret.append(src, lastPos, findPos - lastPos);
        ret += to;
        lastPos = findPos + from.length();
        findPos = src.find(from, lastPos);
    }

    ret += src.substr(lastPos);

    return ret;
}


vector<string> Split(const string& str, char delim)
{
    stringstream ss(str);
    string item;
    vector<string> result;

    while (getline(ss, item, delim))
        result.push_back(move(item));

    return result;
}


string Trim(const string& str)
{
    size_t trimStart = 0;
    size_t trimEnd = str.length();

    while (trimStart < trimEnd)
    {
        char c = str.c_str()[trimStart];
        if (c != ' ' && c != '\t')
            break;
        ++trimStart;
    }

    while (trimEnd > trimStart)
    {
        char c = str.c_str()[trimEnd - 1];
        if (c != ' ' && c != '\t')
            break;
        --trimEnd;
    }

    return str.substr(trimStart, trimEnd - trimStart);
}


string ReadAllText(const string& path)
{
    ifstream ifs(path);
    stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}


void WriteAllText(const string& path, const string& value)
{
    ofstream ofs(path);
    ofs << value;
}


string ToLower(const string& src)
{
    char* buffer = new char[src.length() + 1];
    strcpy(buffer, src.c_str());
    StrToLwrExt((unsigned char*)buffer);
    string result(buffer);
    delete[] buffer;
    return result;
}
