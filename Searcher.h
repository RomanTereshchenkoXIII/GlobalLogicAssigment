#pragma once

#include <string>
#include <iostream>
#include<vector>
#include <filesystem>
#include<thread>
#include<mutex>
#include"windows.h"

#define LAA(se) {{se},SE_PRIVILEGE_ENABLED|SE_PRIVILEGE_ENABLED_BY_DEFAULT}

#define BEGIN_PRIVILEGES(tp, n) static const struct {ULONG PrivilegeCount;LUID_AND_ATTRIBUTES Privileges[n];} tp = {n,{
#define END_PRIVILEGES }};

#define SE_BACKUP_PRIVILEGE (17L)

namespace fs = std::filesystem;

class Searcher {

private:
    std::string file;
    fs::path directory;
    bool found;
    std::mutex mtx;
public:
    Searcher();
    Searcher(std::string f);
    Searcher(std::string f, fs::path d);

    ULONG AdjustPrivileges();
    void Threads ();
    void ScanDirectory();
    fs::path SetFile();
};