#pragma once

//#include "CharBuf.h"

namespace core {

#define NODEINDEX(p) ( ((ulong)(p)-(ulong)nodes)/sizeof(Node) )

template<class T,int I=8> 
class LinkedList {

	struct Node {
		Node* next;
		Node* prev;
		T item;
	};

	//static __declspec(thread) int dd; // just testing :)

	Node* nodes;
	Node* head;
	Node* tail;
	Node* chainRoot;	// always points to the start of the free list chain

	uint numNodes;
	uint maxNodes;

	void reallocNodes() {
		Node* oldNodes = nodes;
		nodes = (Node*)realloc(nodes, sizeof(Node)*maxNodes);
		if(oldNodes && nodes != oldNodes) {
			// nodes has been moved by realloc. we need to adjust our pointers
			auto offset = (ulong)nodes - (ulong)oldNodes;
			//printf("\n\nnodes has moved from %u to %u. adding %i to ptrs\n\n", (uint)oldNodes, (uint)nodes, offset);
			if(head) head = (Node*) ((ulong)head + offset);
			if(tail) tail = (Node*) ((ulong)tail + offset);
			chainRoot = (Node*) ((ulong)chainRoot + offset);
			for(uint i=0; i<numNodes; i++) {
				nodes[i].next = (Node*) ((ulong)nodes[i].next + offset);
				if(nodes[i].prev) {
					nodes[i].prev = (Node*) ((ulong)nodes[i].prev + offset);
				}
			}
		}
		Node* p = nodes + numNodes;
		for(uint i=numNodes; i<maxNodes; i++) {
			p->next = nodes + i+1;
			//printf("\n"); printNode(p);
			++p;
		}
	}

	void inflate() {
		if(numNodes==maxNodes) {
			maxNodes += I;
			reallocNodes();
			if(chainRoot==NULL) chainRoot = nodes;
			//printf("\n-->numNodes = %d, maxNodes = %d", numNodes, maxNodes);
		}
	}
	Node* nodeAt(uint index) {
		assert(head && "head is null");
		Node* p = head;
		uint i = 0;
		while(i != index && p != tail) {
			p = p->next;
			i++;
		}
		return p;
	}
	void printNode(Node* p, CharBuf* buf) {
		buf->appendFmt("['%d' index=%d prev=%d next=%d]", p->item, NODEINDEX(p), p->prev==NULL ? -1 : NODEINDEX(p->prev), NODEINDEX(p->next));
	}
public:
	LinkedList() : nodes(NULL), numNodes(0), maxNodes(0), head(NULL), tail(NULL), chainRoot(NULL) {}
	~LinkedList() { if(nodes) free(nodes); nodes = NULL; }

	uint size() const { return numNodes; }
	bool isEmpty() const { return numNodes==0; }

