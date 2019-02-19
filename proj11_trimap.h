/*
Nicholas Bravata
Project 11 Linked List
Section 003
12/6/2017
*/
#ifndef SINGLELINK_H
#define SINGLELINK_H

#include<iostream>
using std::ostream; using std::cout; using std::endl;
#include<sstream>
using std::ostringstream;
#include<utility>
using std::swap;
#include<string>
using std::string;

// forward declaration so we can make Element a friend of TriMap
template<typename K, typename V>
class TriMap;

/* private Element, friend of TriMap class to remember a pointer that walks
down the list to copy, as tail_ walks down the new list.
Make a new node (copy the current node of the list we are copying),
update the tail_->nex */
template <typename K,typename V>
class Element{

private:
  K key_;
  size_t index_ = 0;
  Element *next_ = nullptr;

public:
  V value_;

  Element() = default;
  Element(K key, V val, size_t i) : key_(key),index_(i), next_(nullptr), value_(val) {};

  // *defined* in the class, makes templating easier.
  friend ostream& operator<<(ostream& out, Element& e){
    out << e.key_ << ":" << e.value_ << ":" << e.index_;
    return out;
  }
  friend class TriMap<K,V>;
};


template<typename K, typename V>
class TriMap{

private:
  Element<K,V> *head_ = nullptr;
  Element<K,V> *tail_ = nullptr;
  size_t size_ = 0;

  // a private function, used by operator<<
  void print_list(ostream& out);

public:
  TriMap() = default;
  TriMap(K, V);
  TriMap(const TriMap&);
  TriMap& operator=(TriMap);
  ~TriMap();
  long size();
  bool insert(K,V);
  bool remove(K);
  Element<K,V>* find_key(K);
  Element<K,V>* find_value(V);
  Element<K,V>* find_index(size_t);

  // *defined* in the class, makes templating easier.
  // uses private method print_list
  // you can remove print_list and just do it here if you
  // prefer. I won't test print_list
  friend ostream& operator<<(ostream& out, TriMap<K,V>& sl){
    sl.print_list(out);
    return out;
  };
};

//Constructor for TriMap given a Key and Value
template<typename K, typename V>
TriMap<K,V>::TriMap(K key, V value){
  Element<K,V>* e = new Element<K,V>(key, value, 0);
  head_ = e;
  tail_ = e;
  size_ = 1;
};
/*
bit of work. we need to remember a pointer that walks
down the list to copy, as tail_ walks down the new list.
Make a new node (copy the current node of the list we are copying),
update the tail_->next_ to point to the new node, update tail_ to new node
 */

//Copy constructor given another TriMap given by bill punch 21.2 Example code
template<typename K, typename V>
TriMap<K,V>::TriMap(const TriMap& tm){
  if (tm.head_ == nullptr){
    head_ = nullptr;
    tail_ = nullptr;
  }
  else{
    head_ = new Element<K,V>(tm.head_->key_,tm.head_->value_);
    tail_ = head_;
    Element<K,V>* tm_ptr = tm.head_->next_;
    Element<K,V>* new_node;
    while(tm_ptr != nullptr){
      new_node = new Element<K,V>(tm_ptr);
      tail_->next_ = new_node;
      tm_ptr = tm_ptr->next_;
      tail_ = new_node;
    }
  }
}

//= Constructor for TriMap given by bill punch 21.2 Example code
template<typename K, typename V>
TriMap<K,V>& TriMap<K,V>::operator=(TriMap tm){
    swap(head_, tm.head_);
    swap(tail_, tm.tail_);
    return *this;
}


// walk down the list using head_ , keeping a trailer pointer called to_del
// delete what to_del points to
// move head_ and to_del to the next node.
template<typename K, typename V>
TriMap<K,V>::~TriMap(){
  Element<K,V>* to_del = head_;
      while (to_del != nullptr){
  	head_ = head_->next_;
  	delete to_del;
  	to_del = head_;
      }
      head_ = nullptr;
      tail_ = nullptr;
}
// Returns size of the TriMap which is the ammount of elements
template<typename K, typename V>
long TriMap<K,V>::size(){
    return size_;
}

