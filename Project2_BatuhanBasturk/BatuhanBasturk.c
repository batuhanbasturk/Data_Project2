/* Name Surname: Batuhan Baştürk
 * School id: 150119035
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node{
    int entry;
    struct Node *left;
    struct Node *right;
    int height;
}typedef Node;

//Function to get the height of the tree
int get_height(Node *tree_node){
    if (tree_node == NULL)
        return 0;
    return tree_node->height;
}

// Function to get maximum of two integers
int maximum(int a, int b){
    if (a > b){
        return a;
    }
    else{
        return b;
    }
}

Node* find_node(Node *tree_node,int entry, int depth){
    //if tree is null or entry is equal to node than assign its depth and return node.
    if(tree_node == NULL || tree_node->entry == entry){
        tree_node->height = depth;
        return tree_node;

    }
    //if entry is bigger than current node go to node's right to find and increase depth 1
    if(tree_node->entry < entry){
        return find_node(tree_node->right, entry, ++depth);
    }
        //else part is if entry is less than key go to node's left to find and increase depth 1
    else if (tree_node->entry > entry) {
        return find_node(tree_node->left, entry, ++depth);
    }

}
//find out which kth element is it in that depth
int kth_element_node(Node *tree_node, int current_depth, Node* found_node){
    if(tree_node == NULL){
        return 0;
    }
    if(current_depth == found_node->height && tree_node -> entry <= found_node -> entry){
        return 1;
    }
    else{
        current_depth += 1;
        return kth_element_node(tree_node -> left, current_depth, found_node) + kth_element_node(tree_node -> right, current_depth, found_node);
    }

}

//Allocates a new node with the given key and NULL left and right pointerS
Node* newNode(int entry){
    Node* tree_node = (Node*) malloc(sizeof(Node));
    tree_node->entry = entry;
    tree_node->left   = NULL;
    tree_node->right  = NULL;
    tree_node->height = 1;  // new node is initially added at leaf
    return(tree_node);
}

//function to right rotate subtree rooted with y
Node *rightRotate_AVL(Node *j){
    Node *i = j->left;
    Node *x = i->right;

    // Perform rotation
    i->right = j;
    j->left = x;

    // Update heights
    j->height = maximum(get_height(j->left), get_height(j->right))+1;
    i->height = maximum(get_height(i->left), get_height(i->right))+1;

    // Return the root
    return i;
}


Node *leftRotate_AVL(Node *i){
    Node *j = i->right;
    Node *x = j->left;

    j->left = i;
    i->right = x;

    i->height = maximum(get_height(i->left), get_height(i->right)) + 1;
    j->height = maximum(get_height(j->left), get_height(j->right)) + 1;

    return j;
}


int get_Balance(struct Node *N){
    if (N == NULL)
        return 0;
    return get_height(N->left) - get_height(N->right);
}

Node* BST_insert(Node* tree_node, int entry)
{
    // If the tree is empty, return a new node
    if (tree_node == NULL)
        return newNode(entry);

    //If it's not empty recur down
    if (entry < tree_node->entry)
        tree_node->left = BST_insert(tree_node->left, entry);
    else if (entry > tree_node->entry)
        tree_node->right = BST_insert(tree_node->right, entry);
    else{
        printf("You should use non-duplicated keys!");
        exit(0);
    }

    // return the node pointer
    return tree_node;
}


Node* AVL_insert(Node* tree_node, int entry){
    //BST insert part
    if (tree_node == NULL)
        return(newNode(entry));

    if (entry < tree_node->entry)
        tree_node->left  = AVL_insert(tree_node->left, entry);
    else if (entry > tree_node->entry)
        tree_node->right = AVL_insert(tree_node->right, entry);
    else {// Same entries are not allowed in BST
        printf("You should use non-duplicated keys!");
        exit(0);
    }
    //update height
    tree_node->height = 1 + maximum(get_height(tree_node->left), get_height(tree_node->right));

    // check if unbalanced
    int balance = get_Balance(tree_node);

    // If this node becomes unbalanced, then
    // there are 4 cases

        // Left Left Case
    if (balance > 1 && entry < tree_node->left->entry)
        return rightRotate_AVL(tree_node);

        // Right Right Case
    if (balance < -1 && entry > tree_node->right->entry)
        return leftRotate_AVL(tree_node);

        // Left Right Case
    if (balance > 1 && entry > tree_node->left->entry) {
        tree_node->left = leftRotate_AVL(tree_node->left);
        return rightRotate_AVL(tree_node);
    }
        // Right Left Case
    if (balance < -1 && entry < tree_node->right->entry) {
        tree_node->right = rightRotate_AVL(tree_node->right);
        return leftRotate_AVL(tree_node);
    }
    /* return the node pointer */
    return tree_node;
}

