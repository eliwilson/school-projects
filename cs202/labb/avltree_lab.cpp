#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "avltree.hpp"
using namespace std;
using CS202::AVLTree;
using CS202::AVLNode;

// checks if height of children differs by > 1
bool imbalance(const AVLNode *n){
  int diff = n->left->height - n->right->height;
  if(diff > 1 || diff < -1){
    return true;
  }else{
    return false;
  }

}

// Finds child with greater height
// then sets height to child height +1
void fix_height(AVLNode *n){
  if(n->left->height > n->right->height){
    n->height = n->left->height+1;
  }else{
    n->height = n->right->height+1;
  }
}

void rotate(AVLNode *n){
  AVLNode *m; // parent's other child node
  AVLNode *p = n->parent; // parent node
  if(p->height == 0){ // checks if parent is sentinel
    return;
  }
  AVLNode *gp = p->parent; // Grandparent node

  // checks what side of p n is on and sets children
  if(p->left == n){
    m = n->right;
    p->left = m;
    n->right = p;
  }else{
    m = n->left;
    p->right = m;
    n->left = p;
  }
  // sets parents
  p->parent = n;
  n->parent = gp;
  m->parent = p;

  // sets grandparents child
  if(gp->right == p){
    gp->right = n;
  }else{
    gp->left = n;
  }

  fix_height(p);
  fix_height(n);
}

void fix_imbalance(AVLNode *n){
  //left = false, right = true
  bool dir1; //direction from n to child with greater height
  bool dir2; // direction from child to grandchild with greater height

  dir1 = n->right->height > n->left->height;

  if(dir1){ // right side
    dir2 = n->right->right->height >= n->right->left->height;
    if(dir2){ // zigzig
      rotate(n->right);
    }else{ // zigzag
      rotate(n->right->left);
      rotate(n->right);
    }
  }else{ // left side
    dir2 = n->left->right->height > n->left->left->height;
    if(!dir2){ // zigzig
      rotate(n->left);
    }else{ // zigzag
      rotate(n->left->right);
      rotate(n->left);
    }
  }

}


AVLTree& AVLTree::operator= (const AVLTree &t)        
{
  size = t.Size();
  sentinel->right = recursive_postorder_copy(t.sentinel->right);
  sentinel->right->parent = sentinel;
  return *this;
}

/* I simply took Insert and Delete from their binary search tree
   implementations.  They aren't correct for AVL trees, but they are
   good starting points.  */

bool AVLTree::Insert(const string &key, void *val)
{
  AVLNode *parent;
  AVLNode *n;

  parent = sentinel;
  n = sentinel->right;

  /* Find where the key should go.  If you find the key, return false. */

  while (n != sentinel) {
    if (n->key == key) return false;
    parent = n;
    n = (key < n->key) ? n->left : n->right;
  }

  /* At this point, parent is the node that will be the parent of the new node.
     Create the new node, and hook it in. */

  n = new AVLNode;
  n->key = key;
  n->val = val;
  n->parent = parent;
  n->height = 1;
  n->left = sentinel;
  n->right = sentinel;

  /* Use the correct pointer in the parent to point to the new node. */

  if (parent == sentinel) {
    sentinel->right = n;
  } else if (key < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }

  /* Increment the size */

  size++;

  // goes up tree fixing imbalances/heights
  while(n->parent != sentinel){
    n = n->parent;
    if(imbalance(n)){
      fix_imbalance(n);
    }
    fix_height(n);
  }

  return true;
}
    
bool AVLTree::Delete(const string &key)
{
  AVLNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  /* Try to find the key -- if you can't return false. */

  n = sentinel->right;
  while (n != sentinel && key != n->key) {
    n = (key < n->key) ? n->left : n->right;
  }
  if (n == sentinel) return false;

  /* We go through the three cases for deletion, although it's a little
     different from the canonical explanation. */

  parent = n->parent;

  /* Case 1 - I have no left child.  Replace me with my right child.
     Note that this handles the case of having no children, too. */

  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;

  /* Case 2 - I have no right child.  Replace me with my left child. */

  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;

  /* If I have two children, then find the node "before" me in the tree.
     That node will have no right child, so I can recursively delete it.
     When I'm done, I'll replace the key and val of n with the key and
     val of the deleted node.  You'll note that the recursive call 
     updates the size, so you don't have to do it here. */

  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
    return true;
  }

  AVLNode *e; // node used to traverse tree
  e = parent;

  // goes up tree fixing imbalances/heights
  while(e != sentinel){
    if(imbalance(e)){
      fix_imbalance(e); 
    }
    fix_height(e); 
    e = e->parent;
  }

  return true;
}
               
/* You need to write these two.  You can lift them verbatim from your
   binary search tree lab. */

vector <string> AVLTree::Ordered_Keys() const
{
  vector <string> rv;
  this->make_key_vector(sentinel->right, rv);
  return rv;
}
    
void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const
{
  if (n == sentinel) return;
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}
     
size_t AVLTree::Height() const
{
  return sentinel->right->height;
}

/* You need to write this to help you with the assignment overload.
   It makes a copy of the subtree rooted by n.  That subtree is part
   of a different tree -- the copy will be part of the tree that
   is calling the method. */

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
  if(n->height == 0){
    return sentinel;
  }

  AVLNode *a = new AVLNode;

  a->key = n->key;
  a->val = n->val;
  a->height = n->height;

  // copies left and right subtree and sets their parent
  a->left = recursive_postorder_copy(n->left);
  a->right = recursive_postorder_copy(n->right);
  a->left->parent = a;
  a->right->parent = a;
  
  

  return a;
}
