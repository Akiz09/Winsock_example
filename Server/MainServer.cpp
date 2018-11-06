#include "Socket.h"

#pragma comment(lib,"ws2_32.lib")
#define DEFAULT_PORT "27015"

using namespace std;
using namespace soc;

void main(int argc, char *argv[]) {

		Socket Server;
		Server.InitializeSock();
		Server.Create();
		Server.Bind();
		Server.Listen();
		Server.Recive();
}
