#include <thread>
#include <stdlib.h>
#include <iostream>

#include "util.h"

void Util::run_cmd_as_thread(const std::string& cmd) {
    system(cmd.c_str());
}

void Util::run_cmd(const std::string& cmd) {
    std::cout << "Running: '" << cmd << "'" << std::endl;
    std::thread t(Util::run_cmd_as_thread, cmd);
    t.join();
    std::cout << "Done." << std::endl;
}

void Util::run_cmd_async(const std::string& cmd) {
    std::cout << "Running async: '" << cmd << "'" << std::endl;
    std::thread t(Util::run_cmd_as_thread, cmd);
    t.detach();
    std::cout << "Done running async." << std::endl;
}
