#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *push(List *list, const void *data)
{
	Node *temp = (Node *)malloc(sizeof(Node *));
	temp->data = data;
	temp->next = NULL;

	if (list->head == NULL)
	{
		return list->head = temp;
	}

	Node *ptr;
	for (ptr = list->head; ptr->next != NULL; ptr = ptr->next);
	return ptr->next = temp;
}
