#include"Searcher.h"

Searcher::Searcher() :directory("C:\\"), found(false){
}

Searcher::Searcher(std::string  f) : file(f), directory("C:\\"), found(false){
}

Searcher::Searcher(std::string f, fs::path d) : file(f), directory(d), found(false) {
}

ULONG Searcher::AdjustPrivileges()
{
    if (ImpersonateSelf(SecurityImpersonation))
    {
        HANDLE hToken;
        if (OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, TRUE, &hToken))
        {
            BEGIN_PRIVILEGES(tp, 1)
                LAA(SE_BACKUP_PRIVILEGE),
            END_PRIVILEGES
            AdjustTokenPrivileges(hToken, FALSE, (PTOKEN_PRIVILEGES)&tp, 0, 0, 0);
            CloseHandle(hToken);
        }
    }
    return GetLastError();
}

void Searcher::Threads () {
    std::vector<std::thread> threads;
    unsigned num_threads = 8;
        for (unsigned i = 0; i < num_threads; i++) {
            threads.push_back(std::thread(ScanDirectory, this));
    }
    for (unsigned i = 0; i < num_threads; i++) {
        threads[i].join();
    }
}

void Searcher::ScanDirectory() {
    AdjustPrivileges();
    std::string filePath;
    mtx.lock();
    try {
        for (auto& p : fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            if (p.is_directory()) {
                mtx.unlock();
            }
            if (p.path().filename().string() == file) {
                found = true;
                filePath = p.path().parent_path().string();
                std::cout <<  "Path to the file: " << file  << ": " << p.path().parent_path().string() << std::endl;
                break;
            }
        }
    }
    catch (fs::filesystem_error& er) {
        std::cout << er.code() << std::endl;
        std::cout << er.what() << std::endl;
        std::cout << er.path1() << std::endl;
        std::cout << er.path2() << std::endl;
    }
} 

fs::path Searcher::SetFile() {
        std::cout << "Enter name of a file: ";
        std::cin >> file;
        return file;
}