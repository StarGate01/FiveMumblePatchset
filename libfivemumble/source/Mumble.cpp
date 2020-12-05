#include "Mumble.h"

#include <iostream>

#include <StdInc.h>
#include <CoreConsole.h>
#include <UvLoopHolder.h>
#include <MumbleClientImpl.h>
#include <codecvt>

#include <AudioUtils.h>

Mumble::Mumble()
{

}

std::wstring_convert<std::codecvt_utf8<wchar_t>> wideToNarrow;

MumbleClient client;

std::vector<AudioDevice> inputDevices;
std::vector<AudioDevice> outputDevices;

void Mumble::Test(const std::string& name)
{
	// Handle audio devices
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	ListDevices(true, inputDevices);
	ListDevices(false, outputDevices);
	std::cout << "Input devices:" << std::endl;
	for (int i = 0; i < inputDevices.size(); i++) std::cout << "  " << i << ": " <<
		inputDevices[i].name << std::endl << "    " << inputDevices[i].guid << std::endl;
	std::cout << "Output devices:" << std::endl;
	for (int i = 0; i < outputDevices.size(); i++) std::cout << "  " << i << ": " <<
		outputDevices[i].name << std::endl << "    " << outputDevices[i].guid << std::endl;

	// Handle server name
	auto remote = net::PeerAddress::FromString("chrz.de", 64738, net::PeerAddress::LookupType::ResolveName);
	if (!remote.has_value())
	{
		std::cerr << "Cannot resolve remote address!" << std::endl;
		return;
	}

	// Setup client
	client.Initialize();
	client.SetInputDevice(inputDevices[0].guid);
	client.SetOutputDevice(outputDevices[0].guid);
	client.SetPositionHook([](const std::string& name) {
		return std::optional<std::array<float, 3>>({ 0.0f, 0.0f, 0.0f });
		});

	client.ConnectAsync(remote.get(), name, "grafkuerb").then([](concurrency::task<MumbleConnectionInfo*> task)
		{
			try
			{
				auto info = task.get();
				std::cout << "Connected, server: " << info->address.ToString() <<
					", user: " << info->username << ", connected: " << (info->isConnected ? "true" : "false") <<
					", connecting: " << (info->isConnecting ? "true" : "false") << std::endl;

				client.SetChannel("CHRZ");
				client.AddListenChannel("CHRZ");
			}
			catch (std::exception& e)
			{
				trace("Error connecting: %s\n", e.what());
			}
		});

	bool ESCAPE = false;
	while (!ESCAPE)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			ESCAPE = true;
			break;
		}

		client.RunFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (client.GetConnectionInfo()->isConnected)
	{
		client.DisconnectAsync().wait();
	}
}