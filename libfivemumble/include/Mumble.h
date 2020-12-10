#pragma once

#include <string>
#include <vector>
#include <array>
#include "CompilationControl.h"
#include "AudioDeviceInfo.h"

class MumbleClient;

enum class EXPORT MumbleConnectionEventType : int
{
	Connected = 0,
	ConnectionError,
	Disconnected
};

typedef void(__stdcall* ConnectionCallbackHandler)(MumbleConnectionEventType type, const std::string info);
typedef void(__stdcall* TextMessageCallbackHandler)(int user, const std::string& message);
typedef void(__stdcall* PositionCallbackHandler)(std::array<float, 3>& position);

class EXPORT Mumble
{

public:

	Mumble(ConnectionCallbackHandler conHandler, TextMessageCallbackHandler msgHandler, PositionCallbackHandler posHandler);
	~Mumble();

	static std::vector<AudioDeviceInfo> GetInputDevices();
	static std::vector<AudioDeviceInfo> GetOutputDevices();

	void ConnectAsync(const std::string& address, const std::string& username, const std::string& password, int port = 64738);
	void DisconnectAsync();

	void SetInputDevice(const std::string& guid);
	void SetOutputDevice(const std::string& guid);

	void SetChannel(const std::string& channel);
	bool SendTextMessage(const std::string& message);
	void RunFrame();

	bool IsConnected() { return connected; }
	std::string& GetCurrentChannel() { return currentChannel; }

private:
	MumbleClient* client;

	ConnectionCallbackHandler connectionHandler;
	TextMessageCallbackHandler textMessageHandler;
	PositionCallbackHandler positionHandler;

	bool connected;
	std::string currentChannel;
	std::array<float, 3> currentPosition;

	void HandleConnectionCallback(MumbleConnectionEventType type, const std::string& info);

};
