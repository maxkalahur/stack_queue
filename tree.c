#include <stdio.h>
#include <stdlib.h>
#include "queue.c"  // queue int
#include "queueS.c" // queue struct
#include "stack.c"  // stack struct

/*
	BST = Binary Search Tree -> left and right BST 
	if new->key < ROOT->key ====> left BST
	else 					====> right BST
*/

typedef struct treeNode {
	struct treeNode * left;
	int key;
	struct treeNode * right;
} tree;

tree* addNode(tree*, int);
tree * createTreeNode(int);
void freeTree(tree**);

void levelorder(tree*);
void preorder(tree*); 	// P->L->R
void inorder(tree*); 	// L->P->R
void postorder(tree*); 	// L->R->P

int main(int argc, char const *argv[]) {
	tree * root = NULL;
	root = addNode(root, 42);
	root = addNode(root, 11);
	root = addNode(root, 64);
	root = addNode(root, 7);
	root = addNode(root, 15);
	root = addNode(root, 55);
	root = addNode(root, 99);
	root = addNode(root, 3);
	root = addNode(root, 100);
	printf("======= LEVELORDER ========\n");
	// levelorder(root); 	// 
	printf("======= PREORDER ========\n");
	// preorder(root); 	// P->L->R
	printf("=========================\n");
	printf("======= INORDER =========\n");
	inorder(root); 	// L->P->R
	printf("=========================\n");
	printf("======= POSTORDER =======\n");
	postorder(root); 	// L->R->P
	printf("=========================\n");
	
	freeTree(&root);
	
	printf("======= PREORDER ========\n");
	preorder(root); 	// P->L->R
	printf("=========================\n");
	printf("======= INORDER =========\n");
	inorder(root); 	// L->P->R
	printf("=========================\n");
	printf("======= POSTORDER =======\n");
	postorder(root); 	// L->R->P
	printf("=========================\n");
	return 0;
}

tree* addNode(tree* root, int key) {
	if (root == NULL) {
		root = createTreeNode(key);
	} else {
		if (key < root->key) {
			root->left = addNode(root->left, key);
		} else {
			root->right = addNode(root->right, key);
		}
	}
	return root;
}

tree * createTreeNode(int key) {
	tree * newTreeNode = (tree*) malloc (sizeof(tree));
	if (newTreeNode == NULL) {
		printf("out of memory :(\n");
		exit(1);
	}
	newTreeNode->key = key;
	newTreeNode->left = NULL;
	newTreeNode->right = NULL;
	return newTreeNode;
}

void levelorder(tree* root) { // 42 11 64 7 15 55 99 3 100

    if (root != NULL) {
		
        queueSStruct * levelQueue = queueSInit();
        queueStruct * queue = queueInit();

        int k;
        tree* tmpNode;
        enqueueS(root, levelQueue);

        for( register int i = 0 ; ; i++ ) {

            if(i > 10000) {
                printf("Loop error\n");
                break;
            }

            queueSStruct * nextLevelStack = queueSInit();
            int hasAny = 0;

            while( tmpNode = dequeueS(levelQueue) ) {

                hasAny = 1;
                printf("%d\n", tmpNode->key);

                enqueue(tmpNode->key, queue);

                if( tmpNode->left ) {
                    enqueueS(tmpNode->left, nextLevelStack);
                }
                if( tmpNode->right ) {
                    enqueueS(tmpNode->right, nextLevelStack);
                }
            }

            levelQueue = nextLevelStack;
            
            if( !hasAny ) {
                printf("Tree is ended.\n");
                while( k = dequeue(queue) ) {
                    if( k == -1 ) { break; }
                    printf("%d\n", k);
                }
                break;
            }

        }
	}

}

