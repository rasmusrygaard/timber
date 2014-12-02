#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

class Util {
public:
    static void run_cmd(const std::string&);
    static void run_cmd_async(const std::string&);

private:
    static void run_cmd_as_thread(const std::string&);
};

#endif
