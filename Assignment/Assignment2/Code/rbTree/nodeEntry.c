#include "nodeEntry.h"
#include <stdio.h>

void PrintValuePair(struct node_entry N)
{
    printf("Finish Key: %ld\n", N.finish_key);
    printf("VPN: %d\n", N.vpn);
}