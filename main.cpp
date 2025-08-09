#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <vector>

// Ls POSIX functionality


void displayDirectoryEntries(const std::vector<dirent>& directory_entries, std::string directory_name) {

    for(std::vector<dirent>::const_iterator t = directory_entries.begin(); t!= directory_entries.end(); t++) {
        std::cout<<t->d_ino<<" "<<t->d_name<<" "<<t->d_type<<std::endl;
        struct stat buffer;
        std::string filename = directory_name + "/" + t->d_name;
        int value = stat(filename.c_str(), &buffer);
        if(value == 0) {
            passwd* owner = getpwuid(buffer.st_uid);
            if(owner != nullptr) {
                std::cout<<"Owner:"<<owner->pw_name<<std::endl;
            }
            std::cout<<"Permission Bits: "<<(buffer.st_mode & 0777)<<std::endl;
        }
    }
}

int main() {

    std::string directory_name = "./Example";
    DIR * directory_pointer = opendir(directory_name.c_str());

    if(directory_pointer == nullptr) {
        std::cerr<<errno;
        return -1;
    }

    std::vector<dirent> directory_entries;
    dirent* directory_entry = readdir(directory_pointer);
    
    while(directory_entry != NULL) {
        directory_entries.push_back(*directory_entry);
        directory_entry = readdir(directory_pointer);
    }


    displayDirectoryEntries(directory_entries, directory_name);
    closedir(directory_pointer);
    return 0;

}