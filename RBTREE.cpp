

#include <iostream>
#include <vector>

using namespace std;


class Node{
public:
    bool red;//hold the color of the node
    int val;//holds the data of the node
    Node* parent;//holds the parent pointer of the node
    Node* left;//holds the left and rigth pointer of the node
    Node* right;
};





class RBTree {//create a tree class so its easier to manage funcitons
public:
    Node* root;//holds the root of the tree
    Node* TNULL;//VERY IMPORTANT it is the value of the sentinel nodes in the tree, it does not have a value but it has a color and a parent very very important otherwise we will need to make multiple functions to get rid of edge cases

    RBTree() {//constuctor for the tree, makes the TNULL for the tree
        TNULL = new Node;//TNULL is a sentinel node which means that it will be black but it will not have value or left and right pointers but it will have a parent pointer
        TNULL->red = false;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }
    
    void fixDelete(Node* cur) {
        Node* brother;//new node that holds the brother node
        while (cur != root && cur->red == false) {//while it is black and not root so we can fix the too many black nodes violation
            if (cur == cur->parent->left) {//if the node is a left child
                brother = cur->parent->right;//the brother becomes the right child of the parent of the node
                if (brother->red == true) {//if the brother is red we will do case 1 fix

                    brother->red = false;
                    cur->parent->red = true;
                    leftRotate(cur->parent);
                    brother = cur->parent->right;
                }

                if (brother->left->red == false && brother->right->red == false) {//if the brother's left child is black and the brother's right child is black as well case 2 fix(this is a double black)

                    brother->red = true;
                    cur = cur->parent;
                } else {
                    if (brother->right->red == false) {//if only the right child is black this is still a double black violation that we need to fix

                        brother->left->red = false;
                        brother->red = true;
                        rightRotate(brother);
                        brother = cur->parent->right;
                    }
                    

                    brother->red = cur->parent->red;//fix the color violation by recoloring and rotating
                    cur->parent->red = false;
                    brother->right->red = false;
                    leftRotate(cur->parent);
                    cur = root;//after we fix this the whole tree should be balanced so we will se the current to root to stop the while loop
                }
            } else {//this is the same thing except the cur node is a right child and its brother will be a left child
                brother = cur->parent->left;
                if (brother->red == true) {

                    brother->red = false;
                    cur->parent->red = true;
                    rightRotate(cur->parent);
                    brother = cur->parent->left;
                }

                if (brother->right->red == false && brother->left->red == false) {

                    brother->red = true;
                    cur = cur->parent;
                } else {
                    if (brother->left->red == false) {

                        brother->right->red = false;
                        brother->red = true;
                        leftRotate(brother);
                        brother = cur->parent->left;
                    }


                    brother->red = cur->parent->red;
                    cur->parent->red = false;
                    brother->left->red = false;
                    rightRotate(cur->parent);
                    cur = root;
                }
            }
        }
        cur->red = false;//will set the cur node to be black, and cur is the root or if the while loop does not run it is originally red and we just change it to be black so we can keep the black node property
    }


