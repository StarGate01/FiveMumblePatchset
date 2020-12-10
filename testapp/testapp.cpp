#include <iostream>
#include <thread>
#include <chrono>
#include "Windows.h"

#include <Mumble.h>

void handleConnection(MumbleConnectionEventType type, const std::string info)
{
	std::cout << "Connection info (type " << (int)type << "): " << info << std::endl;
}

void handleTextMessage(int user, const std::string message)
{
	std::cout << "Text message: " << message << std::endl;
}

void updatePosition(std::array<float, 3>& position)
{
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;
}

Mumble mumble((ConnectionCallbackHandler)handleConnection, 
	(TextMessageCallbackHandler)handleTextMessage, (PositionCallbackHandler)updatePosition);

int main(int argc, char* argv[])
{
    std::cout << "Mumble FiveM library test app!" << std::endl;

    std::string name = std::string(argv[1]);

	std::cout << "Input devices: " << std::endl;
	std::vector<AudioDeviceInfo> inputDevices = Mumble::GetInputDevices();
	for (auto& device: inputDevices)
		std::cout << "  " << device.name << std::endl << "    " << device.guid << std::endl;
	std::cout << "Output devices: " << std::endl;
	std::vector<AudioDeviceInfo> outputDevices = Mumble::GetOutputDevices();
	for (auto& device : outputDevices)
		std::cout << "  " << device.name << std::endl << "    " << device.guid << std::endl;

	mumble.SetInputDevice(inputDevices[0].guid);
	mumble.SetOutputDevice(outputDevices[1].guid);

	mumble.ConnectAsync("chrz.de", name, "grafkuerb");

	bool ESCAPE = false;
	while (!ESCAPE)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			ESCAPE = true;
			break;
		}

		if (mumble.IsConnected() && mumble.GetCurrentChannel() != "CHRZ")
		{
			mumble.SetChannel("CHRZ");
		}

		mumble.RunFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (mumble.IsConnected())
	{
		mumble.DisconnectAsync();
	}

    return 0;
}
