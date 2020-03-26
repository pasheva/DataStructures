/*bstt.h*/
/************************************************ 
 * Author: Mariya Pasheva
 * Date: 03.02.2020
 *
 * University of Illinois at Chicago
 *
 * Project 4: Threaded Binary Tree. The ides is to
 *          make the triversal inorder faster
 *          without a stack or recursion.
 * The idea: Make all right pointers of the child (if
 *            nullptrs) points to the inorder successor. (key)
 *            (if exists)
 *            inorder (left, root, right)
 *                      |_____^
 *                     left->right = root
 ************************************************/

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT> // Map<Key,Value>
class bstt
{
private:

    /* Cointainer NODES */
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => RightPtr is a thread, false => RightPtr is NOT a thread
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty) as a whole
  NODE* itr; // Iterator pointing the first inorder value. (Leftmost value)



    void _removeLeaves(NODE* cur, int& mask){
        if(cur == nullptr){
            mask=1;
            return;
        }
        _removeLeaves(cur->Left, mask);
        if(!cur->isThreaded) {
            _removeLeaves(cur->Right, mask);
        }else{
            cur->Right = nullptr;
            _removeLeaves(cur->Right, mask);
        }
        if(mask){
            mask = 0;
            Size--;
            delete cur;
        }
    }







    /* Helper triversal functions. */
  /* (root,left,right) */
  void _preorderCopy(NODE* cur){
      if( cur == nullptr){
          return;
      }
      this->insert(cur->Key, cur->Value);
      _preorderCopy(cur->Left);
      if(!cur->isThreaded){
          _preorderCopy(cur->Right);
      }
  }

  /* (left,right,root) */
  void _postorderDelete(NODE* cur){
      if (cur == nullptr){ //There is no NODE pointer.
          return;
      }else{
          _postorderDelete(cur->Left);
          if(!cur->isThreaded){
              _postorderDelete(cur->Right);
          }
          delete cur; //Once we get the root of the subtree, delete.
      }
  }

  /* (left, root, right) */
  void _inorderDump(NODE* cur, ostream& output)const{
      if(cur == nullptr){
          return;
      }else{
          _inorderDump(cur->Left, output);
          if(cur->isThreaded){
              if(cur->Right != nullptr){
                  output << "(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
              }else{
                  output << "(" << cur->Key << "," << cur->Value << ")" << endl;
              }
          }else{
              output << "(" << cur->Key << "," << cur->Value << ")" << endl;
              _inorderDump(cur->Right, output);
          }
      }
  }