void preorder(tree* root) { // 42,11,64,7,15,55,99,3,100

    if (root != NULL) {
		
        stackStruct * stack;
        stack = stackInit();

        queueStruct * queue;
        queue = queueInit();

        tree* currNode = root;
        tree* prevNode;
        int go_back = 0, left_back = 0, right_back = 0, stop_left = 0;

        for( register int i = 0 ; ; i++ ) {

            if(i > 10000) {
                printf("Loop error\n");
                break;
            }

            if( go_back ) {

                prevNode = currNode;

                currNode = pop(stack);
                printf("Level Back: %d\n",currNode->key);

                if( currNode == root && ((currNode->right && prevNode->key > currNode->key) || !currNode->right)  ) {
                    printf("Tree is ended\n");

                    for( register int ii = 0; ii <= i; ii++ ) {
                        printf("%d\n", dequeue(queue));
                    }

                    break;
                }

                if( ( left_back && currNode->right ) || ( right_back && (prevNode->left && prevNode->right) && currNode->right )) {
                    push(currNode, stack);
                    go_back = left_back = right_back = 0;
                }
                else {
                    continue;
                }
            }

            if(i == 0) {
                push(currNode, stack);
                enqueue(currNode->key, queue);

                printf("Root: %d\n", currNode->key);
            }
            else if(currNode->left && !stop_left) {

                currNode = currNode->left;

                enqueue(currNode->key, queue);

                if( currNode->left || currNode->right ) {
                    push(currNode, stack);
                }
                else {
                    go_back = stop_left = left_back = 1;
                }

                printf("Left: %d\n",currNode->key);
            }
            else if(currNode->right) {

                currNode = currNode->right;

                enqueue(currNode->key, queue);

                if( currNode->left || currNode->right ) {
                    push(currNode, stack);
                    stop_left = 0;
                }
                else {
                    go_back = right_back = 1;
                }

                printf("Right: %d\n",currNode->key);
            }

        }
	}

}
/*		42 11 7 3 15 64 55 99 100
	preorder({key: 42}) 42 x
	preorder({key: 11}) 11 x
	preorder({key: 7}) 7 x
	preorder({key: 3}) 3 x
	preorder(NULL)
	preorder(NULL)
	preorder(NULL)
	preorder({key: 15}) 15 x
	preorder(NULL)
	preorder(NULL)
	preorder({key: 64}) 64 x
	preorder({key: 55}) 55 x
	preorder(NULL)
	preorder(NULL)
	preorder({key: 99}) 99 x
	preorder(NULL)
	preorder({key: 100}) 100 x
	preorder(NULL)
	preorder(NULL)
*/

void inorder(tree* root) { // 3 7 11 15 42 55 64 99 100

    if (root != NULL) {
		
        stackStruct * stack;
        stack = stackInit();

        queueStruct * queue;
        queue = queueInit();

        tree* currNode = root;
        tree* prevNode;
        int go_back = 0, left_back = 0, right_back = 0, stop_left = 0;

        for( register int i = 0 ; ; i++ ) {

            if(i > 10000) {
                printf("Loop error\n");
                break;
            }

            if( go_back ) {

                prevNode = currNode;

                currNode = pop(stack);
                printf("Level Back: %d\n",currNode->key);

                if( currNode == root && ((currNode->right && prevNode->key > currNode->key) || !currNode->right)  ) {
                    printf("Tree is ended\n");

                    for( register int ii = 0; ii <= i; ii++ ) {
                        printf("%d\n", dequeue(queue));
                    }

                    break;
                }

                if( ( left_back && currNode->right ) || ( right_back && (prevNode->left && prevNode->right) && currNode->right )) {
                    push(currNode, stack);
                    go_back = left_back = right_back = 0;
                }
                else {
                    continue;
                }
            }

            if(i == 0) {
                push(currNode, stack);
                enqueue(currNode->key, queue);

                printf("Root: %d\n", currNode->key);
            }
            else if(currNode->left && !stop_left) {

                currNode = currNode->left;

                enqueue(currNode->key, queue);

                if( currNode->left || currNode->right ) {
                    push(currNode, stack);
                }
                else {
                    go_back = stop_left = left_back = 1;
                }

                printf("Left: %d\n",currNode->key);
            }
            else if(currNode->right) {

                currNode = currNode->right;

                enqueue(currNode->key, queue);

                if( currNode->left || currNode->right ) {
                    push(currNode, stack);
                    stop_left = 0;
                }
                else {
                    go_back = right_back = 1;
                }

                printf("Right: %d\n",currNode->key);
            }

        }
	}


	// if (root != NULL) {
	// 	inorder(root->left);
	// 	printf("%d\n", root->key);
	// 	inorder(root->right);
	// }
}

void postorder(tree* root) {



	// if (root != NULL) {
	// 	postorder(root->left);
	// 	postorder(root->right);
	// 	printf("%d\n", root->key);
	// }
}

void freeTree(tree** root) {
	if (*root != NULL) {
		freeTree(&((*root)->left));
		freeTree(&((*root)->right));
		free(*root);
		*root = NULL;
	}
}