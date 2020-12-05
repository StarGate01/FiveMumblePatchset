/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#include "StdInc.h"
#include "MumbleClientImpl.h"
#include "MumbleMessageHandler.h"

DEFINE_HANDLER(Version)
{
	auto client = MumbleClient::GetCurrent();

	// also send our initial registration packet
	auto username = client->GetState().GetUsername();
	auto usernameUtf8 = ConvertToUTF8(username);

	auto password = client->GetState().GetPassword();
	auto passwordUtf8 = ConvertToUTF8(password);

	MumbleProto::Authenticate authenticate;
	authenticate.set_opus(true);
	authenticate.set_username(usernameUtf8);
	authenticate.set_password(passwordUtf8);

	client->Send(MumbleMessageType::Authenticate, authenticate);

	client->EnableAudioInput();
});
