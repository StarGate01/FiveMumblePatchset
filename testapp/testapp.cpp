// testapp.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>

#include <StdInc.h>
#include <UvLoopHolder.h>
#include <MumbleClientImpl.h>

MumbleClient client;

int main()
{
    std::cout << "Mumble FiveM library test app!" << std::endl;


    auto remote = net::PeerAddress::FromString("chrz.de", 64738, net::PeerAddress::LookupType::ResolveName);
    if (!remote.has_value())
    {
        std::cerr << "Cannot resolve remote address!" << std::endl;
        return 1;
    }

    client.Initialize();
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

    return 0;
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
