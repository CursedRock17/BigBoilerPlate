#ifndef BIGBOILERPLATE_H
#define BIGBOILERPLATE_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm> 
#include <thread>


class BoilerPlate {
public:
    void take_input();
    void create_system();

private:
    std::string use_cmake;
    std::string package_manager;
    std::string current_package;
    std::string current_folder;
    std::string path_uppercase;
    std::string project_name;

    const std::string quote = "\"";

    std::vector<std::string> folders;
    std::vector<std::string> packages;

    //End of Globals 

    //Create a file stream object
    std::ifstream reading_file;
    std::ofstream writing_file;

    //Private functions
    std::string getOsName();
    void add_installs();
};

#endif