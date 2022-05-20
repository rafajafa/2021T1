// example of recursion
// 2021 april

struct node *head {
    struct node *next;
    int value;
};
#define NO_MAXIMUM -1
// Find the maximum value in the linked list
int find_max(struct node *head) {
     if (head == NULL) {
        return NO_MAXIMUM;
     }
     int maximum_so_far = find_max(head->next);
     if (maximum_so_far < head->value) {
         // we have found a new node that's bigger
         maximum_so_far = head->value;
     }
     return maximum_so_far;
}
