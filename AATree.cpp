#include <bits/stdc++.h>
using namespace std;
template<typename T>
class AATree {
	struct node {
		int num, size;
		int level = 1;
		T val;
		node *lchild, *rchild;
	};
	void destroy(node *&p) {
		if (p == nullptr) return;
		destroy(p->lchild);
		destroy(p->rchild);
		delete p;
	}
	int getsize(node *p) {
		if (p == nullptr) return 0;
		return p->size;
	}
	void L_rotate(node *&p) {
		if (p->lchild != nullptr && p->level == p->lchild->level) {
			node *x = p;
			p = p->lchild;
			x->lchild = p->rchild;
			p->rchild = x;
			p->size = x->size;
			x->size = getsize(x->lchild) + getsize(x->rchild) + x->num;
		}
	}
	void R_rotate(node *&p) {
		if (p->rchild != nullptr && p->rchild->rchild != nullptr && p->level == p->rchild->rchild->level) {
			node *x = p;
			p = p->rchild;
			x->rchild = p->lchild;
			p->lchild = x;
			p->level++;
			p->size = x->size;
			x->size = getsize(x->lchild) + getsize(x->rchild) + x->num;
		}
	}
	void rotate(node *&p) {
		L_rotate(p);
		if (p->rchild != nullptr) L_rotate(p->rchild);
		if (p->rchild != nullptr && p->rchild->rchild != nullptr) L_rotate(p->rchild->rchild);
		R_rotate(p);
		if (p->rchild != nullptr) R_rotate(p->rchild);
	}
public:
	node *root;
	AATree() { root = nullptr; }
	~AATree() { destroy(root); }
	void insert(node *&p, T val) {
		if (p == nullptr) {
			p = new node();
			p->val = val;
			p->num = 1;
			p->size = 1;
			return;
		}
		if (val < p->val) insert(p->lchild, val);
		else if (val > p->val) insert(p->rchild, val);
		else p->num++;
		p->size = getsize(p->lchild) + getsize(p->rchild) + p->num;
		L_rotate(p), R_rotate(p);
	}
	void remove(node *&p, T val) {
		if (p == nullptr) return;
		if (val < p->val) remove(p->lchild, val);
		else if (val > p->val) remove(p->rchild, val);
		else {
			if (p->num > 1) {
				p->num--;
				p->size--;
				return;
			}
			if (p->lchild != nullptr && p->rchild != nullptr) {
				node *x = p->rchild;
				while (x->lchild != nullptr) x = x->lchild;
				swap(p->num, x->num);
				swap(p->val, x->val);
				swap(p->level, x->level);
				x->size = getsize(x->lchild) + getsize(x->rchild) + x->num;
				p->size = getsize(p->lchild) + getsize(p->rchild) + p->num;
				remove(p->rchild, val);
				p->size = getsize(p->lchild) + getsize(p->rchild) + p->num;
				rotate(p);
				return;
			}
			node *x = p;
			if (p->lchild != nullptr) p = p->lchild;
			else p = p->rchild;
			if (p != nullptr) rotate(p);
			delete x;
			return;
		}
		p->size = getsize(p->lchild) + getsize(p->rchild) + p->num;
		rotate(p);
	}
	int rank(node *p, T val) {
		if (p == nullptr) return 0;
		if (p->val > val) return rank(p->lchild, val);
		else if (p->val < val) return getsize(p->lchild) + p->num + rank(p->rchild, val);
		else return getsize(p->lchild);
	}
	T kth(node *p, int k) {
		if (p == nullptr) return 0;
		if (getsize(p->lchild) >= k) return kth(p->lchild, k);
		if (getsize(p->lchild) + p->num >= k) return p->val;
		return kth(p->rchild, k - getsize(p->lchild) - p->num);
	}
	T pre(node *p, T val) {
		node *x = p;
		T res = 0;
		while (x != nullptr) {
			if (x->val < val) res = x->val, x = x->rchild;
			else x = x->lchild;
		}
		return res;
	}
	T next(node *p, T val) {
		node *x = p;
		T res = 0;
		while (x != nullptr) {
			if (x->val > val) res = x->val, x = x->lchild;
			else x = x->rchild;
		}
		return res;
	}
};
AATree<int> tr;
int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		int opt, x;
		cin >> opt >> x;
		if (opt == 1) tr.insert(tr.root, x);
		if (opt == 2) tr.remove(tr.root, x);
		if (opt == 3) cout << tr.rank(tr.root, x) + 1 << "\n";
		if (opt == 4) cout << tr.kth(tr.root, x) << "\n";
		if (opt == 5) cout << tr.pre(tr.root, x) << "\n";
		if (opt == 6) cout << tr.next(tr.root, x) << "\n";
	}
	return 0;
}