#include<stdio.h>
#include<stdlib.h>
#include "ListHeader.h"

void insert_At_head(int number)
{
	node *newnode = (node *)malloc(sizeof(node));

	newnode->number = number;
	newnode->next = newnode;
	newnode->previous = newnode;

	if( head == NULL)
	{
		head = newnode;
		tail = newnode;
	}

	else
	{
		newnode->next = head;
		newnode->previous = tail;
		head->previous = newnode;
		tail->next = newnode;
		head = newnode;
	}
}

void insert_At_tail(int number)
{

	node *newnode = (node *)malloc(sizeof(node));
	newnode->number = number;
	newnode->next = newnode;
	newnode->previous = newnode;

	if(head == NULL)
	{
		head = newnode;
		tail = newnode;
	}

	else
	{
		tail->next = newnode;
		newnode->next = head;
		newnode->previous = tail;
		tail = newnode;
		head->previous = tail;
	}

}

void insert_At_middle(int number, int position)
{

	if(position == 1)
	{
		insert_At_head(number);
		return;
	}

	else if(position>1 && head!=NULL)
	{
		node *current = head;
		node *temp = (node *)malloc(sizeof(node));
		int count = 0;
		do
		{
			count++;
			temp = current;
			current = current->next;
		}while(current->next != head && count<position-1);

		if(count == position - 1)
		{
			if(temp == tail)
			{
				insert_At_tail(number);
			}

			else
			{
				node *newnode = (node *)malloc(sizeof(node));
				newnode->number = number;

				temp->next = newnode;
				newnode->next = current;
				newnode->previous = temp;
				current->previous = newnode;
			}

			return;
		}
	}
}

void delete_head()
{
	if(head == NULL)
		return;
	node *temp = head;

	tail->next = head->next;
	head = head->next;
	head->previous = tail;

	free(temp);
}

void delete_tail()
{
	if(head == NULL)
		return;

	node *temp = head;
	node *current = head;

	while(current->next != head)
	{
		temp = current;
		current = current->next;
	}

	temp->next = head;
	temp = tail;
	head->previous = tail;
	free(current);
}

void delete_middle(int position)
{
	if(head == NULL)
		return;

	if(position == 1)
	{
		delete_head();
		return;
	}

	node *current = head;
	node *temp;
	int count = 0;

	do
	{
		count++;
		temp = current;
		current = current->next;
	}while(current->next != head && count<position-1);

	if(count == position-1)
	{
		if(current == tail)
		{
			delete_tail();
			return;
		}

		temp->next = current->next;
		current->next->previous = temp;
		free(current);
		return;
	}
}

void printList()
{
	if(head == NULL)
		return;

	node *current = head;

	do
	{
		printf("%d",current->number);
		current = current->next;
	}while(current != head);
}