int main(){
    Node *root = NULL;

    //initialize array with size of 1028
    int numberArray[1028];

    int input_node_count = 0;

    int i, j, temp;

    FILE *input_File;
    if((input_File = fopen("input.txt", "r")) == NULL){
        printf("File open error!");
        exit(0);
    }
    //read until file is finished and count how many numbers in it
    while(!feof(input_File)){
        fscanf(input_File, "%d", &numberArray[input_node_count]);
        input_node_count++;
    }

    //get numbers into array and if input count is less than 16 exit program
    if(input_node_count < 16) {
        printf("You need at least 16 input to create a tree!");
        exit(0);
    }
    else {
        //get file inputs into array
        for (i = 0; i < input_node_count; i++) {
            fscanf(input_File, "%d", &numberArray[i]);
            //if there is any non-negative value end program and print that you need to have positive inputs
            if(numberArray[i] < 0){
                printf("You need to have non-negative input!");
                exit(0);
            }
        }
    }
    fclose(input_File);

    //log_equation that i discovered while trying to find algorithm in later use also part of current depth level formula
    int log_equation = floor(log10(input_node_count)/log10(4));
    int depth_level_BST = 3 * log_equation;
    //waypoints that 16,64,256,...
    int waypoint = pow(4, log_equation);

    //sorting array descending order
    for (i = 0; i < input_node_count; ++i){
        for (j = i + 1; j < input_node_count; ++j){
            if (numberArray[i] < numberArray[j]){
                temp = numberArray[i];
                numberArray[i] = numberArray[j];
                numberArray[j] = temp;
            }
        }
    }

    /* log4(n) is floor
     * I discovered while entry input number is larger than 4^log4(n) - log4(n) and smaller than 4^log4(n) - (log4(n) - 1)
     * until minus log4(n) it is avl tree afterwards i need to insert last log4(n) element as a bst
     * For example: When entry input is 17 4^log4(17) - log4(17) = 16-2= 17 >= 14 and 4^log4(17) + (log4(17) -1) = 16+1 17<=17
     * so i need to use 15 input for avl tree to make balanced after that i need to complete my depth level to 6 currently I'm at 4th depth level
     * i insert as a bst remaining inputs
     * these ones are special cases or minority
     * input counts that are in this cases (16 17) (64 65 66) (256 257 258 259) ...
     *
     *
     *else part is if my node count is not in the range of upward algorithm than that means that i inserted node and my
     * depth level is +1 than before
     * For example my input count is 18 in current input i can create balanced avl tree with 15 full of depth level 4
     * 16th node is already in 5th depth level that means that i need to insert (log4(18) - 1) = 1 element as a bst
     * 18 - (log4(18) - 1) as an avl tree
     */
    if((input_node_count >= waypoint - log_equation) && (input_node_count <= waypoint + (log_equation - 1))){
        for(i = 0; i < input_node_count - log_equation;i++){
            root = AVL_insert(root, numberArray[i]);
        }
        for(i = (input_node_count - log_equation);i < input_node_count;i++){
            root = BST_insert(root, numberArray[i]);
        }
    }
    else{
        for(i = 0; i < (input_node_count - (log_equation - 1));i++){
            root = AVL_insert(root, numberArray[i]);
        }
        for(i = input_node_count - (log_equation - 1); i < input_node_count;i++){
            root = BST_insert(root, numberArray[i]);
        }

    }

    printf("Depth level of BST is %d", depth_level_BST);

    int key_to_find = -1;

    while(key_to_find != 0) {

        printf("\nKey value to be searched (Enter 0 to exit):");
        scanf("%d", &key_to_find);

        if(key_to_find == 0){
            printf("Exit");
            exit(0);
        }
        Node* wanted_node = find_node(root,key_to_find,0);
        int kth_element = kth_element_node(root, 0, wanted_node);
        printf("At depth level %d, ", wanted_node->height);
        printf("%dth element", kth_element);

    }

    return 0;
}
