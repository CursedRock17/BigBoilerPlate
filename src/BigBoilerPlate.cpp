#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <memory>

#include "../include/BigBoilerPlate.h"

//Create a project installer that sets up the file

void BoilerPlate::take_input(){
        std::cout << "Hello, from BigBoilerPlate we will setup a base project for you!\n";

        std::cout << "What is your project name?" << std::endl;
        std::cin >> project_name;

        std::cout << "Would you like to use CMake? (yes or no)" << std::endl;
        std::cin >> use_cmake;

        if(use_cmake == "yes"){
            std::cout << "Choice of package manager (vcpkg, conan, or none)" << std::endl;
            std::cin >> package_manager;

            if(package_manager == "vcpkg" || package_manager == "conan"){
                std::cout << "Begin Listing Packages you would like to use and install then say: Done" << std::endl;
                while(current_package != "Done"){
                    std::cin >> current_package;
                    if(current_package != "Done")
                        packages.push_back(current_package);
                }
            }
        }

        std::cout << "List any extra top level folders, when finished say: Done" << std::endl;
        while(current_folder != "Done"){
            std::cin >> current_folder;
            if(current_folder != "Done")
                folders.push_back(current_folder);
        }

}

std::string BoilerPlate::getOsName()
{
    #ifdef _WIN32
    return "Windows";
    #elif _WIN64
    return "Windows";
    #elif __APPLE__ || __MACH__
    return "Mac";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}   

void BoilerPlate::add_installs()
{
    //Begin installs for package manager and whatever, can be a multithreaded function
    std::string current_OS = getOsName();

    if(package_manager == "vcpkg")
    {
        system("cd ~");

        if(current_OS != "Window")
        {
            int installed = system("~/vcpkg/vcpkg --version");

            if(installed != 0){
                //Begin install of vcpkg
                std::cout << "Installing vcpkg for you, at your root directory";
                system("git clone https://github.com/Microsoft/vcpkg.git");
                system("~/vcpkg/bootstrap-vcpkg.sh");
            }  
                //Begin install libraries
            for(auto const& lib : packages){
                std::string fitted_lib = "~/vcpkg/vcpkg install " + lib;
                const char* char_string = fitted_lib.c_str();
                system(char_string);

                std::array<char, 128> buffer;
                std::string result;
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("~/vcpkg/vcpkg install fmt", "r"), pclose);

                while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                    result += buffer.data();
                }

                writing_file.close();
                writing_file.open("CMakeLists.txt", std::ios_base::app);

                writing_file << result << std::endl;
            }
        }

        else {

        }
        

        writing_file.close();
        writing_file.open("CMakeLists", std::ios_base::app);
    }

    
}


void BoilerPlate::create_system()
{
    namespace fs = std::filesystem;
    const std::string quote = "\"";
    //First we can fetch the OS as a MACRO, then we need to begin building out the file structure

    fs::path main_path = fs::current_path();

    //Create the original folder to hold the top level main
    writing_file.close();
    writing_file.open("main.cpp", std::ios_base::app);

    writing_file << "#include <iostream>" << std::endl << std::endl;

    if(use_cmake == "yes" || use_cmake == "Yes") 
    {
        //Need to create and then add onto the main Cmake file before we append to it later
        writing_file.close();
        writing_file.open("CMakeLists.txt", std::ios_base::app);

    //May want to get macros of most cpp versions
        writing_file << "cmake_minimum_required(VERSION 3.23)" << std::endl << std::endl <<
        "project(" << project_name << "VERSION 1.0)" << std::endl << std::endl 
        << "set(CMAKE_CXX_STANDARD 20)" << std::endl << std::endl << "set(CMAKE_CXX_STANDARD_REQUIRED TRUE)" << std::endl
        << "add_executable(" << project_name << " main.cpp)" << std::endl << std::endl;
    }

    //We need to go through each folder, enter it, then write a cpp, h, and CMake test file
    for(auto path : folders){
        fs::create_directory(path);

        //          CPP Section         //
        writing_file.close();
        writing_file.open(path + "/" + path + ".cpp", std::ios::out);
        

        //At this point the file should be opened and created therefore write out a 
        //Sample class for the user to get started with
        writing_file << "#include " << quote << path << ".h" << quote << std::endl
        << std::endl << path << "::" << path << "(){}" << std::endl
        << path << "::~" << path << "(){}" << std::endl;
        
        //          CPP Section         //


        //          Header Section         //
        writing_file.close();
        writing_file.open(path + "/" + path + + ".h", std::ios::out);

        //At this point the file should be opened and created therefore write out a 
        //Sample class for the user to get started with
        writing_file << "#ifndef " << path << "_H" << std::endl << "#define " << path 
        << "_H" << std::endl << std::endl << "class " << path << " {" << std::endl <<
        std::endl << "public:" << std::endl << "    " << path << "();" << std::endl  << "    "  << "~" << path << "();" << std::endl
        << std::endl << "private:" << std::endl << "};" << std::endl << std::endl << "#endif" << std::endl;
        
        //          Header Section         //

        //          Cmake Section          //
        if(use_cmake == "yes" || use_cmake == "Yes")
        {
            writing_file.close();
            writing_file.open(path + "/CMakeLists.txt", std::ios::out);

            writing_file << "add_library(" << path << " " << path << ".cpp)" << std::endl; 

            //Deal with the main cmake in this command to limit system strain
            writing_file.close();
            writing_file.open("CMakeLists.txt", std::ios_base::app);

            writing_file << "add_subdirectory(" << path << ")" << std::endl;
            writing_file << "target_link_libraries(" << project_name << "PUBLIC" << path << ")" << std::endl;
        }

        //          Cmake Section          //


        // Main Section Files //    
        writing_file.close();
        writing_file.open("main.cpp", std::ios_base::app);
    
        writing_file << "#include " << quote << path << "/" << path << ".h" << quote << std::endl;

        // Main Section Files //
        

    }

    //Now to include all the users extended libraries i.e: boost
    if(package_manager == "vcpkg" || package_manager == "conan")
    {
        std::thread work_system(&BoilerPlate::add_installs, this);

        work_system.join();
    }


    //Finish up the main file
    writing_file.close();
    writing_file.open("main.cpp", std::ios_base::app);

    writing_file << std::endl << "int main (int argc, char *argv[])" << std::endl << "{" << std::endl << "return 0;" << std::endl << "}";
}
