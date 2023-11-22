/* 
    Project 4 - Priority Queue (priorityqueue.h)

    Kseniia Nastahunina

    The class is intended for create and manuplate a custom made binary search tree
    by using priority queue. Every node in the tree has value and priority integer
    which identefies the priority of the node. The custom tree differs form a regular 
    BST by allowing dublicates whitch are linked to the first node with the same priority. 

*/
#pragma once

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  //priority of the node
        T value;  //stored data for the p-queue
        bool dup;  //marked true when there are duplicate priorities
        NODE* parent;  //links back to parent
        NODE* link;  //links the NODES with duplicate priorities
        NODE* left;  //links to left child
        NODE* right;  //links to right child
    };
    NODE* root;  //pointer to root node of the BST
    int size;  //# of elements in the pqueue
    NODE* curr;  //pointer to next item in pqueue (see begin and next)

    /* Recursive toString function. Takes current node and output type as
    parameters. Prints out all of the elements in the BST using inorder traversal. */
    void _toString(NODE* cur, ostream& output) {
        if (cur == nullptr){
            return;
        }
        //go left
        _toString(cur->left, output);
        //add to string
        output << cur->priority << " value: " << cur->value << endl;
        //go link
        _toString(cur->link, output);
        //go right
        _toString(cur->right, output);
    }

    /* Recursive clear function. Takes current node as parameter.
    Deletes/deallocates all BST enteries using preorder traversal. */
    void _clear(NODE* cur){
        if(cur == nullptr){
            return;
        }
        //go left
        _clear(cur->left);
        //go right
        _clear(cur->right);
        //go link
        _clear(cur->link);
        //go self
        delete cur;
    }

    /*Recursive compare function for == operator. Takes current node of "this" BST and 
    current node of "other" BST as parameters. Compares two trees returns true if they 
    are the same. */
    bool _compare(NODE* thisCur, NODE* otherCur) const {
        if(thisCur->priority != otherCur->priority){ //if priorites are different 
            return false;
        }
        if(thisCur->value != otherCur->value){ //if values are different
            return false;
        }

        bool isLinkEqual = true; //used in the return statment at the end of the function
        bool isLeftEqual = true;
        bool isRightEqual = true;

        if(thisCur->link != nullptr || otherCur->link != nullptr){ //if one of the links isnt nullptr
            if (thisCur->link == nullptr || otherCur->link == nullptr){ //if one of the links is nullptr return false
                return false;
            }
            else{
                isLinkEqual = _compare(thisCur->link, otherCur->link); //call for the next link
            }
        }
        if(thisCur->left != nullptr || otherCur->left != nullptr){ //if one of the left children isnt nullptr
            if (thisCur->left == nullptr || otherCur->left == nullptr){ //if one of the left children is nullptr return false
                return false;
            }
            else{
                isLeftEqual = _compare(thisCur->left, otherCur->left); //call for the next left child
            }
        }
        if(thisCur->right != nullptr || otherCur->right != nullptr){ //if one of the right children isnt nullptr
            if (thisCur->right == nullptr || otherCur->right == nullptr){ //if one of the right children is nullptr return false
                return false;
            }
            else{
                isRightEqual = _compare(thisCur->right, otherCur->right); //call for the next right child
            }
        }
        if(isLinkEqual && isLeftEqual && isRightEqual){ //if left, right, and link are equal(true) return true
            return true;
        }
        else{ // not equal return false
            return false;
        }
    }

    /* Recursive copy function. Takes current node as parameter.
    Copies all BST enteries into another BTS using postorder traversal. */
    void _copy(NODE* cur){
        if(cur == nullptr){
            return;
        }
        //enqueue self
        enqueue(cur->value, cur->priority);
        //go link
        _copy(cur->link);
        //go left
        _copy(cur->left);
        //go right
        _copy(cur->right);
    }
    
