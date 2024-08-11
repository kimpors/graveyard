#pragma once

typedef struct node
{
	struct node *next;
	const void *data;
} Node;

typedef struct {
	Node *head;
} List;

Node *push(List *list, const void *data);
