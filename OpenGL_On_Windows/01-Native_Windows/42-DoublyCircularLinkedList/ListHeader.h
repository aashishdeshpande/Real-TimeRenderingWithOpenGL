struct linked_list{

	int number;
	struct linked_list *next;
	struct linked_list *previous;
};

typedef struct linked_list node;

node *head = NULL;
node *tail = NULL;

void insert_At_head(int number);
void insert_At_tail(int number);
void insert_At_middle(int number, int position);
void delete_head();
void delete_tail();
void delete_middle(int position);
void printList();
