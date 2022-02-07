/*  Eli Wilson 
    laba 11/23/2021
    implements binary search tree
*/
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS202::BSTree;
using CS202::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

int BSTree::Depth(const string &key) const
{
	int depth = 0;
  BSTNode * node = sentinel->right;
  // searches for node with key from top to bottom
  while(true){
    if(node == sentinel){ // sentinel = bottom
      return -1;
    }
    if(node->key > key){
      depth++;
      node = node->left;
    }else if(node->key < key){
      depth++;
      node = node->right;
    }else{
      return depth;
    }
  }
}
           
int BSTree::Height() const // total height from top to lowest node
{
  int h = this->recursive_find_height(sentinel->right);
  return h;
}

vector <string> BSTree::Ordered_Keys() const
{
  vector <string> rv;
  this->make_key_vector(sentinel->right, rv);
  return rv;
}
    
BSTree::BSTree(const BSTree &t)        
{
  // creates sentinel then uses assignment overload
  sentinel = new BSTNode;
  sentinel->left = NULL;
  sentinel->right = sentinel;
  sentinel->parent = NULL;
  sentinel->key = "---SENTINEL---";
  sentinel->val = NULL;
  *this = t;

}

BSTree& BSTree::operator= (const BSTree &t) 
{
  // makes balanced tree with vector of keys and vals in t
  vector <string> keys;
  vector <void *> vals;

  keys = t.Ordered_Keys();
  vals = t.Ordered_Vals();
  size = vals.size();

  sentinel->right = make_balanced_tree(keys, vals, 0, keys.size());
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
  // Visits children, adds 1 to max height
  int l, r;
  
  if(n == sentinel){ 
    return 0;
  }
  l = this->recursive_find_height(n->left);
  r = this->recursive_find_height(n->right);
  if(l > r){
    return l + 1;
  }else{
    return r + 1;
  }
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
	if (n == sentinel) return;
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  // creates tree recursively starting from middle of key/val vector, expanding outward.
  BSTNode * node;
  size_t ind, lnum, rnum;


  if(num_indices == 0){
    return sentinel;
  }

  node = new BSTNode; // parent node
  if(num_indices == vals.size()){
    sentinel->right = node;
  }

  ind = (num_indices/2)+first_index;
  lnum = ind-first_index;
  rnum = ind-first_index;
  if(num_indices % 2 == 0){ // takes larger middle value if num_indices is even
    if(sorted_keys[ind] < sorted_keys[ind - 1]){
      ind--;
      lnum--;
    }else{
      rnum--;
    }
  }
  node->key = sorted_keys[ind];
  node->val = vals[ind];
  node->left = this->make_balanced_tree(sorted_keys, vals, first_index, lnum);
  node->right = this->make_balanced_tree(sorted_keys, vals, ind+1, rnum);
  node->left->parent = node;
  node->right->parent = node;
  return node;
}
