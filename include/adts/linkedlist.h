#ifndef ___LINKEDLIST_H_
#define ___LINKEDLIST_H_

#include <iostream>

namespace myadts 
{ 
    // singly linked list
    template <class T>
    class SLinkedList
    {
    protected:
        // private class with forward declaration
        class SNode
        {
        public:
            T data;
            SNode *next;
        };
      
        int _size;
        SNode *head;
        SNode *tail;
    
    public:
        SLinkedList() { _size = 0; head = tail = NULL; }
        virtual ~SLinkedList()
        {
            SNode *cur = head, *nodeToDelete;
            while(cur)
            {
                nodeToDelete = cur;
                cur = cur->next;
                delete nodeToDelete;
            }
        }
        
        int size() const { return _size; }
        
        // insert element at head of list
        virtual void push_front(T& val)
        {
            SNode* pNode = new SNode;
            pNode->data = val;
            if(!head) // empty list
            {
                pNode->next = NULL;
                tail = pNode;
            }
            else
            {
                pNode->next = head;
            }
            head = pNode;
            ++_size;
        }
        
        // insert element at tail of list
        virtual void push_back(T& val)
        {
            SNode* pNode = new SNode;
            pNode->data = val;
            pNode->next = NULL;
            if(!tail) // empty list
            {
                head = pNode;
            }
            else
            {
                tail->next = pNode;
            }
            tail = pNode;
            ++_size;
        }
        
        // remove element from head of list
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual void pop_front(T** pVal=NULL)
        {
            if(head)
            {
                if(pVal)
                    *pVal = new T(head->data);
                
                SNode* nodeToDelete = head;
                head = head->next;
                if(head == NULL) tail = NULL;
                delete nodeToDelete;
                --_size;
            }
            else
            {
                if(pVal)
                    *pVal = NULL;
            }
        }
        
        // remove element from tail of list
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual void pop_back(T** pVal=NULL)
        {
            if(tail)
            {
                if(pVal)
                    *pVal = new T(tail->data);
                
                SNode* newLastNode = head; // this is definitely existing as tail is valid
                SNode* nodeToDelete = tail;
                if(newLastNode->next) // more than one node
                {
                    while(newLastNode->next->next) // iterate until newLastNode is pointing to second-last node (the next node will be == tail)
                        newLastNode = newLastNode->next;
                        
                    tail = newLastNode;
                }
                else // only one node
                {
                    head = tail = NULL;
                }
                delete nodeToDelete;
                --_size;
            }
            else
            {
                if(pVal)
                    *pVal = NULL;
            }
        }
            
        // insert element at position <pos>, return true if succeeded, false if failed (cannot insert more than size()), 
        //  if pos == size(), then it will be equivalent to push_back()
        //  if pos == 0, then it will be equivalent to push_front()
        virtual bool insertAt(int pos, T& val)
        {
            if(pos > _size) return false;
            
            // case where the list is empty AND pos == 0 is a subset of the first condition of following if..else branch
            // if the list is empty AND pos > 0, it would have failed the preceding 'if' check
            if(pos == 0) push_front(val);
            else
            {
                // if we reach this branch, there must at least one node AND we are inserting in a pos >= 1
                SNode *prev;
                int curPos = 1;
                prev = head;
                while(curPos != pos) { prev = prev->next; ++curPos; }
                SNode* pNode = new SNode;
                pNode->data = val;
                pNode->next = prev->next;
                prev->next = pNode;
                if(!pNode->next) tail = pNode;
                ++_size;
            }
            
            return true;
        }
            
        // remove element from position <pos>, return true if succeeded, false if failed (cannot remove from if pos > size() - 1), 
        //  if pos == size() - 1, then it will be equivalent to pop_back()
        //  if pos == 0, then it will be equivalent to pop_front()
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual bool removeFrom(int pos, T** pVal=NULL)
        {
            if(pos > _size - 1) return false;
            
            // case where the list has only 1 element is a subset of the first condition of following if..else branch
            // if the list is empty and pos >= 0, it would have failed the preceding 'if' check
            if(pos == 0) pop_front(pVal);
            else
            {
                // if we reach this branch, there must at least two nodes AND we are removing from a pos >= 1
                SNode *prev;
                int curPos = 1;
                prev = head;
                while(curPos != pos) { prev = prev->next; ++curPos; }
                SNode* nodeToDelete = prev->next;
                *pVal = new T(nodeToDelete->data);
                prev->next = nodeToDelete->next;
                delete nodeToDelete;
                if(!prev->next) tail = prev;
                --_size;
            }
            
            return true;
        }
        
        // linear search for existence of element passed in, returns true if exists, and pass out the position if a valid address is passed in and element is found
        virtual bool exist(T& val, int *pPos=NULL)
        {
            SNode* cur = head;
            int pos = 0;
            while(cur && cur->data != val) { cur = cur->next; ++pos; }
            if(!cur) return false;
            else
            {
                *pPos = pos;
                return true;
            }
        }
        
        // remove element if found
        virtual bool remove(T& val)
        {
            int pos;
            if(exist(val, &pos)) return removeFrom(pos); // should be always true
            else return false;
        }
        
        // return true if empty
        bool empty() { return _size == 0 ? true : false; }
        
        friend std::ostream& operator<<(std::ostream& os, const SLinkedList<T>& list)
        {
            int pos = 0;
            os << "Size of List(" << &list << ") : " << list.size() << "\n";
            SNode* cur = list.head;
            while(cur)
            {
                os << pos << " : " << cur->data << "\n";
                cur = cur->next;
                ++pos;
            }
            os << std::endl;
            return os;
        }
    };
    #if 0
    // doubly linked list
    template <class T>
    class DLinkedList : public SLinkedList<T>
    {
    protected:
        // private class with forward declaration
        class DNode : SNode
        {
        public:
            SNode *prev;
        };
    