public:
    /*Default constructor. Creates an empty BTS. */
    priorityqueue() {
        root = nullptr;
        size = -1;
        curr = nullptr;
    }
    
    /* Copy operator. Takes priority queue to copy to. Clears "this" tree using 
    clear() function and then makes a copy of the "other" tree using private 
    recursion function. */
    
    priorityqueue& operator=(const priorityqueue& other) {
        
        // TO DO: write this function.
        clear();

        _copy(other.root);

        return *this;
    }
    
    /* Clear function. Deletes/deallocates all BST enteries using private recursive 
    function. Settes all values to it initial condition. */
    void clear() {
        _clear(root);
        root = nullptr;
        curr = nullptr;
        size = -1;
    }
    
    /* Frees the memory associated with the priority queue using clear function. */
    ~priorityqueue() {
        
        clear();
    
    }
    
    /* Add function. Takes type T value and priority of the new node as parameters.
    Inserts the value into the custom BST in the correct location based on
    priority. */
    void enqueue(T value, int priority) {
        NODE* newNode = new NODE(); // allocate new node to insert
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->link = nullptr;
        newNode->priority = priority; // set priority to new node given in parameters
        newNode->value = value; // set value to new node given in parameters
        newNode->dup = false;

        if(priority > -1){ //check that priority is valid
            if (root == nullptr) { //base case empty tree
                newNode->parent = nullptr;
                root = newNode;
                size++; 
                return;
            }

            NODE* cur = root; // create new node to trace current
            while (cur != nullptr){ // find empty space
                if(priority == cur->priority){ //check for duplicates first
                    cur->dup = true;
                    if(cur->link == nullptr){ //if there is no more duplicate
                        cur->link = newNode; //set next link to new node
                        newNode->parent = cur; //set parent of new node to previous linked node
                        cur = nullptr; //reset cur
                    }
                    else{
                        cur = cur->link;//if there more than one duplicate
                    }
                }
                else if(priority > cur->priority){ //if new priority is greater than current node's priority
                    if (cur->right == nullptr){ // if current node does not have a right child
                        cur->right = newNode; //set right child of current node to new node
                        newNode->parent = cur; //link the parent of new node
                        cur = nullptr; //reset cur
                    }
                    else { //if current node has right child
                        cur = cur->right; //continue search for null space to insert
                    }
                }
                else if(priority < cur->priority){ // if new priority is less than current priority
                    if (cur->left == nullptr){ // if node does not have left child
                        cur->left = newNode; //set left child of current node to new node
                        newNode->parent = cur; //link the parent of new node
                        cur = nullptr; //reset cur
                    }
                    else { //if current node has left child
                        cur = cur->left; //continue search for null space to insert
                    }
                }
            }
            size++; //increas size of priority queue
        }
        else{ //if new nodes value is invalid
            cout << "Cannot add negative priority. Please try again" << endl;
        }
    }
    
    /* Dequeue function. Deletes next node in order. Returns the value of the next element in the priority queue and 
    removes the element from the priority queue. */
    T dequeue() {
        NODE* cur = root; //creates new node to trace next node to remove

        if(cur == nullptr){ //base case empty tree
            return NULL;
        }
        while(cur->left != nullptr){ //finds leftmost (lowest priority) node 
            cur = cur->left; //sets current to the left most node
        }

        T valueOut = cur->value; //sets value of the node to delete to return it after deletion

        //relinking nodes for deletion
        if(cur->link != nullptr){ //checks for the link first
            if(cur == root){ //if the node with the links is root
                root = cur->link; //the next link becomes the roog
            }
            else if(cur != root){ //if not the root
                cur->parent->left = cur->link; //sets the parent's left to next link so the link would take place of the deleted node
            }
            cur->link->parent = cur->parent; //relink parent
            cur->link->right = cur->right; //relink node's right
        }
        else if(cur->right != nullptr){ //if cur right exists
            cur->right->parent = cur->parent; //relink parent
            if(cur != root){ //if current is root
                cur->parent->left = cur->right; //link new parent to the current nodes right
            }
            else if(cur == root){ //if cur is root
                root = cur->right; //reset root to the right since there are no left because it must be already deleted
            }
        }
        else if(cur->right == nullptr){ //if no right
            if(cur == root){ //if cur is root
                root = nullptr; //set to nullptr since it must be the last node  left
            }
            else{ //not root
                cur->parent->left = nullptr; //set to nullptr since it must be last node form the left 
            }
        }

        delete cur; //delete the node after relinking left, right, parents, links and possibly setting new roots
        size--; //decrease the size of the priority queue
        

        return valueOut; // return the value of the deleted node
    }
    
    /* Size getter. Returns the # of elements in the priority queue, -1 if empty.*/

    int Size() {
        
        
        return size; // TO DO: update this return
        
        
    }
       
    /*Begin function that finds the first node of inorder traversal. 
    Resets internal state for an inorder traversal. After the
    call to begin(), the internal state denotes the first inorder
    node; this ensure that first call to next() function returns
    the first inorder node value. */
    void begin() {
        
        curr = root; //using curr pointer
        if(curr == nullptr){ //base case
            return;
        }
        while(curr->left!=nullptr){ //find leftmost node
            curr = curr->left;
        }
        
    }
    
    /* Next function findes the next node of inorder traversal. Takes 
    value and priority of a node as parameter that might be used to print/return it.
    Uses the internal state to return the next inorder priority, and
    then advances the internal state in anticipation of future
    calls.  If a value/priority are in fact returned (via the reference
    parameter), true is also returned.
    
    False is returned when the internal state has reached null,
    meaning no more values/priorities are available.  This is the end of the
    inorder traversal.*/
    bool next(T& value, int &priority) {

        NODE* prev = nullptr; //previous node used later to compare too nodes and returnn to the correct node
        priority = curr->priority; //set priority and value
        value = curr->value;

        if(curr->link != nullptr){ //if link from current node exists
            curr = curr->link; //set curr to link
            return true; //there is a node to return
        }
        //the if statment below checks if the current node does not have a link
        //and not a root and its priority is the same as its parents
        //meaning it is the last linked node
        else if(curr->link == nullptr && curr != root && (curr->priority == curr->parent->priority)){
            while(curr != root && curr->priority == curr->parent->priority){ //returning back to the first linked node
                curr = curr->parent;
            }
        }
        if(curr->right == nullptr){  //if no right
            while(curr->right == prev){ //while curr's right is curr(or nullpointer the first loop)
                prev = curr; //reset prev to current node
                if(curr == root){ //if root meaning we are at the end of traversal
                    curr = nullptr;
                    return false;
                }
                curr = curr->parent; //reset curr to its parent for the next while loop to compare
            }
        }
        else { //if there is right
            curr = curr->right; //reset
            while(curr->left != nullptr){
                curr = curr->left; //find leftmost of right node
            }
        }
        
        return true;
        
    }
    
    /*ToString function. Calls the recursive private function. Returns the final formated string with
    all entaries of priority queue.*/
    string toString() {
        
        stringstream stream;
        _toString(root, stream);
        string str = stream.str();
        return str; 
        
    }
    
    
    /*Peek function. Finds the next value in the priority queue and returns its value.*/
    T peek() {

        NODE *cur = root;
        if(cur == nullptr){
            return NULL;
        }
        while(cur->left != nullptr){
            cur = cur->left;
        }
        T valueOut = cur->value;

        return valueOut; 
        
        
    }
    
    
    /*Equality operator. Chechs if two trees are the same. Returns true if this priority queue as the 
    priority queue passed in as other.  Otherwise returns false.*/

    bool operator==(const priorityqueue& other) const {

        if(this->root == nullptr && other.root == nullptr){ //base case
            return true;
        }
        if((this->root == nullptr && other.root != nullptr) || (this->root != nullptr && other.root == nullptr)){ //if roots are different
            return false;
        }
        
        return _compare(this->root, other.root);

    }
    
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }
};