//checks the trimap to see if the key already exists if it does returns false and does nothing
//If key doesnt exist it inserts the Element into the trimap in key order.
template<typename K, typename V>
bool TriMap<K,V>::insert(K key, V value){
  // very important, what are all the cases here!!!!
  // elements are inserted in *key order*
  if(find_key(key) == nullptr) {
    if(head_ == nullptr){
      Element<K,V>* e = new Element<K,V>(key, value, 0);
      head_ = e;
      tail_ = e;
      size_ = 1;
      return true;
    }
    else{
      Element<K,V>* current = head_;
      Element<K,V>* last;
      size_t index = 0;
      while(index != size_ - 1 and current->key_ < key){
        last = current;
        current = current->next_;
        index++;
      }
      if(current == head_){
        Element<K,V>* to_add =new Element<K,V>(key,value, size_);
        to_add->next_ = head_;
        head_ = to_add;
        size_ += 1;
        return true;
      }
      else if(current == tail_){
        Element<K,V>* to_add = new Element<K,V>(key,value, size_);
        if(current->key_ > key){
          last->next_ = to_add;
          to_add->next_ = current;
          size_ += 1;
          return true;
        }
        else{
          tail_->next_ =  to_add;
          tail_ = to_add;
          size_ += 1;
          return true;
        }

      }
      else{
        Element<K,V>* to_add =new Element<K,V>(key,value, size_);
        current->next_ = to_add;
        to_add->next_ = current->next_;
        size_ += 1;
        return true;
      }
    }
  }
  return false;

}

//Checks to see if key exists in the trimap. If so it removes the key and lowers the index of the elements by 1 for elements greater than the index of removed key
template<typename K, typename V>
bool TriMap<K,V>::remove(K key){
  // again, what are all the cases!!!
  // also, if you delete a node, the indicies of all
  // nodes with a higher index have that index reduced by one
  size_t index = 0;
 if(find_key(key) == nullptr){
   return false;
 }
 else{
   Element<K,V>* current = head_;
   Element<K,V>* last;
   bool found = false;
   while(found == false and current != nullptr){
     if(current == tail_){
       index = current->index_;
       last-> next_ = nullptr;
       tail_ = last;
       delete current;
       found = true;
     }
     if(current->key_ == key){
       if(current == head_){
         index = current->index_;
         head_ = head_->next_;
         delete current;
         found = true;
         break;
      }
       last->next_ = current->next_;
       index = current->index_;
       delete current;
       found = true;
     }
     else{
       last = current;
       current = current->next_;
     }
   }
   current = head_;
   while(current != nullptr){
     if(current->index_ > index){
       current->index_ = current->index_ - 1;
     }
     current = current->next_;
   }
   size_--;
   return true;
 }

}

//Searches the trimap for a key and returns a Element*
template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_key(K key){
  // no binary search, just linear search
  if(head_ == nullptr){
    return nullptr;
  }
  Element<K,V>* current = head_;
  while(current != tail_){
    if(current == nullptr){
      break;
    }
    if(current->key_ == key){
      return current;
    }
    current = current->next_;
  }
  if(tail_->key_ == key){
    return tail_;
  }
  return nullptr;
}

//Searches the trimap for a value and returns a Element*
template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_value(V val){
  // linear search
  if(head_ == nullptr){
    return nullptr;
  }
  Element<K,V>* current = head_;
  while(current != tail_){
    if(current == nullptr){
      break;
    }
    if(current->value_ == val){
      return current;
    }
    current = current->next_;
  }
  if(tail_->value_ == val){
    return tail_;
  }
  return nullptr;
}

//Searches the trimap for a index and returns a Element*
template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_index(size_t i){
  // linear search
  if(head_ == nullptr){
    return nullptr;
  }
  Element<K,V>* current = head_;
  while(current != tail_){
    if(current == nullptr){
      break;
    }
    if(current->index_ == i){
      return current;
    }
    current = current->next_;
  }
  if(tail_->index_ == i){
    return tail_;
  }
  return nullptr;
}

//Standard print function for a Trimap
template<typename K, typename V>
void TriMap<K,V>::print_list(ostream &out){
  // this is a class member function, has access
  // to private data. Called by operator<<
  Element<K,V>* current = head_;
  ostringstream s;
  if(size_ == 1){
    s << *head_;
  }
  else{
    while(current != tail_){
      s << *current << ", ";
      current = current->next_;
    }
    s << *current;
  }

  string t = s.str();
  out << t;
};

#endif
