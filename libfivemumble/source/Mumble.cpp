#include "Mumble.h"

#include <iostream>

#include <StdInc.h>
#include <CoreConsole.h>
#include <UvLoopHolder.h>
#include <MumbleClientImpl.h>
#include <codecvt>

#include <AudioUtils.h>


Mumble::Mumble(ConnectionCallbackHandler conHandler, TextMessageCallbackHandler msgHandler, PositionCallbackHandler posHandler):
	connectionHandler(conHandler),
	textMessageHandler(msgHandler),
	positionHandler(posHandler)
{
	client = new MumbleClient();

	// Initialize and register components
	InitFunctionBase::RunAll();

	// Setup command security
	se::Principal devp = se::Principal{ "system.developer" };
	se::Principal mump = se::Principal{ "system.mumble" };
	se::Context* ctx = seGetCurrentContext();
	ctx->PushPrincipal(devp);
	ctx->PushPrincipal(mump);
	ctx->AddAccessControlEntry(devp, se::Object{ "command.developer" }, se::AccessType::Allow);
	ctx->AddAccessControlEntry(mump, se::Object{ "command.voice_inBitrate" }, se::AccessType::Allow);
	ctx->AddAccessControlEntry(mump, se::Object{ "command.voice_use3dAudio" }, se::AccessType::Allow);
	ctx->AddAccessControlEntry(mump, se::Object{ "command.voice_useSendingRangeOnly" }, se::AccessType::Allow);
	ctx->AddAccessControlEntry(mump, se::Object{ "command.voice_use2dAudio" }, se::AccessType::Allow);
	ctx->AddAccessControlEntry(mump, se::Object{ "command.voice_useNativeAudio" }, se::AccessType::Allow);

	// Enable debug messages
	console::GetDefaultContext()->ExecuteSingleCommand("developer 1");

	//console::GetDefaultContext()->ExecuteSingleCommand("voice_inBitrate 48000");
	//console::GetDefaultContext()->ExecuteSingleCommand("voice_useNativeAudio true");

	// Set position hook
	client->SetPositionHook([this](const std::string& name) {
		positionHandler(currentPosition);
		return std::optional<std::array<float, 3>>(currentPosition);
	});

	// Initialize
	connected = false;
	currentChannel = "";
	client->Initialize();
}

Mumble::~Mumble()
{
	delete client;
}

std::vector<AudioDeviceInfo> Mumble::GetInputDevices()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	std::vector<AudioDevice> devices;
	ListDevices(true, devices);
	std::vector<AudioDeviceInfo> result;
	for (auto& device : devices) result.push_back({ device.name, device.guid });
	return result;
}

std::vector<AudioDeviceInfo> Mumble::GetOutputDevices()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	std::vector<AudioDevice> devices;
	ListDevices(false, devices);
	std::vector<AudioDeviceInfo> result;
	for (auto& device : devices) result.push_back({ device.name, device.guid });
	return result;
}

void Mumble::ConnectAsync(const std::string& address, const std::string& username, const std::string& password, int port)
{
	auto remote = net::PeerAddress::FromString(address, port, net::PeerAddress::LookupType::ResolveName);
	if (!remote.has_value())
	{
		HandleConnectionCallback(MumbleConnectionEventType::ConnectionError,
			"DNS lookup failed");
		return;
	}
	client->ConnectAsync(remote.get(), username, password).then([this](concurrency::task<MumbleConnectionInfo*> task)
		{
			try
			{
				auto info = task.get();
				if (info->isConnected)
				{
					HandleConnectionCallback(MumbleConnectionEventType::Connected, 
						"Connected to " + info->address.ToString() + " as user " + info->username);
				}
				else
				{
					HandleConnectionCallback(MumbleConnectionEventType::ConnectionError, 
						"Did not connect");
				}
			}
			catch (std::exception& e)
			{
				HandleConnectionCallback(MumbleConnectionEventType::ConnectionError, e.what());
			}	
		});
}

void Mumble::DisconnectAsync()
{
	client->DisconnectAsync().then([this](concurrency::task<void> task)
		{
			currentChannel = "";
			HandleConnectionCallback(MumbleConnectionEventType::Disconnected, "Disconnected");
		});
}

void Mumble::SetInputDevice(const std::string& guid)
{
	client->SetInputDevice(guid);
	client->EnableAudioInput();
}

void Mumble::SetOutputDevice(const std::string& guid)
{
	client->SetOutputDevice(guid);
}

void Mumble::RunFrame()
{
	client->RunFrame();
}

void Mumble::SetChannel(const std::string& channel)
{
	if(currentChannel != "") client->RemoveListenChannel(currentChannel);

	client->SetChannel(channel);
	client->AddListenChannel(channel);

	currentChannel = channel;
}

bool Mumble::SendTextMessage(const std::string& message)
{
	if (currentChannel != "")
	{
		// Find channel id
		std::wstring wname = ToWide(currentChannel);
		for (const auto& channel : client->GetState().GetChannels())
		{
			if (channel.second.GetName() == wname)
			{
				// Assemble message packet
				MumbleProto::TextMessage msg;
				msg.set_message(message);
				msg.add_channel_id(channel.first);
				client->Send(MumbleMessageType::TextMessage, msg);

				return true;
			}
		}
	}
	return false;
}

void Mumble::HandleConnectionCallback(MumbleConnectionEventType type, const std::string& info)
{
	switch (type)
	{
	case MumbleConnectionEventType::Connected:
		connected = true;
		break;
	case MumbleConnectionEventType::ConnectionError:
	case MumbleConnectionEventType::Disconnected:
		connected = false;
		break;
	}
	connectionHandler(type, info);
}
