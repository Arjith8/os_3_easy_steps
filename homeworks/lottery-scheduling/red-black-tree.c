#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum Color{
    RED,
    BLACK
};

struct RedBlackTreeNode {
    int val;
    enum Color color; 
    struct RedBlackTreeNode *left_child;
    struct RedBlackTreeNode *right_child;
    struct RedBlackTreeNode *parent;
};

struct RedBlackTreeNode *create_node(int value) {
    struct RedBlackTreeNode *node = malloc(sizeof(*node));
    if (node==NULL){
        return NULL;
    }

    node->val = value;
    node->color = RED;
    node->left_child = NULL;
    node->right_child = NULL;
    node->parent = NULL;

    return node;
}

struct RedBlackTreeNode *rr_rotate(struct RedBlackTreeNode *node, struct RedBlackTreeNode *root){
    struct RedBlackTreeNode *parent_node = node->parent;
    struct RedBlackTreeNode *grand_parent_node = parent_node->parent;

    struct RedBlackTreeNode *great_grand_parent_node = grand_parent_node->parent;
    if (great_grand_parent_node != NULL){
        if (great_grand_parent_node->left_child == grand_parent_node)
            great_grand_parent_node->left_child = parent_node;
        else
            great_grand_parent_node->right_child = parent_node;
    }

    struct RedBlackTreeNode *parent_left_child = parent_node->left_child;
    parent_node->left_child = grand_parent_node;
    grand_parent_node->parent = parent_node;
    grand_parent_node->right_child = parent_left_child;
    if (parent_left_child != NULL)
        parent_left_child->parent = grand_parent_node;

    if (great_grand_parent_node == NULL)
        root = parent_node;
    
    return root;
}


int resolveConflicts(struct RedBlackTreeNode *node, struct RedBlackTreeNode *root){
    if (node->color != RED || node->parent->color != RED)
        return 0;
    

    struct RedBlackTreeNode *parent_node = node->parent;
    if (parent_node->parent == NULL){
        return 0;
    }

    struct RedBlackTreeNode *grandparent_node = parent_node->parent;
    struct RedBlackTreeNode *uncle_node;
    if (grandparent_node->left_child == node->parent)
        uncle_node = grandparent_node->right_child;
    else
        uncle_node = grandparent_node->left_child;

    if (uncle_node != NULL && uncle_node->color == RED){
        uncle_node->color = BLACK;
        parent_node->color = BLACK;
        if (grandparent_node != root){
            grandparent_node->color = RED;
            return resolveConflicts(grandparent_node, root);
        }
        return 0;
    } else {
        if (grandparent_node->right_child == parent_node && parent_node->left_child == node){
            parent_node->parent = node;
            parent_node->left_child = NULL;
            node->right_child = parent_node;
            grandparent_node->right_child = node;
            node->parent = grandparent_node;
            
            rr_rotate(node, root);
        }
        if (grandparent_node->left_child == parent_node && parent_node->right_child == node){
            parent_node->parent = node;
            parent_node->right_child = NULL;
            node->left_child = parent_node;
            grandparent_node->left_child = node;
            node->parent = grandparent_node;
            //ll rotate on grandparent - pending
        }
        // other 2 cases
    }
    return 0;
}

int insertNode(struct RedBlackTreeNode *node, struct RedBlackTreeNode *root){
    struct RedBlackTreeNode *current = root;
    while (current != NULL){
        if (current->val > node->val){
            if (current->left_child == NULL){
                node->parent = current;
                current->left_child = node;
                break;
            }
            current = current->left_child;
        } else {
            if (current->right_child == NULL){
                node->parent = current;
                current->right_child = node;
                break;
            }
            current = current->right_child;
        }
    };
    return 0;
}

int main(){
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    size_t values_len = sizeof(values)/sizeof(values[0]);

    struct RedBlackTreeNode *root = create_node(values[0]);
    if (root == NULL){
        perror("Node creation failed");
        return EXIT_FAILURE;
    }
    root->color = BLACK;

    for (int i = 1; i < values_len; i++) {
        struct RedBlackTreeNode *node  = create_node(values[i]);
        if (node == NULL) {
            perror("Node creation failed");
            return EXIT_FAILURE;
        }
        insertNode(node, root);
    }

    free(root);
    return 0;
}
