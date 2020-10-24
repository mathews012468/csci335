/** Name: Mathew Soto
  * Course: CSCI 335
  * Instructor: Pavel Shostak
  *
  * FastQueue.h implements a queue using a vector container
  */

#include <vector>
#include <stdexcept>
#include <iostream>

template <typename Object>
class FastQueue
{
  public:

/**constructor, no arguments
 *@post the index of the front element of the queue is set to 0
 *@post the size of the queue is set to 0
 *@post the capacity of the queue is set to 10 
 */
FastQueue() {
	start_ = 0;
	size_ = 0;
	MakeItN(10);
}

/**adds an element to the back of the queue
 *@param element can be any item of type Object
 *@post capacity of the queue increases to 10 if current capacity is 0
 *@post capacity of the queue doubles if the size of the queue is equal to the capacity when enqueue is called
 *@post size of the queue increases by 1
 */
void enqueue(const Object & element) {
	if (queue_.size() == 0) {
		MakeItN(10);
	}

	if (size_ == queue_.size()) {
		MakeItN(2*size_);
	}

	queue_[(start_+size_)%(queue_.size())] = element;
	size_++;
}

/**removes an element from the front of the queue if queue is nonempty
 *@post index of the front element is shifted up by 1 modulo the capacity of the queue
 *@post size of the queue decreases by 1 if queue is nonempty
 *@post if queue is empty as a result of dequeueing, index of the front element of the queue is set to 0
 *@post if queue is empty before dequeue(), nothing happens other than setting start_ = 0
 */
void dequeue() {
	if (size_ != 0) {
		start_ = (start_ + 1)%(queue_.size());
		size_--;
	}

	if (size_ == 0) {
		start_ = 0;
	}
}

/**get the front element of the queue
 *@return constant reference to the front element of the queue
 *throws an out_of_range exception if the queue is empty
 */
const Object & front() const {
	if (size_ == 0) {
		throw std::out_of_range("the queue is empty");
	}

	return queue_[start_];
}

/**get the last element of the queue
 *@return constant reference to the last element of the queue
 *throws an out_of_range exception if the queue is empty
 */
const Object & back() const {
	if (size_ == 0) {
		throw std::out_of_range("the queue is empty");
	}

	return queue_[(start_+size_-1)%(queue_.size())];
}

/**get i-th element of the queue
 *@param integer containing the desired index, integer should be between 0 and size_-1, inclusive
 *@return constant reference to the i-th element of the queue
 *throws an out_of_range exception if index isn't between 0 and size_-1, inclusive
 */
const Object & at(int index) const {
	if (size_-1 < index || index < 0) {
		throw std::out_of_range("unacceptable index");
	}
	return queue_[(start_ + index)%(queue_.size())];
}

/**get size of the queue
 *@return integer containing the size of the queue
 */
int size() const {
	return size_;
}

/**get capacity of the queue
 *@return integer containing the capacity of the queue
 */
int capacity() const {
	return queue_.size();
}

/**shrinks the capacity of the queue to the size of the queue
 *@post size_ == queue_.size()
 *@post the size of the queue and the elements of the queue are not changed
 */
void shrink_to_fit() {
	if (size_ == 0) {
		queue_.clear();
		start_ = 0;
	}
	else if (size_ != queue_.size()) {
		MakeItN(size_);
	}
}

/**adds the elements of a vector to the back of the queue
 *@param vector containing items of type Object
 *@post all of the elements in the vector are appended to the back of the queue
 *@post size of the queue increases by the size of the vector
 */
void bigEnqueue(const std::vector<Object> & added) {
	int size_added = added.size();
	for (int i = 0; i < size_added; i++) {
		enqueue(added[i]);
	}
}

  private:

/**changes the capacity of queue to n
 *@param integer determining the capacity of queue, needs to be at least the size of queue
 *@post queue's capacity becomes n, the size and elements of queue do not change
 */
void MakeItN(int n) {
	std::vector<Object> temp(n);
	for (int i = 0; i < size_; i++) {
		temp[i] = queue_[(start_+i)%queue_.size()];
	}
	start_ = 0;

	std::swap(queue_,temp);
}

/**the queue**/
std::vector<Object> queue_;

/**index of starting element**/
int start_;

/**size of queue**/
int size_;

};