#include <iostream>
#include "../stratum_miner/stratum_client.h"

int main(int argc, char* argv[])
{
	try
	{
		stratum::stratum_client sc("mine.moneropool.com", "3336", 
			"41ttJiGt5GH3frtXYKUEW6KuJoqwbeHaPTgTxCND3gATaJiQps6DhWcF2k2d1ZWUMrJ6qNg47VSSpLhrSFB4KLmjLLQ5sbU", 
			"x");
		system("pause");
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}
