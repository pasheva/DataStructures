/*avlt.h*/

/************************************************
 * Author: Mariya Pasheva
 * Date: 03.17.2020
 *
 * University of Illinois at Chicago
 *
 * Project 4: Right Threaded AVL Tree.
 *
 ************************************************/

#pragma once

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
    int    Height;     // height of tree rooted at this node
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size = 0;  // # of nodes in the tree (0 if empty)
  NODE* itr; // Iterator pointing the first inorder value. (Leftmost value)



    /* Helper function for the range_search which points the itr to the lower value. */
    void _begin(KeyT key)
    {
            itr = Root;

        if (itr != nullptr) {
           while (itr->Left != nullptr){
               if (itr->Key >= key){
                   itr = itr->Left;
               }else{
                   if (!itr->isThreaded) {
                       itr = itr->Right;
                   }else{
                       break;
                   }
               }
           }
        }
    }


    /* Helper triversal functions for copy constructor  and operator= | Preorder */
    void _Copy(NODE* cur)
    {
        stack<NODE*> nodes;
        if( cur == nullptr){
            return;
        }
        nodes = this->_simpleInsert(cur->Key, cur->Value);
        if(!nodes.empty()){
            _CopyHeight(nodes);
        }
        _Copy(cur->Left);
        if(!cur->isThreaded){
            _Copy(cur->Right);
        }
    }


    /* Helper function for the destructor | Postorder */
    void _Delete(NODE* cur)
    {
        if (cur == nullptr){
            return;
        }else{
            _Delete(cur->Left);
            if(!cur->isThreaded){
                _Delete(cur->Right);
            }
            delete cur;
        }
    }


    /* Helper function in order to dump the proper contents.  */
    void _Dump(NODE* cur, ostream& output) const
    {
        if (cur == nullptr) {
            return;
        } else {
            _Dump(cur->Left, output);
            if (cur->isThreaded) {
                if (cur->Right != nullptr) {
                    output << "(" << cur->Key << "," << cur->Value << "," << cur->Height << "," << cur->Right->Key << ")" << endl;
                } else {
                    output << "(" << cur->Key << "," << cur->Value << "," << cur->Height  << ")" << endl;
                }
            } else {
                output << "(" << cur->Key << "," << cur->Value << "," << cur->Height  << ")" << endl;
                _Dump(cur->Right, output);
            }
        }
    }


    /* Helper function for copy constructor height update */
    void _CopyHeight(stack<NODE*>& node)
    {
        NODE *cur = nullptr;
        int newH;
        int prevH;
        while (!(node.empty())) {
            cur = node.top();
            prevH = cur->Height;
            int HL, HR;
            // let HL = height of cur's left sub-tree (-1 if empty);
            HL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
            // let HR = height of cur's right sub-tree (-1 if empty);
            if (cur->isThreaded) {
                HR = -1;
            } else {
                HR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
            }
            newH = 1 + max(HL,HR);
            if(prevH != newH){
                cur->Height = newH;
            }else{
                break;
            }
            node.pop();
        }
    }


    /* Helper function updating the heights for regular insertion */
    void _updateHeight(stack<NODE*>& node, NODE** childN, NODE** parentN, NODE** N)
    {
        NODE* cur = nullptr;
        int balance;
        int prevH;
        int newH;
        while (!(node.empty()))
        {
            cur = node.top();
            prevH = cur->Height;

            int HL,HR;

            // let HL = height of cur's left sub-tree (-1 if empty);
            HL = (cur->Left == nullptr)?-1:cur->Left->Height;
            // let HR = height of cur's right sub-tree (-1 if empty);
            if(cur->isThreaded){
                HR = -1;
            }else{
                HR = (cur->Right == nullptr)?-1:cur->Right->Height;
            }

            //Updating the height of a node only if it is different
            //otherwise, we break since the height has not changed,
            //therefore, the parent nodes' heights would not change.
            newH = 1 + max(HL,HR);
            if(prevH != newH){
                cur->Height = newH;
            }else{
                break;
            }

            balance = _BalanceFactor(cur);

            // If the balance at the node is borken assign to N.
            if( *N == nullptr  && (balance == 2 || balance == -2)){
                    *N = cur;
                    node.pop();
                    if(!node.empty()){
                        *parentN = node.top();
                        node.pop();
                    }
                    break;
            }else{
                if( *N == nullptr){
                    *childN = node.top();
                }
                node.pop();
            }
        }
    }


    NODE* _getActualRight(NODE* cur) const
    {
        if (cur->isThreaded)  // then actual Right ptr is null:
            return nullptr;
        else  // actual Right is contents of Right ptr:
            return cur->Right;
    }


    /*
    * Rotates the tree around the node N, where Parent is N's parent.  Note that
    * Parent could be null, which means N is the root of the entire tree.  If
    * Parent denotes a node, note that N could be to the left of Parent, or to
    * the right.  You'll need to take all this into account when linking in the
    * new root after the rotation.  Don't forget to update the heights as well.
    *
    *            ParentN                                ParentN
    *              /                                      /
    *             N                     R             childN(L)
    *            /                      =>             /    \
    *        childN(L)                              curr      N
    *          /
    *       curr(newly inserted)
    *
    *
    */
    void _RightRotate(NODE* Parent, NODE* N)
    {
        // Labling
        NODE* L = N->Left;
        NODE* A = L->Left;
        NODE* B = _getActualRight(L);
        NODE* C = _getActualRight(N);

        // Rotation
        L->Right = N;
        if (L->isThreaded){
            L->isThreaded = false;
            B = nullptr;
        }
        N->Left = B;


        // Updating parent to link to L
        if (Parent == nullptr){
            Root = L;
        }else if (N == Parent->Left){
            Parent->Left = L;
        }else{
            Parent->Right = L;
        }
        // Getting children's height
        int HA, HB, HC;

        HA = (A == nullptr)?-1: A->Height;
        HB = (B == nullptr)?-1: B->Height;
        HC = (C == nullptr)?-1: C->Height;

        //Updating Height
        N->Height = 1 + max(HB, HC);
        L->Height = 1 + max(HA, N->Height);
    }


    /*
     *  Rotates the tree around the node N, where Parent is N's parent.  Note that
     *  Parent denotes a node, note that N could be to the left of Parent, or to
     *  the right.  You'll need to take all this into account when linking in the
     *  new root after the rotation.  Don't forget to update the heights as well.
     *
     *     ParentN                                      ParentN
     *         \                                            \
     *          N                        L                childN(R)
     *           \                       =>                /      \
     *           childN(R)                                N       curr
     *             \
     *             curr (newly inserted)
     *
     *
     */
    void _LeftRotate(NODE* Parent, NODE* N)
    {
        // Labeling
        NODE* R = _getActualRight(N);
        NODE* A = N->Left;
        NODE* B = R->Left;
        NODE* C = _getActualRight(R);

        // Rotation
        R->Left = N;
        if(B != nullptr){
            N->Right = B;
        }else{
            N->Right = R;
            N->isThreaded = true;
        }

        // Updating parent link to R
        if (Parent == nullptr){
            Root = R;
        }else if (N == Parent->Left){
            Parent->Left = R;
        }else{
            Parent->Right = R;
        }

        // Getting children's height
        int HA, HB, HC;

        HA = (A == nullptr)?-1: A->Height;
        HB = (B == nullptr)?-1: B->Height;
        HC = (C == nullptr)?-1: C->Height;

        // Updating Heights
        N->Height = 1 + max(HB, HA);
        R->Height = 1 + max(HC, N->Height);
    }


    /* Helper function which calculated the balancing factor of a node. */
    int _BalanceFactor(NODE* node)
    {
        int LH = -1;
        int RH = -1;

        if( node->Left != nullptr){
            LH = node->Left->Height;
        }
        if(node->isThreaded || node->Right == nullptr){
            RH = -1;
        }else{
            RH = node->Right->Height;
        }


        int balanceFactor = LH - RH;

        return balanceFactor;
    }


    /* If a node with a specific key has been found then  it is returned, otherwise nullpointer. */
    NODE* _search(NODE* root, KeyT key) const
    {
        while( root != nullptr){

            // Found
            if( root->Key == key){
                return root;
            }
                // Left
            else if(key < root->Key){
                root = root->Left;
            }
                // Right
            else if(root->Key < key){
                /* Reaching a threaded child meaning the key is not in the tree. */
                if( root->isThreaded){
                    return nullptr;
                }else{
                    root = root->Right;
                }
            }
        }
        return nullptr;
    }


  stack<NODE*> _simpleInsert(KeyT key, ValueT value)
  {
        NODE* cur = this->Root;
        NODE* prev = nullptr;
        stack<NODE*> stack;

        // Insertion into an empty tree.
        if(cur == nullptr){
            cur = new NODE;
            cur->Key = key;
            cur->Value = value;
            cur->Left = nullptr;
            cur->Right = nullptr;
            cur->isThreaded = true;
            cur->Height = 0;
            this->Size++;
            this->Root = cur;
            return stack;
        }
        /* We have at least a root */
        while(cur != nullptr){
            prev = cur;
            //If the key already exists, do no change the tree.
            if(cur->Key == key){
                return stack;
            }
            stack.push(cur);

            if( key < cur->Key){
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
        cur->Height = 0;

        //Whether the parent(prev) is threaded.
        if(prev->isThreaded){
            // Position of the node relevant to the parent
            // Left of the Parent
            if(cur->Key < prev->Key){
                cur->Right = prev;
                cur->isThreaded = true;
             //Right of the parent.
            }else {
                //If the parent of the newly inserted node is root (isThreaded = true)
                if(prev->Key  == this->Root->Key){
                    cur->isThreaded = false;
                    cur->Right = nullptr;
                }else{
                    cur->Right = prev->Right;
                    cur->isThreaded = true;
                }
            }
        }else{
            // Positioning of the node relavqant to the Root.
            // Right of the Root.
            if(cur->Key > this->Root->Key){
                // Left of the parent.
                if(cur->Key < prev->Key){
                    cur->Right = prev;
                    cur->isThreaded = true;
                }else{
                    cur->Right = nullptr;
                    cur->isThreaded = false;
                }
                // Left of the Root.
            }else{
                cur->Right = prev;
                cur->isThreaded = true;
            }
        }


        // Updating the parent to point at the newly created child node.
        if(cur->Key < prev->Key){
            prev->Left = cur;
        }else{
            prev->Right = cur;
            prev->isThreaded = false;
        };

        this->Size++;

        return stack;
  }

  void _removeLeaves(NODE* cur, int& mask){
        if(cur == nullptr){
            mask=1;
            return;
        }
        _removeLeaves(cur->Left, mask);
        if(!cur->isThreaded){
            _removeLeaves(cur->Right,mask);
        }
        if(mask){
            mask = 0;
            Size--;
            delete cur;
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
    avlt()
    {
        this->Root = nullptr;
        this->itr = nullptr;
        this->Size = 0;
    }

    //
    // copy constructor
    //
    // NOTE: makes an exact copy of the "other" tree, such that making the
    // copy requires no rotations.
    //
    /**
    * @note
    * copy constructor
    *
    * @param
    *    A reference to already created obj tree. (lvalue)
    */
    avlt (const avlt& other)
    {
        this->Root = nullptr;
        this->itr = nullptr;
        this->Size = 0;
        _Copy(other.Root);
    }


    void removeLeaves(){
        int mask = 0;
        NODE* cur = Root;
        _removeLeaves(cur,mask);
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
    virtual ~avlt()
    {
        _Delete(this->Root);
        this->Root = nullptr;
        this->Size = 0;
        this->itr = nullptr;
    }


    /**
    * @note
    * Clears "this" tree and then makes a copy of the "other" tree.
    * Makes an exact copy of the "other" tree, such that making the
    * copy requires no rotations.
    *
    * @param
    *  Reference to bstt object of already created tree. (lvalue)
    *
    * @return
    *  Reference (addr)to the (deep) copy of the other tree object.
    */
    avlt& operator=(const avlt& other)
    {
        this->clear();
        _Copy(other.Root);

        return *this;
    }

    /**
    * @note
    * Clears the contents of the tree, resetting the tree to empty.
    */
    void clear()
    {
        _Delete(this->Root);
        this->Root = nullptr;
        this->Size = 0;
        this->itr = nullptr;
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
    *
    * @details
    *      Time complexity:  O(1)
    *
    * @return
    *      the height of the tree, -1 if empty.
    */
    int height() const
    {
        if (Root == nullptr)
        return -1;
        else
        return Root->Height;
    }



    /**
    * @note
    *  Searches the tree for the given key, returning true if found
    *  and false if not.  If the key is found, the corresponding value
    *  is returned via the reference parameter.
    *
    * @details
    *  Time complexity:  O(lgN) worst-case (Binary search)
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
        cur = _search(cur, key);

        if(cur != nullptr){
          value = cur->Value;
          return true;
        }

        return false; //Default statement
    }



    /**
    *
    *  Searches the tree for all keys in the range [lower..upper], inclusive.
    *  It is assumed that lower <= upper.  The keys are returned in a vector;
    *  if no keys are found, then the returned vector is empty.
    *
    *  Time complexity: O(lgN + M), where M is the # of keys in the range
    *  [lower..upper], inclusive.
    *
    *  NOTE: do not simply traverse the entire tree and select the keys
    *  that fall within the range.  That would be O(N), and thus invalid.
    *  Be smarter, you have the technology.
    *
    *
    * @param lower
    * @param upper
    * @return
    */
    vector<KeyT> range_search(KeyT lower, KeyT upper)
    {
        vector<KeyT>  keys;
        KeyT key;
        NODE* tempSaveItr = itr;

        //Getting the lowest key in the tree
        _begin(lower);

        if(itr->Key > upper){
            itr = tempSaveItr;
            return keys;
        }

        while( next(key)){
            if (key > upper){
                itr = tempSaveItr;
                return keys;
            }
            if(key < lower){
                    continue;
            }else{
                keys.push_back(key);
            }
        }


        itr = tempSaveItr;
        return keys;
    }


    /**
    * @note
    * Inserts the given key into the tree; if the key has already been insert then
    * the function returns without changing the tree.  Rotations are performed
    * as necessary to keep the tree balanced according to AVL definition.
    *
    * @details
    * Time complexity:  O(lgN) worst-case
    *
    * Parent Hierarchy:
    *    parentN
    *      \
    *       N
    *        \
    *      childN
    *          \
    *          cur
    *
    * @param key
    *        Copy value of the key being inserted.
    * @param value
    *        Copy value of the value inserted with they key.
    */
    void insert(KeyT key, ValueT value)
    {
        NODE* parentN = nullptr;
        NODE* N = nullptr;
        NODE* childN = nullptr;
        stack<NODE*> stack;

        stack = _simpleInsert(key,value);

        if(stack.empty()){return;}

        //Walking up to update the heights
        _updateHeight(stack, &childN, &parentN, &N);

        //Checking  if any rotations are needed.
        int childNBF, NBF;
        if(parentN != nullptr || N != nullptr) {
          childNBF = _BalanceFactor(childN);
          NBF = _BalanceFactor(N);

          if (NBF >= 2 && childNBF >= 1) {
              _RightRotate(parentN, N);
          } else if (NBF <= -2 && childNBF <= -1) {
              _LeftRotate(parentN, N);
          } else if (NBF >= 2 && childNBF <= -1) {
              _LeftRotate(N, childN);
              _RightRotate(parentN, N);
          } else if (NBF <= -2 && childNBF >= 1) {
              _RightRotate(N, childN);
              _LeftRotate(parentN, N);
          }
        }
    }


    /**
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

        return ValueT{ };
    }


    /**
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

        return KeyT{ };
    }

    /**
    *  %
    *  Return the height stored in the note that contain key, if key is
    *  not found, -1 is returned.
    *
    *  @example
    *  cout << tree%12345 << endl;
    *
    *  @details
    *  Time complexity: O(logN) worst-case
    *
    *  @param key
    *  @return
    */
    int operator%(KeyT key) const
    {

        NODE* cur = this->Root;

        cur = _search(cur,key);

        if(cur != nullptr){
            return cur->Height;
        }

        return -1;
    }

    /**
    * Resets internal state for an inorder traversal.  After the
    * call to begin(), the internal state denotes the first inorder
    * key; this ensure that first call to next() function returns
    * the first inorder key.
    *
    * @details
    * Space complexity: O(1)
    * Time complexity:  O(lgN) worst-case
    *
    * @example
    *    tree.begin();
    *    while (tree.next(key))
    *      cout << key << endl;
    */
    void begin()
    {
        itr = Root;

        if (itr != nullptr)  // advance as left as possible:
        {
          while (itr->Left != nullptr)
              itr = itr->Left;
        }
    }

    /**
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
        if (itr == nullptr)
        {
          key = KeyT{};
          return false;
        }

        //
        // we have at least one more, so grab it now,
        // advance, and return true:
        //
        key = itr->Key;
        if ( _getActualRight(itr) == nullptr )
        {
          // follow the thread:
          itr = itr->Right;
        }
        else
        {
          itr = itr->Right;  // go right, and then left as far as we can:
          while (itr->Left != nullptr)
              itr = itr->Left;
        }
        return true;
    }


    /**
    * Dumps the contents of the tree to the output stream, using a
    * recursive inorder traversal.
    *
    * @param cout
    */
    void dump(ostream& output) const
    {
        output << "**************************************************" << endl;
        output << "********************* AVLT ***********************" << endl;

        output << "** size: " << this->size() << endl;
        output << "** height: " << this->height() << endl;

        //
        // inorder traversal, with one output per line: either
        // (key,value,height) or (key,value,height,THREAD)
        //
        // (key,value,height) if the node is not threaded OR thread==nullptr
        // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
        //

        _Dump(this->Root, output);

        output << "**************************************************" << endl;
    }
};

