#include <sys/ipc.h>
#include <sys/shm.h>

#include "Config.h"

void initAppli();
void detruireAppli();

int main()
{
	initAppli();
	sleep(10);
	detruireAppli();
	return 0;
}

void initAppli()
{

}

void detruireAppli()
{

}
