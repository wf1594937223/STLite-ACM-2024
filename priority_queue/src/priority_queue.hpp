#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	Compare cmp;
	size_t siz = 0;
	struct node
	{
		T* val = nullptr;
		node *lson = nullptr, *rson = nullptr;
		node() {}
	};
	void swap(node* &x, node* &y)
	{
		node* w = x;
		x = y;
		y = w;
		return;
	}
	node* dfs_copy(node *now, node *co)
	{
		now->val = new T(*(co->val));
		if (co->lson)
		{
			now->lson = new node;
			dfs_copy(now->lson, co->lson);
		}
		if (co->rson)
		{
			now->rson = new node;
			dfs_copy(now->rson, co->rson);
		}
		return now;
	}
	void dfs_del(node *now)
	{
		if (now->val != nullptr) delete now->val;
		if (now->lson)
		{
			dfs_del(now->lson);
			delete now->lson;
		}
		if (now->rson)
		{
			dfs_del(now->rson);
			delete now->rson;
		}
		return;
	}
	node* merg(node* x, node* y)
	{
		if ((x == nullptr || x->val == nullptr) && (y == nullptr || y->val == nullptr))
			return nullptr;
		if (x == nullptr || x->val == nullptr) return y;
		if (y == nullptr || y->val == nullptr) return x;
		if (!cmp(*(y->val), *(x->val))) swap(x, y);
		x->rson = merg(x->rson, y);
		swap(x->lson, x->rson);
		return x;
	}
public:
	node* head = nullptr;
	/**
	 * TODO constructors
	 */
	priority_queue() {}
	priority_queue(const priority_queue &other)
	{
		if (this != &other)
		{
			siz = other.siz;
			if (other.head)
			{
				head = new node;
				dfs_copy(head, other.head);
			}
		}
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue()
	{
		if (head) dfs_del(head);
		if (head) delete head;
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other)
	{
		if (this == &other) return *this;
		if (head != nullptr)
		{
			dfs_del(head);
			delete head;
		}
		siz = other.siz;
		if (other.head)
		{
			head = new node;
			dfs_copy(head, other.head);
		}
		else head = nullptr;
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const
	{
		if (head == nullptr || head->val == nullptr) throw(container_is_empty());
		return (*head->val);
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e)
	{
		node* tmp = new node;
		tmp->val = new T(e);
		head = merg(head, tmp);
		siz++;
		return;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop()
	{
		if (head == nullptr || head->val == nullptr) throw(container_is_empty());
		delete head->val; node *tmp1 = head->lson, *tmp2 = head->rson;
		delete head;
		head = merg(tmp1, tmp2);
		siz--;
		return;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {return siz;}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {return (siz == 0);}
	/**
	 * merge two priority_queues with at most O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other)
	{
		head = merg(head, other.head);
		siz += other.siz;
		other.siz = 0;
		other.head = nullptr;
		return;
	}
};

}

#endif
