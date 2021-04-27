#pragma once

#include <string>
#include <vector>

using namespace std;


// Заменяет \ на / и убирает слэш в конце
string NormalizePath(string path);

void ReadDir(const string& dirPath, vector<string>& outFiles, vector<string>& outSubdirs);

bool StartsWith(const string& str, const string& value);
string CutStart(const string& str, const string& value);
string ReplaceStart(const string& str, const string& from, const string& to);

bool EndsWith(const string& str, const string& value);
string CutEnd(const string& str, const string& value);
string ReplaceEnd(const string& str, const string& from, const string& to);

string ReplaceAll(const string& src, const string& from, const string& to);
vector<string> Split(const string& str, char delim);
string Trim(const string& str);
string ToLower(const string& src);

string ReadAllText(const string& path);
void WriteAllText(const string& path, const string& value);
