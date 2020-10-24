/** Name: Mathew Soto
  * Course: CSCI 335
  * Instructor: Pavel Shostak
  *
  * my_memory.h contains two class templates,
  * my_unique_ptr and my_shared_ptr, meant to
  * mimic the functionality of the smart
  * pointers unique_ptr and shared_ptr in the
  * C++ standard library
  *
  * specifically, my_unique_ptr should allow
  * only one object of its type to point to a
  * given block of memory 
  * 
  * my_shared_ptr, in addition to the pointer
  * to the block of memory, also has a pointer
  * to a counter keeping track of how many
  * objects of its type are pointing to that
  * block of memory
  */

#include <iostream>

template <typename Type>
class my_unique_ptr 
{
  public:

/**default constructor
 *@post initializes the object to point to nullptr
 */
my_unique_ptr() {
	my_ptr_ = nullptr;
}

/**constructor
 *@param a pointer to an object of type Type
 *@post sets the object to point to the memory pointed to by the parameter
 */ 
my_unique_ptr(Type * type_pointer) {
	my_ptr_ = type_pointer;
}

/**move constructor
 *@param an object of type my_unique_ptr<Type>
 *@post constructs new my_unique_ptr<Type> object pointing to the same block of memory as the parameter
 */
my_unique_ptr(my_unique_ptr<Type> && type_ptr) {
	my_ptr_ = type_ptr.my_ptr_;
	type_ptr.my_ptr_ = nullptr;
}

/**move assignment
 *@param an object of type my_unique_ptr<Type>
 *@post current object points to where type_ptr points before the function is called
 *@post the block of memory owned by current object before the function is called is deallocated
 *@return reference to current object
 */
my_unique_ptr & operator=(my_unique_ptr<Type> && type_ptr) {
	if (my_ptr_ == nullptr) {
		my_ptr_ = type_ptr.my_ptr_;
		type_ptr.my_ptr_ = nullptr;
	}
	else {
		Type * temp = my_ptr_;
		my_ptr_ = type_ptr.my_ptr_;
		type_ptr.my_ptr_ = nullptr;
		delete temp;
	}

	return *this;
}

/**disable copy constructor and copy assignment
 * this is so that only one object of type my_unique_ptr<Type>
 * points to a given block of memory
 */
my_unique_ptr(const my_unique_ptr<Type> & type_ptr) = delete;
my_unique_ptr & operator=(const my_unique_ptr<Type> & type_ptr) = delete;

/**destructor**/
~my_unique_ptr() {
	if (my_ptr_ != nullptr) {
		delete my_ptr_;
	}
}

/**dereference operator
 *@return reference to data stored in the block of memory pointed to by this object
 */
Type & operator*() const {
	return *my_ptr_;
}

/**operator allowing my_shared_ptr<Type> to be used for class member access
 *@return pointer to the block of memory owned by this object
 */
Type * operator->() const {
	return my_ptr_;
}

/**tests if the pointer of this object is a nullptr**/
bool isNullptr() const {
	if (my_ptr_ == nullptr) {
		return true;
	}
	return false;
}

  private:
/**pointer to the block of memory**/
Type * my_ptr_;

};



