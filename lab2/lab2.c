#include <stdio.h>
#include <stdlib.h>

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "Assertion failed: expected %d, actual %d\n", (expected), (actual)); \
            abort(); \
        } \
    } while (0)

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
    struct Node *parent; 
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

Node* findNode(Node* root, int data) {
    Node* current = root;
    while (current != NULL) {
        if (data == current->data) {
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

void insertNode(Node** root, int data) {
    Node* newNode = createNode(data);
    if (*root == NULL) {
        *root = newNode;
        return;
    }

    Node* current = *root;
    Node* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;
    if (data < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}

Node* joinTrees(Node* root1, Node* root2) {
    if (root1 == NULL) return root2;
    if (root2 == NULL) return root1;

    Node* current = root2;
    while (current->left != NULL) {
        current = current->left;
    }

    current->left = root1;
    root1->parent = current;
    return root2;
}

void splitTree(Node* node, Node** leftTree, Node** rightTree) {
    *leftTree = node->left;
    if (*leftTree != NULL) {
        (*leftTree)->parent = NULL;
    }

    *rightTree = node->right;
    if (*rightTree != NULL) {
        (*rightTree)->parent = NULL;
    }

    node->left = NULL;
    node->right = NULL;
}

void printTree(Node* root, int level) {
    if (root == NULL) return;
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("  ");
    printf("%d\n", root->data);
    printTree(root->left, level + 1);
}
void test_findNode_exists() {
    printf("test_findNodes_exists()\n");
    Node* root = NULL;
    insertNode(&root, 5);
    insertNode(&root, 3);
    insertNode(&root, 8);

    Node* foundNode = findNode(root, 3);
    ASSERT_EQ(foundNode->data, 3);
}

void test_findNode_notExists() {
    Node* root = NULL;
    printf("test_findNodes_notExists()\n");
    insertNode(&root, 5);
    insertNode(&root, 3);
    insertNode(&root, 8);

    Node* foundNode = findNode(root, 10);
    ASSERT_EQ(foundNode, NULL);
}

void test_insertNode_emptyTree() {
    printf("test_insertNode_emptyTree()\n");
    Node* root = NULL;
    insertNode(&root, 5);
    ASSERT_EQ(root->data, 5);
    ASSERT_EQ(root->parent, NULL);
}

void test_insertNode_leftChild() {
    printf("test_insertNode_leftChild()\n");
    Node* root = createNode(5);
    insertNode(&root, 3);
    ASSERT_EQ(root->left->data, 3);
    ASSERT_EQ(root->left->parent, root);
}

void test_insertNode_rightChild() {
    printf("test_insertNode_rightChild()\n");
    Node* root = createNode(5);
    insertNode(&root, 8);
    ASSERT_EQ(root->right->data, 8);
    ASSERT_EQ(root->right->parent, root);
}

void test_joinTrees_emptyTrees() {
    printf("test_joinTrees_emptyTrees()\n");
    Node* root1 = NULL;
    Node* root2 = NULL;
    Node* joined = joinTrees(root1, root2);
    ASSERT_EQ(joined, NULL);
}

void test_joinTrees_oneEmptyTree() {
    printf("test_joinTrees_oneEmptyTree()\n");
    Node* root1 = createNode(5);
    Node* root2 = NULL;
    Node* joined = joinTrees(root1, root2);
    ASSERT_EQ(joined, root1);
}

void test_joinTrees_twoTrees() {
    printf("test_joinTrees_twoTrees()\n");
    Node* root1 = createNode(5);
    insertNode(&root1, 3);
    Node* root2 = createNode(8);
    insertNode(&root2, 7);

    Node* joined = joinTrees(root1, root2);
    ASSERT_EQ(joined, root2); 
}

void test_splitTree_singleNode() {
    printf("test_splitTree_singleNode()\n");
    Node* root = createNode(5);
    Node* left, *right;
    splitTree(root, &left, &right);
    ASSERT_EQ(left, NULL);
    ASSERT_EQ(right, NULL);
}

void test_splitTree_withChildren() {
    printf("test_splitTree_withChildren()\n");
    Node* root = createNode(5);
    insertNode(&root, 3);
    insertNode(&root, 8);

    Node* left, *right;
    splitTree(root, &left, &right);
    ASSERT_EQ(left->data, 3);
    ASSERT_EQ(right->data, 8);
    ASSERT_EQ(left->parent, NULL);
    ASSERT_EQ(right->parent, NULL);
    ASSERT_EQ(root->left, NULL);
    ASSERT_EQ(root->right, NULL);
}
int main() {
    Node* root = NULL;
    insertNode(&root, 5);
    insertNode(&root, 3);
    insertNode(&root, 8);
    insertNode(&root, 2);
    insertNode(&root, 4);

    printf("Initial Tree:\n");
    printTree(root, 0);

    // Пример сцепления и расцепления
    Node* root2 = createNode(10);
    insertNode(&root2, 7);
    insertNode(&root2, 12);

    root = joinTrees(root, root2);
    printf("\nTrees after merge:\n");
    printTree(root, 0);

    Node* left, *right;
    splitTree(root2, &left, &right);
    printf("\nLeft sub-tree:\n");
    printTree(left, 0);
    printf("\nRight sub-tree:\n");
    printTree(right, 0);

    test_findNode_exists();
    test_findNode_notExists();
    test_insertNode_emptyTree();
    test_insertNode_leftChild();
    test_insertNode_rightChild();
    test_joinTrees_emptyTrees();
    test_joinTrees_oneEmptyTree();
    test_joinTrees_twoTrees();
    test_splitTree_singleNode();
    test_splitTree_withChildren();

    printf("All test ran successfully!\n");
    return 0;
}