#ifndef DINAMIC_ARRAY
#define DINAMIC_ARRAY

#include <new>

template<class Type>
struct Allocator {
	//malloc, calloc, new char[]
	Type* allocate(size_t size) {
		return malloc(sizeof(Type) * size);
	}
	//realloc
	Type* reallocate(void* old, size_t new_size) {
		return (Type*)realloc(old, new_size);
	}
	//free, delete[]
	Type* deallocate(void* old) {
		free(old);
		return nullptr;
	}
};

template<class Type>
struct Trait {
	Type& create(Type* place, const Type& obj) {
		return *(new ((void*)(place)) Type(obj));
	}
	void destroy(Type& place) {
		place.~Type();
	}
};

template<
	class Type,
	typename Traits = Trait<Type>,
	typename Allocators = Allocator<Type>
>
class DinamicArray {

	//using allocate = allocatorTraits.alloc.allocate;
	//using reallocate = allocatorTraits.alloc.reallocate;
	//using create = allocatorTraits.trait.create;
	//using destroy = allocatorTraits.trait.destroy;
public:
	DinamicArray() :data_(nullptr), size_(0), capacity(0) {}
	DinamicArray(size_t n) :size_(n), capacity(n) {
		data_ = allocatorTraits.alloc.allocate(n);
		//data_ = allocate(n);
	}
	~DinamicArray() {
		if (data_) {
			for (auto& e : *this) {
				allocatorTraits.trait.destroy(e);
			}
			allocatorTraits.alloc.deallocate(data_);
		}
	}
	Type& operator[](int n) {
		return data_[n];
	}

	Type& at(int n) {
		//???????? ???????? ??????? ? ??????? ??????????
		return data_[n];
	}
	Type& front() {
		return at(0);//*date_ ,data_[0]
	}
	Type& back() {
		return at(size_);
	}
	void pushBack(const Type& obj) {
		if (size_ == capacity) {
			data_ = allocatorTraits.alloc.reallocate(data_, capacity * 2);
			capacity *= 2;
		}
		allocatorTraits.trait.create(&data_[size_], obj);
		size_++;
	}

	void popBack() {
		size_--;
		destroy(data_[size_]);
	}
	class Iterator {
		friend class DinamicArray;
		Iterator() = delete;
		Iterator(Type* node, bool revers) :node(node), revers(revers) {}
	public:
		Type& operator *() {
			return *node;
		}
		Type* operator ->() {
			return node;
		}
		Iterator& operator++() {
			if (revers) {
				node -= 1;
			}
			else {
				node += 1;
			}
			return *this;
		}
		Iterator& operator++(int) {
			Iterator ret(*this);
			if (revers) {
				node -= 1;
			}
			else {
				node += 1;
			}
			return ret;
		}
		Iterator& operator--() {
			if (revers) {
				node += 1;
			}
			else {
				node -= 1;
			}
			return *this;
		}
		Iterator& operator--(int) {
			Iterator ret(*this);
			if (revers) {
				node += 1;
			}
			else {
				node -= 1;
			}
			return ret;
		}
		bool operator ==(const Iterator& other) {
			return this->node == other.node;
		}
		bool operator !=(const Iterator& other) {
			return this->node != other.node;
		}
	private:
		bool revers;
		Type* node;
	};
	Iterator begin() {
		return Iterator(data_, false);
	}
	Iterator end() {
		return Iterator(data_ + size_ - 1, false);
	}
	Iterator rbegin() {
		return Iterator(data_ + size_ - 1, true);
	}
	Iterator rend() {
		return Iterator(data_, true);
	}
private:
	Type* data_;
	size_t size_;
	size_t capacity;
	struct {
		Traits trait;
		Allocators alloc;
	}allocatorTraits;
};




#endif //!DINAMIC_ARRAY