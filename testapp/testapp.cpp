#include <iostream>

#include <Mumble.h>

Mumble mumble;

int main(int argc, char* argv[])
{
    std::cout << "Mumble FiveM library test app!" << std::endl;

    std::string name = std::string(argv[1]);
    mumble.Test(name);

    return 0;
}