    void swap(Node* u, Node* v){//swaps the two nodes's parent pointers
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left){
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNode(int find) {//delete function
        Node* node = root;
        Node* temp = TNULL;
        Node* cur;
        Node* holder;
        while (node != TNULL){//finds if the node is in the tree, and if it isnt it will throw an error
            if (node->val == find) {
                temp = node;
            }

            if (node->val <= find) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        //at this point temp should either be pointing to TNULl or the node we are trying to delete
        if (temp == TNULL) {
            std::invalid_argument("the node you are trying to delete is not in the tree");//we will check if the node we are trying to find is in the tree, if not we will throw an error
        }
        
        holder = temp;//this will hold the node we are trying to delete
        bool oColor = holder->red;//a variable that stores the original color of the node we are trying to delete
        if (temp->left == TNULL) {//if there is no left node or if the node has no children we will set the node we are tryingto delete to the right child
            cur = temp->right;
            swap(temp, temp->right);//after setting it to the right child we will swap them, if the right child is null, the node will just get replaced by the sentinel node which we will use to fix the tree
        } else if (temp->right == TNULL) {//if the child has a left node but not a right node
            cur = temp->left;//we will do the same thing
            swap(temp, temp->left);
        } else {//if the node has both the right and left child
            holder=temp->left;//we will find the next best candidate to replace the node by going left and going all the way right
            while(holder->right != TNULL){//one thing to note that there are multiple ways to get the candidate, we can also go right and then all the way left
                 holder = holder->right;//however since doing it this way gets the right answer and it is also on the reference given to us(inorder predecessor) we will go left and then all the way right
            }
            oColor = holder->red;//set the original color of the candidate
            cur = holder->left;
            if (holder->parent == temp) {//check if the candidate is the child of temp, if it is we can just change the parents if it isnt we will have to use the swap function
                cur->parent = holder;
              } else {
                swap(holder, holder->left);
                holder->left = temp->left;
                holder->left->parent = holder;
            }

            swap(temp, holder);//after swapping we will need to set thechildren
            holder->right = temp->right;
            holder->right->parent = holder;
            holder->red = temp->red;
        }
        delete temp;//we will deallocate the memory of the node we want to delete
        if (oColor == false){//if the original color was black we will ahve to fix the tree because this will violate black shortest path law of the tree
            fixDelete(cur);
        }
    }

    void fixInsert(Node* k){//this function will fix the tree after inserting a new node
        Node* uncle;
        while (k != root && k->parent->red == true) {//this is while the parent is also red and because the newly inserted node must be red and in a red black tree you are not allowed to have 2 reds in a row, we will fix the tree until this violation is gone
            if (k->parent == k->parent->parent->right) {//we will first see if the parent is a right child or a left child so we can get the uncle
                uncle = k->parent->parent->left;//after checking we can get the uncle
                if (uncle->red == true) {//if the uncle is red we can do case 1

                    uncle->red = false;
                    k->parent->red = false;
                    k->parent->parent->red = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {//if uncle is black and the node we have is a left child we can do case 2

                        k = k->parent;
                        rightRotate(k);
                    }
                    //after rotating we will have case 3

                    k->parent->red = false;
                    k->parent->parent->red = true;
                    leftRotate(k->parent->parent);
                }
            } else {//if the parent is a right child, we will get the uncle a different way
                uncle = k->parent->parent->right;//everything is mirrored here

                if (uncle->red == true) {
                    uncle->red = false;
                    k->parent->red = false;
                    k->parent->parent->red = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->red = false;
                    k->parent->parent->red = true;
                    rightRotate(k->parent->parent);
                }
            }

        }
        root->red = false;
    }

    void leftRotate(Node* cur) {//left rotate function
        Node* holder = cur->right;//a holder pointer to hold nodes we need
        cur->right = holder->left;//we will set the nodes right pointer to its rights child's left child
        if (holder->left != TNULL) {//if hodler has a left child, then we can set the parent of the left child to cur
            holder->left->parent = cur;
        }
        holder->parent = cur->parent;//we set parents
        if (cur == root) {//if cur is the root
            this->root = holder;
        } else if (cur == cur->parent->left) {//if it is not the root then we will have to link the parent of cur to holder, so we need to check if cur is a right or left child and then link accordingly
            cur->parent->left = holder;
        } else {
            cur->parent->right = holder;
        }
        holder->left = cur;//link cur to holder and set cur's parent to be holder to complete the rotation
        cur->parent = holder;
    }
    void rightRotate(Node* cur) {//same thing as the left rotate but it is mirrored
        Node* holder = cur->left;
        cur->left = holder->right;
        if (holder->right != TNULL) {
            holder->right->parent = cur;
        }
        holder->parent = cur->parent;
        if (cur == root) {
            root = holder;
        } else if (cur == cur->parent->right) {
            cur->parent->right = holder;
        } else {
            cur->parent->left = holder;
        }
        holder->right = cur;
        cur->parent = holder;
    }

    void insert(int val) {//insert value to the tree

        Node* node = new Node;
        node->parent = TNULL;
        node->val = val;
        node->left = TNULL;
        node->right = TNULL;
        node->red = true;
        //creating the new node, we must set everything that is supposed to be null to be TNull so we can get rid of edge casees in the future
        //note that the color is red because when we insert a new node the color must be red

        Node* holder = nullptr;//node we use to traverse the tree
        Node* cur = root;//set cur to the root so we can traverse the tree to insert the node

        while (cur != TNULL) {//simple binary insert traversal to find the node we should set as the parent to the node we need to insert
            holder = cur;
            if (node->val < cur->val) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }


        node->parent = holder;//we will link the node's parent
        if (holder == nullptr) {//we will check if the inserted node will be the root or not
            root = node;
        } else if (node->val < holder->val) {//simple binary tree insertion
            holder->left = node;
        } else {
            holder->right = node;
        }

        fixInsert(node);//call the fix insert function, the fix insert will tackle the edge cases
    }

    void preorder(Node* np) {//pre order function that prints the tree in preorder
        if(np != TNULL) {
            std::cout << np->val << " ";
            preorder(np->left);
            preorder(np->right);
        }
    }

};

int main() {
    RBTree bst;
    vector<int> nums = {10, 5, 6, 4, 13, 67, 8, 2, 34, 25, 47, 12, 14, 1, 9};//create the vector
    for(int n : nums) {//insert the nodes
        bst.insert(n);
    }
    cout << "This is the tree before deleting stuff ";
    bst.preorder(bst.root);
    bst.deleteNode(5);//delete thenodes
    bst.deleteNode(2);
    bst.deleteNode(47);
    bst.deleteNode(12);
    cout << endl << "This is the tree after deleting stuff ";
    bst.preorder(bst.root);

}