	void add(T item) {
		//printf("\n---------------------> add %d",item);
		inflate();

		Node* p = (tail==NULL) ? chainRoot : tail->next;

		p->prev = tail;
		p->item = item;

		if(head==NULL) head = p;
	
		tail = p;
		++numNodes;
		//printf("\n--->head = %d, tail = %d, numNodes = %d, nextFree=%d chainRoot=%d",NODEINDEX(head), NODEINDEX(tail), numNodes, NODEINDEX(tail->next), NODEINDEX(chainRoot));
	}
	void insert(T item, uint index) {
		assert(index<=numNodes);
		//printf("\n---------------------> insert %d",item);
		if(index>=numNodes) { add(item); return; } 
		inflate();

		Node* p = tail->next;
		tail->next = p->next;
		p->item = item;

		if(index==0) {
			p->next = head;
			p->prev = NULL;

			head->prev = p;

			head = p;
		} else {
			Node* next = nodeAt(index);
			Node* prev = next->prev;

			p->prev = prev;
			p->next = next;

			prev->next = p;
			next->prev = p;
		}
		++numNodes;
		//printf("\n--->head = %d, tail = %d, numNodes = %d, nextFree=%d",NODEINDEX(head), NODEINDEX(tail), numNodes, NODEINDEX(tail->next));
	}
	T removeHead() {
		//printf("\n---------------------> removeHead");
		assert(head);
		T item = head->item;
		if(head==tail) {
			chainRoot = head;
			head = NULL;
			tail = NULL;
		} else {
			Node* n = head;
			head = head->next;
			head->prev = NULL;

			n->next = tail->next;
			tail->next = n;
		}
		--numNodes;
		//printf("\n--->head = %d, tail = %d, numNodes = %d, nextFree=%d",NODEINDEX(head), NODEINDEX(tail), numNodes, NODEINDEX(tail->next));
		return item;	
	}
	T removeTail() {
		//printf("\n---------------------> removeTail");
		assert(tail);
		if(head==tail) return removeHead();
		T item = tail->item;
		tail = tail->prev;
		--numNodes;
		//printf("\n--->head = %d, tail = %d, numNodes = %d, nextFree=%d",NODEINDEX(head), NODEINDEX(tail), numNodes, NODEINDEX(tail->next));
		return item;
	}
	T remove(uint index) {
		//printf("\n---------------------> remove index %d", index);
		assert(index<numNodes);
		
		if(index==0) return removeHead();
		if(index==numNodes-1) return removeTail();

		Node* n = nodeAt(index);

		Node* prev = n->prev;
		Node* next = n->next;
		prev->next = next;
		next->prev = prev;

		n->next = tail->next;
		tail->next = n;

		--numNodes;
		//printf("\n--->head = %d, tail = %d, numNodes = %d, nextFree=%d",NODEINDEX(head), NODEINDEX(tail), numNodes, NODEINDEX(tail->next));
		return n->item;
	}

	void clear() {
		if(head!=NULL) chainRoot = head;
		head = NULL;
		tail = NULL;
		numNodes = 0;
	}

	T& operator[](uint index) {
		assert(index<numNodes);
		return nodeAt(index)->item;
	}

	T get(uint index) {
		assert(index<numNodes);
		return nodeAt(index)->item;
	}

	void pack() {
		optimise();
		maxNodes = numNodes;
		reallocNodes();
	}

	// reorder nodes so that head to tail traversal will be more cache friendly
	void optimise() {
		if(numNodes<2) return;
		
		T* values = (T*)malloc(sizeof(T)*numNodes);
		Node* n = head;
		for(uint i=0; i<numNodes; i++) {
			values[i] = n->item;
			n = n->next;
		}
		
		Node* dest = nodes; 
		for(uint i=0; i<maxNodes; i++) {
			dest->prev = dest-1;
			dest->next = dest+1;
			if(i<numNodes) {
				dest->item = values[i];
			}
			++dest;
		}
		head 	  = nodes;
		tail 	  = nodes + (numNodes-1);
		chainRoot = nodes;
		head.prev = null;
	
		free(values);
	}
	const char* toString() {
		static CharBuf buf;
		if(head==NULL) return "[]";
		buf.clear();
		buf += "[";
		Node* p = head;
		while(p != tail) {
			buf.append(p->item).append(", ");
			p = p->next;
		}
		buf.append(p->item);
		buf += "]";
		return buf.cStr();
	}
	const char* toDebugString() {
		static CharBuf buf;
		buf.clear();
		buf.appendFmt("\n--->head = %d, tail = %d, numNodes = %d, maxNodes=%u, nextFree=%d, chainRoot=%u",
				NODEINDEX(head), NODEINDEX(tail), numNodes, maxNodes, NODEINDEX(tail->next), NODEINDEX(chainRoot));
		if(head==NULL) return NULL;
		Node* p = head;
		while(p != tail) {
			buf.append("\n"); printNode(p, &buf);
			p = p->next;
		}
		buf.append("\n"); printNode(p, &buf);
		return buf.cStr();
	}
};

#undef NODEINDEX

} // namespace core