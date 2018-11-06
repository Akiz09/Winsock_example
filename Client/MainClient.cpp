#include "clientSocket.h"

#define DEFAULT_PORT "27015"

using namespace std;
using namespace cli;

int	main(int argc, char **argv) {
	clientSocket Client;
	Client.InitializeClient();
	Client.Create(argc, argv);
	Client.Connect();
	Client.Send();
}