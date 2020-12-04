#include <iostream>

#include <StdInc.h>
#include <CoreConsole.h>
#include <UvLoopHolder.h>
#include <MumbleClientImpl.h>
#include <codecvt>

#include <dsound.h>

std::wstring_convert<std::codecvt_utf8<wchar_t>> wideToNarrow;

MumbleClient client;

struct AudioDevice
{
	GUID guid;
	std::string guidString;
	std::string name;
};

std::vector<AudioDevice> inputDevices;
std::vector<AudioDevice> outputDevices;

LPDSENUMCALLBACKW enumCb = [](LPGUID guid, LPCWSTR desc, LPCWSTR, void* cxt) -> BOOL
{
	if (guid != nullptr)
	{
		((std::vector<AudioDevice>*)cxt)->push_back({
			*guid,
			fmt::sprintf("{%08lX-%04hX-%04hX-%02hX%02hX-%02hX%02hX%02hX%02hX%02hX%02hX}", guid->Data1, guid->Data2, guid->Data3,
				guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
				guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]),
			wideToNarrow.to_bytes(std::wstring(desc))
			});
	}
	return TRUE;
};


static void LogPrintListener(ConsoleChannel channel, const char* msg)
{
	std::cout << "[" << channel << "] " << fmt::sprintf("%s", msg);
}

int main()
{
    std::cout << "Mumble FiveM library test app!" << std::endl;

	console::CoreAddPrintListener(LogPrintListener);

    auto remote = net::PeerAddress::FromString("chrz.de", 64738, net::PeerAddress::LookupType::ResolveName);
    if (!remote.has_value())
    {
        std::cerr << "Cannot resolve remote address!" << std::endl;
        return 1;
    }

	DirectSoundCaptureEnumerateW(enumCb, &inputDevices);
	DirectSoundEnumerateW(enumCb, &outputDevices);
	std::cout << "Input devices:" << std::endl;
	for (int i = 0; i < inputDevices.size(); i++) std::cout << "  " << i << ": " << 
		inputDevices[i].name << std::endl << "    " << inputDevices[i].guidString << std::endl;
	std::cout << "Output devices:" << std::endl;
	for (int i = 0; i < outputDevices.size(); i++) std::cout << "  " << i << ": " << 
		outputDevices[i].name << std::endl << "    " << outputDevices[i].guidString << std::endl;

    client.Initialize();

	std::cout << "Setting input device" << std::endl;
	client.SetInputDevice(inputDevices[0].guidString);

	std::cout << "Setting output device" << std::endl;
	client.SetOutputDevice(outputDevices[0].guidString);
	
	client.SetChannel("CHRZ");
    client.ConnectAsync(remote.get(), "testapp").then([](concurrency::task<MumbleConnectionInfo*> task)
	{
		try
		{
			auto info = task.get();
			std::cout << "Connected, server: " << info->address.ToString() <<
				", user  " << info->address.ToString() << ", connected:" << (info->isConnected ? "true" : "false") <<
				"connecting, :" << (info->isConnecting ? "true" : "false");
		}
		catch (std::exception& e)
		{
			trace("Error connecting: %s\n", e.what());
		}
	});

	std::cin.get();

	if (client.GetConnectionInfo()->isConnected)
	{
		client.DisconnectAsync().wait();
	}

    return 0;
}
