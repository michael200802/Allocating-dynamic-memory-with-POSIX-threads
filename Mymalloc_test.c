//#include <stdlib.h>
#include "Mymalloc.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef struct _tree_t
{
    int x;
    struct _tree_t * left, * right, ** dad;
}tree_t;

tree_t * ABB_newnode(int x, tree_t ** dad)
{
    tree_t * newnode = (tree_t*)calloc(1,sizeof(tree_t));
    newnode->dad = dad;
    newnode->x = x;
    return newnode;
}

void ABB_add_node(tree_t ** root, int x)
{
    if(*root)
    {
        tree_t * ptr_to_node = *root;
        while(ptr_to_node->x != x)
        {
            if(ptr_to_node->x > x)
            {
                if(ptr_to_node->left)
                {
                    ptr_to_node = ptr_to_node->left;
                }
                else
                {
                    ptr_to_node->left = ABB_newnode(x,&ptr_to_node->left);
                }
            }
            else
            {
                if(ptr_to_node->right)
                {
                    ptr_to_node = ptr_to_node->right;
                }
                else
                {
                    ptr_to_node->right = ABB_newnode(x,&ptr_to_node->right);
                }
            }
        }
    }
    else
    {
        *root = ABB_newnode(x,NULL); 
    }
}

void ABB_show(tree_t * root)
{
    if(root == NULL)
    {
        return;
    }

    static size_t level;
    if(root->dad == NULL)
    {
        level = 0;
    }
    level++;

    size_t aux = level;
    ABB_show(root->right);
    level = aux;

    for (size_t i = 1; i < level; i++)
    {
        printf("   ");
    }
    printf("%d\n",root->x);
    ABB_show(root->left);
}

void ABB_recorrido_inOrden(tree_t * root)
{
    if(root)
    {
        ABB_recorrido_inOrden(root->left);
        printf("%d | ",root->x);
        ABB_recorrido_inOrden(root->right);

        if(root->dad == NULL)
        {
            putchar(10);
        }
    }
}

void ABB_prune(tree_t * root)
{
    if(root == NULL)
    {
        return;
    }

    static bool isroot = true;
    static tree_t * first_node;

    if(isroot)
    {
        first_node = root;
        isroot = false;
    }

    ABB_prune(root->left);

    if(first_node == root)
    {
        if(root->dad)
        {
            *root->dad = NULL;
        }
    }
    free(root);

    ABB_prune(root->right);

    if(first_node == root)
    {
        isroot = true;
    }
}

tree_t * ABB_search_node(tree_t * root, int value)
{
    while (root != NULL && root->x != value)
    {
        if(root->x > value)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return root;
}

enum command_type {ADD,DEL,SHOW,SEARCH,EXIT,INVALID};

int getcommand(enum command_type * command_type)
{
    static char buffer[100] = {0};
    if(buffer[0] != 0)
    {
        scanf("\n");
    }
    scanf("%[A-Z]",buffer);
    strtok(buffer," ");
    if(strcmp(buffer,"ADD") == 0)
    {
        *command_type = ADD;
    }
    else if(strcmp(buffer,"DEL") == 0)
    {
        *command_type = DEL;
    }
    else if(strcmp(buffer,"SEARCH") == 0)
    {
        *command_type = SEARCH;
    }
    else
    {
        if(strcmp(buffer,"SHOW") == 0)
        {
            *command_type = SHOW;
        } 
        else if(strcmp(buffer,"EXIT") == 0)
        {
            *command_type = EXIT;
        }
        else
        {
            *command_type = INVALID;
        }
        return 0;
    }
    int add;
    scanf("%d",&add);
    return add;
}

int main()
{

    enum command_type command_type;
    tree_t * tree = NULL;
    int arg;

    do
    {
        arg = getcommand(&command_type);
        switch (command_type)
        {
        case ADD:
            ABB_add_node(&tree,arg);
            break;

        case DEL:
            if(tree->x == arg)
            {
                ABB_prune(tree);
                tree = NULL;
            }
            else
            {
                tree_t * node = ABB_search_node(tree,arg);
                if(node)
                {
                    ABB_prune(node);
                }
                else
                {
                    puts("Value not found");
                }
            }
            break;
        
        case SHOW:
            ABB_show(tree);
            break;
        
        case SEARCH:
            {
                tree_t * node = ABB_search_node(tree,arg);
                if(node)
                {
                    puts("Value found");
                }
                else
                {
                    puts("Value not found");
                }  
            }   
            break;

        case INVALID:
            puts("Invalid command");
            break;

        case EXIT:
            puts("Bye");
            return 0;
        }
    } while (command_type != EXIT);
    return 0;
}