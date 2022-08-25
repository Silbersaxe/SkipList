#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

#define MAX_LEVEL 6

int seeded = 0;

//DEFINITION: Node
typedef struct Node
    {
        int key;                
        int value;              
        struct Node **forward;
    } Node;

//DEFINITION: Skiplist
typedef struct Skiplist
    {
        int level;              
        int size;               
        struct Node *header;    
    } Skiplist;

//FUNKTION: Skiplist initialisieren
Skiplist S_Init(Skiplist *list)
    {
        Node *_header = (Node *) malloc(sizeof(struct Node)); 
        list -> header = _header;
        
        _header -> key = INT_MAX;
        _header -> forward = (Node **) malloc(sizeof(Node *) * (MAX_LEVEL + 1));
        
        for (int i = 0; i <= MAX_LEVEL; i++)
            {
                _header -> forward[i] = list -> header;
            }

        list -> level = 1;
        list -> size = 0;
    }

//FUNKTION: Level generiern
int random_lvl()
    {
        if (!seeded)
            {
                srand((unsigned int) time(NULL));
                seeded = 1;
            }
        
        int level = 1;

            while (random() < RAND_MAX / 2 && level < MAX_LEVEL)
                {
                    level++;
                }

        printf("%d\n", level);

        return level;        
    }

//FUNKTION: Node einfügen
int S_Insert(Skiplist *list, int key, int value)
    {
        Node *update[MAX_LEVEL + 1];
        Node *a = list -> header;

        int i, level;

        for (i = list -> level; i >= 1; i--)
            {
                while (a -> forward[i] -> key < key)
                    {
                        a = a -> forward[i];
                    }
                update[i] = a;
            }

        a = a -> forward[1];

        if (key == a -> key)
            {
                a -> value = value;
                return 0;
            }
        else
            {
                level = random_lvl();

                if (level > list -> level)
                    {
                        for (i = list -> level + 1; i <= level; i++)
                            {
                                update[i] = list -> header;
                            }

                        list -> level = level;
                    }

                a = (Node *) malloc(sizeof(Node));
                a -> key = key;
                a -> value = value;
                a -> forward = (Node **) malloc(sizeof(Node *) * (level + 1));

                for (i = 1; i <= level; i++)
                    {
                        a -> forward[i] = update[i] -> forward[i];
                        update[i] -> forward[i] = a;
                    }

                list -> size += 1;
            }

        return 0;            
    }

//FUNKTION: Node suchen
Node *S_Search(Skiplist *list, int key)
    {
        Node *a = list -> header;

        int i;

        for (i = list -> level; i >= 1; i--)
        {
            while (a -> forward[i] -> key < key)
            {
                a = a -> forward[i];
            }
        }

        if (a -> forward[1] -> key == key)
            {
                return a -> forward[1];
            }
        else    
            {
                return NULL;
            }
        
        return NULL;
    }

//FUNKTION: Node löschen
int S_Delete(Skiplist *list, int key)
    {
        Node *update[MAX_LEVEL + 1];
        Node *a = list -> header;

        int i;

        for (i = list -> level; i >= 1; i--)
            {
                while (a -> forward[i] -> key < key)
                    {
                        a = a -> forward[i];
                    }

                update[i] = a;
            }

        a = a -> forward[1];

        if (a -> key == key)
            {
                for (i = 1; i <= list -> level; i++)
                    {
                        if (update[i] -> forward[i] != a)
                            {
                                break;
                            }

                        update[i] -> forward[1] = a -> forward[i];
                    }

                if (a)
                    {   
                        free(a -> forward);
                        free(a);
                    }

                while (list -> level > 1 && 
                       list -> header -> forward[list -> level] == list -> header)
                    {
                        list -> level--;
                    }

                list -> size -= 1;

                return 0;
            }    

        return 1;    
    }

//FUNKTION: Liste ausgeben
void S_Dump(Skiplist *list)
    {
        Node *a = list -> header;

        while (a && a -> forward[1] != list -> header)
            {
                a = a -> forward[1];
                printf("Key: %d | Value: %d\n", a  -> key, a-> value);
            }

        printf("NIL\n");
    }

//MAIN
int main(int args[])
    {
        Skiplist L;

        S_Init(&L);
     
        S_Insert(&L,5,3);
        S_Insert(&L,6,7);
        S_Insert(&L,3,24);
        S_Insert(&L,9,5);
        S_Insert(&L,1,78);
        S_Insert(&L,4,1);
        S_Insert(&L,2,12);
        S_Insert(&L,8,4);
        S_Insert(&L,7,56);
        
        printf("Size: %d\n", L.size);

        S_Dump(&L);
    }