template<typename Type>
class my_shared_ptr
{
  public:

/**default constructor
 *@post initializes the object to point to nullptr
 *@post the counter is set to nullptr
 */
my_shared_ptr() {
	my_ptr_ = nullptr;
	int_ptr_ = nullptr;
}

/**constructor
 *@param a pointer to an object of type Type
 *@post sets the object to point to the memory pointed to by the parameter
 *@post the counter is set to nullptr if the parameter is a nullptr
 *@post the counter is set 1 if the parameter points to a block of memory
 */
my_shared_ptr(Type * type_ptr) {
	my_ptr_ = type_ptr;
	int_ptr_ = nullptr;
	if (my_ptr_ != nullptr) {
		int_ptr_ = new int {1};
	}
}

/**copy constructor
 *@param an object of type my_shared_ptr<Type>
 *@post creates object to point to the memory pointed to by the parameter
 *@post if the pointed to memory is not a nullptr, then the counter increments by 1
 *       this is to reflect that one more object is pointing to the given block of memory
 */
my_shared_ptr(const my_shared_ptr<Type> & type_ptr) {
	my_ptr_ = type_ptr.my_ptr_;
	int_ptr_ = type_ptr.int_ptr_;

	if (my_ptr_ != nullptr) {
		(*int_ptr_)++;
	}
}

/**move constructor
 *@param an object of type my_shared_ptr<Type>
 *@post creates object to point to the memory pointed to by the parameter
 *@post the parameter's counter becomes the current object's counter
 *@post the parameter's pointer to the block of memory and counter are set to nullptr
 *       so that the destructor doesn't deallocate the memory pointed to by the
 *       current object after the move constructor
 */
my_shared_ptr(my_shared_ptr<Type> && type_ptr) {
	my_ptr_ = type_ptr.my_ptr_;
	int_ptr_ = type_ptr.int_ptr_;
	type_ptr.my_ptr_ = nullptr;
	type_ptr.int_ptr_ = nullptr;	
}

/**copy assignment
 *@param an object of type my_shared_ptr<Type>
 *@post object points to the memory pointed to by the parameter
 *@post if the pointed to memory is not a nullptr, the counter increments by 1
 *       just as in the copy constructor, this indicates that another object
 *       points to the given block of memory
 *@post creates an object within the scope of this function with the same state
 *       as *this before the copy assignment so that the destructor takes care of
 *       the memory of *this before the assignment
 */
my_shared_ptr<Type> & operator=(const my_shared_ptr<Type> & type_ptr) {
	my_shared_ptr<Type> temp = *this;
	if (temp.int_ptr_ != nullptr) {
		(*(temp.int_ptr_))--;
	} //since copy constructor increases counter by 1 if my_ptr!=nullptr,
          //this if statement ensures that temp has the same state as *this
          //before the copy assignment

	my_ptr_ = type_ptr.my_ptr_;
	int_ptr_ = type_ptr.int_ptr_;
	if (int_ptr_ != nullptr) {
		(*int_ptr_)++;
	}

	return *this;
}

/**move assignment
 *@param an object of type my_shared_ptr<Type>
 *@post object points to the memory pointed to by the parameter
 *@post creates an object within the scope of this function with the same state
 *       as *this before the copy assignment so that the destructor takes care of
 *       the memory of *this before the assignment
 *@post the parameter's pointer to the block of memory and counter are set to nullptr
 *       so that the destructor doesn't deallocate the memory pointed to by the
 *       current object after the move constructor
 */
my_shared_ptr<Type> & operator=(my_shared_ptr<Type> && type_ptr) {
	my_shared_ptr<Type> temp = *this;
	if (int_ptr_ != nullptr) {
		(*(temp.int_ptr_))--;
	} //just as in the copy assignment, since copy constructor increases
          //counter by 1 if my_ptr!=nullptr, this if statement ensures that
          //temp has the same state as *this before the copy assignment

	my_ptr_ = type_ptr.my_ptr_;
	int_ptr_ = type_ptr.int_ptr_;
	type_ptr.my_ptr_ = nullptr;
	type_ptr.int_ptr_ = nullptr;

	return *this;
}

/**destructor**/
~my_shared_ptr() {
	if (int_ptr_ != nullptr) {
		if (*int_ptr_ == 1) {//shared_ptr is the only object pointing
                                     //to the given block of memory
			delete my_ptr_;
			delete int_ptr_;
		}
		else {//there are other objects pointing to the same
                      //block of memory, so we decrement counter
			(*int_ptr_)--;
		}
	}
}
 
/**dereference operator
 *@return reference to data stored in the block of memory pointed to by this object
 */
Type & operator*() {
	return *my_ptr_;
}


/**operator allowing my_shared_ptr<Type> to be used for class member access
 *@return pointer to the block of memory owned by this object
 */
Type * operator->() {
	return my_ptr_;
}

/**tests if the pointer of this object is a nullptr**/
bool isNullptr() {
	if (my_ptr_ == nullptr) {
		return true;
	}
	return false;
}

  private:
/**pointer to the block of memory**/
Type * my_ptr_;

/**pointer to the counter keeping track of how many objects point to a given block of memory**/
int * int_ptr_;

};