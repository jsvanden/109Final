#include <stdlib.h>

#include "Server.hpp"
#include "Utility.hpp"

using namespace utility;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	try
	{
		Server socket(atoi(argv[1]));

		while (true)
			socket.Update();
	}
	catch (SRIException s)
	{
		printf("%s\n", s.what());
		exit(1);
	}

	return 0;
}