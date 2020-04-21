#ifndef _NodeEntry_H
#define _NodeEntry_H

struct node_entry
{
    unsigned long finish_key;
    unsigned int vpn;
};

void PrintValuePair(struct node_entry N);

#endif /* _NodeEntry_H */