    public:        
        // insert element at head of list
        virtual void push_front(T& val)
        {
            DNode* pNode = new DNode;
            pNode->data = val;
            pNode->prev = NULL;
            if(!head) // empty list
            {
                pNode->next = NULL;
                tail = pNode;
            }
            else
            {
                head->prev = pNode;
                pNode->next = head;
            }
            head = pNode;
            ++_size;
        }
        
        // insert element at tail of list
        virtual void push_back(T& val)
        {
            DNode* pNode = new DNode;
            pNode->data = val;
            pNode->next = NULL;
            if(!tail) // empty list
            {
                pNode->prev = NULL;
                head = pNode;
            }
            else
            {
                pNode->prev = tail;
                tail->next = pNode;
            }
            tail = pNode;
            ++_size;
        }
        
        // remove element from head of list
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual void pop_front(T** pVal=NULL)
        {
            if(head)
            {
                if(pVal)
                    *pVal = new T(head->data);
                
                DNode* nodeToDelete = head;
                head = head->next;
                if(head == NULL) tail = NULL;
                else head->prev = NULL;
                delete nodeToDelete;
                --_size;
            }
            else
            {
                if(pVal)
                    pVal = NULL;
            }
        }
        
        // remove element from tail of list
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual void pop_back(T** pVal=NULL)
        {
            if(tail)
            {
                if(pVal)
                    *pVal = new T(tail->data);
                
                DNode* nodeToDelete = tail;
                if(tail->prev) // more than 1 node before delete
                {
                    tail = tail->prev;
                    tail->next = NULL
                }
                else // 1 node only before delete
                    head = tail = NULL;
                
                delete nodeToDelete;
                --_size;
            }
            else
            {
                if(pVal)
                    pVal = NULL;
            }
        }
            
        // insert element at position <pos>, return true if succeeded, false if failed (cannot insert more than size()), 
        //  if pos == size(), then it will be equivalent to push_back()
        //  if pos == 0, then it will be equivalent to push_front()
        virtual bool insertAt(int pos, T& val)
        {
            if(pos > _size) return false;
            
            // case where the list is empty AND pos == 0 is a subset of the first condition of following if..else branch
            // if the list is empty AND pos > 0, it would have failed the preceding 'if' check
            if(pos == 0) push_front(val);
            else if(pos == _size) push_back(val);
            else
            {
                // if we reach this branch, there must at least one node AND we are inserting in a pos >= 1 OR pos < size()
                int mid = _size / 2;
                int curPos;
                DNode* pNode = new DNode;
                pNode->data = val;
                pNode *prev, *post;
                if(pos <= mid) // forward traversal
                {
                    curPos = 1;
                    prev = head;
                    while(curPos != pos) { prev = prev->next; ++curPos; }
                    post = prev->next;
                }
                else
                {
                    curPos = _size - 1;
                    post = tail;
                    while(curPos != pos) { post = post->prev; --curPos; }
                    prev = post->prev;
                }
                
                pNode->prev = prev;
                pNode->next = post;
                if(prev) prev->next = pNode;
                if(post) post->prev = pNode;
                if(!pNode->prev) head = pNode;
                if(!pNode->next) tail = pNode;
            }
            
            return true;
        }
            
        // remove element from position <pos>, return true if succeeded, false if failed (cannot remove from if pos > size() - 1), 
        //  if pos == size() - 1, then it will be equivalent to pop_back()
        //  if pos == 0, then it will be equivalent to pop_front()
        // if input argument is NULL, do nothing else
        // else allocate a copy of T and pass out the address (caller must deallocate it)
        virtual bool removeFrom(int pos, T** pVal=NULL)
        {
            if(pos > _size - 1) return false;
            
            // case where the list has only 1 element is a subset of the first condition of following if..else branch
            // if the list is empty and pos >= 0, it would have failed the preceding 'if' check
            if(pos == 0) pop_front(pVal);
            else if(pos == _size - 1) pop_back(pVal);
            else
            {
                // if we reach this branch, there must at least two nodes AND we are removing from a pos >= 1 or pos < _size-1
                int mid = _size / 2;
                DNode *prev, *post, *nodeToDelete;
                int curPos;
                
                if(pos <= mid) // forward traversal
                {
                    curPos = 1;
                    prev = head;
                    while(curPos != pos) { prev = prev->next; ++curPos; }
                    nodeToDelete = prev->next;
                    post = nodeToDelete->next;
                }
                else
                {
                    curPos = _size - 2;
                    post = tail;
                    while(curPos != pos) { post = post->prev; --curPos; }
                    nodeToDelete = post->prev;
                    prev = nodeToDelete->prev;
                }
                
                if(prev) prev->next = post;                
                if(post) post->prev = prev;
                
                *pVal = new T(nodeToDelete->data);
                delete nodeToDelete;
                --_size;
            }
            
            return true;
        }
        
        // linear search for existence of element passed in, returns true if exists, and pass out the position if a valid address is passed in and element is found
        virtual bool exist(T& val, int *pPos=NULL)
        {
            DNode* cur = head;
            int pos = 0;
            while(cur && cur->data != val) { cur = cur->next; ++pos; }
            if(!cur) return false;
            else
            {
                *pPos = pos;
                return true;
            }
        }
    };
    #endif
}

#endif // ___LINKEDLIST_H_