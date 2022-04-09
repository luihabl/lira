#pragma once 

#include <tinysdl.h>
#include <string>
#include <filesystem>

namespace Lira {

    struct SoundBank 
    {
        std::string name = "";
        std::string platform = "";
        
        SoundBank() = default;
        SoundBank(const std::string& _name, const std::string& _platform) : name(_name), platform(_platform) {}
    };

}