public:
  /**
   * @note
   * default constructor
   *
   * @details
   * Creates an empty tree.
   */
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }



    void removeLeaves(){
        int mask = 0;
        NODE* cur = Root;
        _removeLeaves(cur,mask);
    }



    /**
   * @note
   * copy constructor
   *
   * @param
   *    A reference to already created obj tree. (lvalue)
   */
  bstt(const bstt& other)
  {
      this->Root = nullptr;
      this->Size = 0;
     _preorderCopy(other.Root);
     this->Size = other.Size;
  }

  /**
   * @note
   * destructor:
   *
   * @details
   * Called automatically by system when tree is about to be destroyed;
   * this is our last chance to free any resources / memory used by
   * this tree.
   *
   */
  virtual ~bstt()
  {
    this->Size = 0;
    _postorderDelete(this->Root);
  }


  /**
   * @note
   * Clears "this" tree and then makes a copy of the "other" tree.
   *
   * @param
   *  Reference to bstt object of already created tree. (lvalue)
   *
   * @return
   *  Reference (addr)to the (deep) copy of the other tree object.
   */
  bstt& operator= (const bstt& other)
  {
    this->clear();
    _preorderCopy(other.Root);

    return *this;
  }


  /**
   * @note
   * Clears the contents of the tree, resetting the tree to empty.
   */
  void clear()
  {
    if(this->Root == nullptr){
        return;
    }else{
        _postorderDelete(this->Root);
    }
    this->Root = nullptr;
    this->Size = 0;
  }


  /**
   * @note
   *  Returns the # of nodes in the tree, 0 if empty.
   *
   * @details
   *  Time complexity:  O(1)
   *
   * @return
   *  The size of the full tree. All nodes.
   */
  int size() const
  {
    return Size;
  }

  /**
   * @note
   *  Searches the tree for the given key, returning true if found
   *  and false if not.  If the key is found, the corresponding value
   *  is returned via the reference parameter.
   *
   * @details
   *  Time complexity:  O(lgN) on average (Binary search)
   *
   *  @param key
   *        Copy value of the key being searched for.
   *  @param value
   *        Reference to a varible, which will store the value of the found key.
   *
   *   @return
   *        True if the key has been found
   *        False if the key has not been found.
   */
  bool search(KeyT key, ValueT& value) const
  {
     NODE* cur = this->Root;

     while( cur != nullptr){

            // Found
            if( cur->Key == key){
                value = cur->Value;
                return true;
            }
            /* The searched key is smaller than the cur Node one */
            // Left
            else if(key < cur->Key){
                cur = cur->Left;
            }
            /* The searched key is larger than the cur Node one */
            // Right
            else if(cur->Key < key){
                // Right could be either a regular node with a key or **Threaded**
                /* Reaching a threaded child meaning the key is not in the tree. */
                if( cur->isThreaded){
                    return false;
                }else{
                    cur = cur->Right;
                }
            }
        }
    return false; //Default statement
    }

  /**
   * @note
   * Inserts the given key into the tree; If the key has already been insert then
   * the function returns without changing the tree.
   *
   * @details
   * Time complexity:  O(lgN) on average
   *
   * @param key
   *        Copy value of the key being inserted.
   * @param value
   *        Copy value of the value inserted with they key.
   */
  void insert(KeyT key, ValueT value)
  {
    NODE* cur = this->Root;
    NODE* parent = nullptr;

    // Insertion into an empty tree.
    if(cur == nullptr){
        cur = new NODE;
        cur->Key = key;
        cur->Value = value;
        cur->Left = nullptr;
        cur->Right = nullptr;
        cur->isThreaded = true;
        this->Size++;
        this->Root = cur;
        return;
    }
    /* We have at least a root */
    while(cur != nullptr){
        parent = cur;
        //If the key already exists we do no change the tree.
        if(cur->Key == key){
            return;
        }
        /* If the key is smaller than the current one.*/
        else if( key < cur->Key){
            cur = cur->Left;
        }else if(cur->Key < key){
            /* If the current node is thread we will keep looping between parent and child. */
            if(!cur->isThreaded) {
                cur = cur->Right;
            }else{
                break;
            }
        }
    }

    // Once the pointer is null.
    cur = new NODE;
    cur->Key = key;
    cur->Value = value;
    cur->Left = nullptr;


    /* We are only doing right threading */
    // CHECK wehther the parent has been threaded.
    if(parent->isThreaded){
        // CHECK the positioning of the node relevant to the parent
        /* Left of the Parent */
       if(cur->Key < parent->Key){
          cur->Right = parent;
          cur->isThreaded = true;
       }else {
           cur->Right = parent->Right;
           cur->isThreaded = true;
       }
    }else{
        // CHECK the positioning of the node relavqant to the Root.
        /* Right of the Root. */
        if(cur->Key > this->Root->Key){
            // If it is on the left of the parent it threads it to the parent.
           if(cur->Key < parent->Key){
               cur->Right = parent;
               cur->isThreaded = true;
           }else{
               cur->Right = nullptr;
               cur->isThreaded = false;
           }
        /* Left of the Root. */
        }else{
            cur->Right = parent;
            cur->isThreaded = true;
        }
    }

    // Updating the parent to point at the newly created child node.
    if(cur->Key < parent->Key){
        parent->Left = cur;
    }else{
        parent->Right = cur;
        parent->isThreaded = false;
    };

    this->Size++;
  }

  /**
   * @note
   * Returns the value for the given key; if the key is not found,
   * the default value ValueT{} is returned.
   *
   * @details
   * Time complexity:  O(lgN) on average
   *
   * @param key
   *        The key passed by value.
   * @return
   *        The vaue coresponding it to the key.
   */
  ValueT operator[](KeyT key) const
  {
    ValueT value;
    if(search(key, value)){
        return value;
    }

    return ValueT{};
  }

  /**
   * @info
   * Finds the key in the tree, and returns the key to the "right".
   * If the right is threaded, this will be the next inorder key.
   * if the right is not threaded, it will be the key of whatever
   * node is immediately to the right.
   * If no such key exists, or there is no key to the "right", the
   * default key value KeyT{} is returned.
   *
   * @details
   * Time complexity:  O(lgN) on worst-case
   *
   * @param key
   *        Passed by value
   *
   * @return
   *        The next key from the that one passed in the param.
   */
  KeyT operator()(KeyT key) const
  {

      NODE* cur = this->Root;

      while( cur != nullptr){

          // Found
          if( cur->Key == key){
              if(cur->Right != nullptr){
                return cur->Right->Key;
              }else{
                  return KeyT{};
              }
          }
          //Left
          else if(key < cur->Key) {
              cur = cur->Left;
          }
          // Right
          else if(cur->Key < key){
              // Right could be either a regular node with a key or **Threaded**
              /* Reaching a threaded child meaning the key is not in the tree. */
              if( cur->isThreaded){
                  return KeyT{};
              }else{
                  cur = cur->Right;
              }
          }
      }
      return KeyT{};
  }

  /**
   * @info
   * Resets internal state for an inorder traversal.  After the
   * call to begin(), the internal state denotes the first inorder
   * key; this ensure that first call to next() function returns
   * the first inorder key.
   *
   * @details
   * Space complexity: O(1)
   * Time complexity:  O(lgN) on worst-case
   *
   * @example
   *    tree.begin();
   *    while (tree.next(key))
   *      cout << key << endl;
   */
  void begin()
  {
      NODE* cur = this->Root;
      itr = nullptr;

      while(cur != nullptr){
          itr = cur;
          cur = cur->Left;
      }
  }

  /**
   * @info
   * Uses the internal state to return the next inorder key, and
   * then advances the internal state in anticipation of future
   * calls.  If a key is in fact returned (via the reference
   * parameter), true is also returned.
   *
   * False is returned when the internal state has reached null,
   * meaning no more keys are available.  This is the end of the
   * inorder traversal.
   *
   * @details
   * Space complexity: O(1)
   * Time complexity:  O(lgN) on worst-case
   *
   * @example
   *    tree.begin();
   *    while (tree.next(key))
   *      cout << key << endl;
   */
  bool next(KeyT& key)
  {
      //Base cases
      if(itr == nullptr){
          key = KeyT{};
          return false;
      }
      key = itr->Key;
      // Case: If the node is thr return the key on the right of it.
      if (itr->isThreaded){
          itr = itr->Right;
          return true;
      }else{
          //       1. Go to right once
          itr = itr->Right;
          //       2. Go to down all the way left. If left exists.
          while(itr->Left != nullptr){
              itr = itr->Left;
          }
          return true;
      }
    return false;
  }

  /**
   * @info
   * Dumps the contents of the tree to the output stream, using a
   * recursive inorder traversal.
   *
   * @param cout
   */
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
    // Private helper function.
    _inorderDump(this->Root, output);

    output << "**************************************************" << endl;
  }
	
};

