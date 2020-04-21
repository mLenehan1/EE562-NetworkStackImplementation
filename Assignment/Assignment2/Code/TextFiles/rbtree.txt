#include "rbtree.h"
#include <stdio.h>
#include "fatal.h"
#include "nodeEntry.h"

typedef enum ColorType
{
    Red,
    Black
} ColorType;

struct RedBlackNode
{
    ElementType Element;
    RedBlackTree Left;
    RedBlackTree Right;
    ColorType Color;
};

static Position NullNode = NULL; /* Needs initialization */

/* Initialization procedure */
RedBlackTree
Initialize(void)
{
    RedBlackTree T;

    if (NullNode == NULL)
    {
        NullNode = malloc(sizeof(struct RedBlackNode));
        if (NullNode == NULL)
            fatalerror("Out of space!!!");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
        NullNode->Element.finish_key = 12345;
        NullNode->Element.vpn = 12345;
    }

    /* Create the header node */
    T = malloc(sizeof(struct RedBlackNode));
    if (T == NULL)
        fatalerror("Out of space!!!");
    T->Element.finish_key = NegInfinity;
    T->Element.vpn = NegInfinity;
    T->Left = T->Right = NullNode;
    T->Color = Black;

    return T;
}

/* END */

void Output(ElementType Element)
{
    printf("Finish Key: %ld \t VPN: %d\n", Element.finish_key, Element.vpn);
}

/* Print the tree, watch out for NullNode, */

/* and skip header */

static void
DoPrint(RedBlackTree T)
{
    if (T != NullNode)
    {
        DoPrint(T->Left);
        Output(T->Element);
        DoPrint(T->Right);
    }
    else printf("test\n");
}

void PrintTree(RedBlackTree T)
{
    DoPrint(T->Right);
}

/* END */

static RedBlackTree
MakeEmptyRec(RedBlackTree T)
{
    if (T != NullNode)
    {
        MakeEmptyRec(T->Left);
        MakeEmptyRec(T->Right);
        free(T);
    }
    return NullNode;
}

RedBlackTree
MakeEmpty(RedBlackTree T)
{
    T->Right = MakeEmptyRec(T->Right);
    return T;
}

Position
Find(ElementType X, RedBlackTree T)
{
    if (T == NullNode)
        return NullNode;
    if (X.finish_key < T->Element.finish_key)
        return Find(X, T->Left);
    else if (X.finish_key > T->Element.finish_key)
        return Find(X, T->Right);
    else
        return T;
}

Position
FindMin(RedBlackTree T)
{
    T = T->Right;
    while (T->Left != NullNode)
        T = T->Left;

    return T;
}

Position
FindMax(RedBlackTree T)
{
    while (T->Right != NullNode)
        T = T->Right;

    return T;
}

/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */

/* Update heights, then return new root */

static Position
SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */

/* Update heights, then return new root */

static Position
SingleRotateWithRight(Position K1)
{
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    return K2; /* New root */
}

/* Perform a rotation at node X */
/* (whose parent is passed as a parameter) */

/* The child is deduced by examining Item */

static Position
Rotate(ElementType Item, Position Parent)
{

    if (Item.finish_key < Parent->Element.finish_key)
        return Parent->Left = Item.finish_key < Parent->Left->Element.finish_key ? SingleRotateWithLeft(Parent->Left) : SingleRotateWithRight(Parent->Left);
    else
        return Parent->Right = Item.finish_key < Parent->Right->Element.finish_key ? SingleRotateWithLeft(Parent->Right) : SingleRotateWithRight(Parent->Right);
}

static Position X, P, GP, GGP;

static void HandleReorient(ElementType Item, RedBlackTree T)
{
    X->Color = Red; /* Do the color flip */
    X->Left->Color = Black;
    X->Right->Color = Black;

    if (P->Color == Red) /* Have to rotate */
    {
        GP->Color = Red;
        if ((Item.finish_key < GP->Element.finish_key) != (Item.finish_key < P->Element.finish_key))
            P = Rotate(Item, GP); /* Start double rotate */
        X = Rotate(Item, GGP);
        X->Color = Black;
    }
    T->Right->Color = Black; /* Make root black */
}

RedBlackTree
Insert(ElementType Item, RedBlackTree T)
{
    X = P = GP = T;
    NullNode->Element.finish_key = Item.finish_key;
    NullNode->Element.vpn = Item.vpn;
    while (X->Element.finish_key != Item.finish_key) /* Descend down the tree */
    {
        GGP = GP;
        GP = P;
        P = X;
        if (Item.finish_key < X->Element.finish_key)
            X = X->Left;
        else
            X = X->Right;
        if (X->Left->Color == Red && X->Right->Color == Red)
            HandleReorient(Item, T);
    }

    if (X != NullNode)
        return NullNode; /* Duplicate */

    X = malloc(sizeof(struct RedBlackNode));
    if (X == NULL)
        fatalerror("Out of space!!!");
    X->Element.finish_key = Item.finish_key;
    X->Element.vpn = Item.vpn;
    X->Left = X->Right = NullNode;

    if (Item.finish_key < P->Element.finish_key) /* Attach to its parent */
    {
        P->Left = X;
        printf("Left %ld\n", P->Left->Element.finish_key);
    }
    else
    {
        P->Right = X;
        printf("Right %ld\n", P->Right->Element.finish_key);
    }
    HandleReorient(Item, T); /* Color it red; maybe rotate */

    return T;
}

RedBlackTree
Remove(ElementType Item, RedBlackTree T)
{
    printf("Remove is unimplemented\n");
    if (Item.finish_key)
        return T;
    return T;
}

long Retrieve(Position P)
{
    return P->Element.finish_key;
}