#include "../include/binominal_queue.h"

void binominal_queue::swap(binominal_node& a, binominal_node& b) {
	binominal_node temp = a;
	a = b;
	b = temp;
}

binominal_node* binominal_queue::merge_trees(binominal_node* b1, binominal_node* b2)
{

	if (b1->key < b2->key)
		swap(*b1, *b2);
	
	b2->parent = b1;
	
	b2->next = b1->child;
	
	b1->child = b2;
	
	b1->degree++;
	return b1;
}

void binominal_queue::merge_trees_heap()
{
	binominal_node* current_node = _head;
	binominal_node* next_node;
	_head = nullptr;

	binominal_node* trees[MAX_DEGREE_OF_QUEUE] = { nullptr };

	while (current_node != nullptr)
	{
		next_node = current_node->next;
		current_node->next = nullptr;
		int degree = current_node->degree;

		while (trees[degree] != nullptr)
		{
			current_node = merge_trees(current_node, trees[degree]);
			trees[degree] = nullptr;
			degree++;
		}
		trees[degree] = current_node;
		current_node = next_node;
	}

	for (int i = 0; i < MAX_DEGREE_OF_QUEUE; ++i)
	{
		if (trees[i] != nullptr)
		{
			if (_head == nullptr || trees[i]->key > _head->key)
			{
				trees[i]->next = _head;
				_head = trees[i];
			}
			else
			{
				trees[i]->next = _head->next;
				_head->next = trees[i];
			}
		}
	}
}

priority_queue* binominal_queue::merge(priority_queue* q)
{
	binominal_queue* cast_q = dynamic_cast<binominal_queue*>(q);

	binominal_node* new_head = nullptr;
	binominal_node** current_node = &new_head;
	binominal_node* h1 = _head;
	binominal_node* h2 = cast_q->_head;

	while (h1 != nullptr && h2 != nullptr)
	{
		if (h1->degree <= h2->degree)
		{
			*current_node = h1;
			h1 = h1->next;
		}
		else
		{
			*current_node = h2;
			h2 = h2->next;
		}

		current_node = &((*current_node)->next);
	}

	if (h1 != nullptr)
		*current_node = h1;
	if (h2 != nullptr)
		*current_node = h2;

	_head = new_head;

	merge_trees_heap();

	cast_q->_head = nullptr;
	return this;
}

void binominal_queue::insert(const char* data, int key)
{
	binominal_queue new_queue;
	binominal_node* new_node = new binominal_node(key, 0, const_cast<char*>(data), nullptr, nullptr, nullptr);
	new_queue._head = new_node;
	merge(&new_queue);
}

char* binominal_queue::find_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");
	return _head->data;
}

char* binominal_queue::remove_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");

	binominal_node* max_priority_node = _head;
	_head = _head->next;

	binominal_queue newHeap;
	binominal_node* child = max_priority_node->child;
	while (child != nullptr) 
	{
		binominal_node* next = child->next;
		child->parent = nullptr;
		child->next = newHeap._head;
		newHeap._head = child;
		child = next;
	}

	char* result = max_priority_node->data;
	char* data_copy = new char[std::strlen(result) + 1];
	std::strcpy(data_copy, result);
	delete[] result;
	delete max_priority_node;

	merge(&newHeap);

	return data_copy;
}

void binominal_queue::delete_tree(binominal_node* node) {
	if (node == nullptr) return;
	delete_tree(node->child);
	delete_tree(node->next);
	delete node;
}

binominal_queue::~binominal_queue() {
	delete_tree(_head);
	_head = nullptr;
}

binominal_queue& binominal_queue::operator= (const binominal_queue& arg)
{
	if (this != &arg) { 
		delete_tree(_head);
		_head = deep_copy(arg._head);
	}
	return *this;
}

binominal_queue::binominal_queue(const binominal_queue& arg)
{
	_head = deep_copy(arg._head);
}

binominal_node* binominal_queue::deep_copy(const binominal_node* arg)
{
	if (arg == nullptr) return nullptr;

	binominal_node* new_arg = new binominal_node(
		arg->key,
		arg->degree,
		arg->data,
		nullptr,
		deep_copy(arg->child),
		deep_copy(arg->next)  
	);

	if (new_arg->child != nullptr) {
		new_arg->child->parent = new_arg;
	}

	return new_arg;
}