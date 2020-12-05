#pragma once

#include "StdInc.h"

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <wrl.h>
#include <vector>

#include <audiopolicy.h>

namespace WRL = Microsoft::WRL;

struct AudioDevice
{
	std::string guid;
	std::string name;
	WRL::ComPtr<IMMDevice> device;
};

static HRESULT GetStringProp(IPropertyStore* bag, PROPERTYKEY key, std::string* out);

WRL::ComPtr<IMMDevice> GetMMDeviceFromGUID(bool input, const std::string& guid);

void DuckingOptOut(WRL::ComPtr<IMMDevice> device);

void ListDevices(bool input, std::vector<AudioDevice>& store);