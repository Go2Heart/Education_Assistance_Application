#ifndef RBTREE_H
#define RBTREE_H

#include <cstdio>
#include <cstdlib>
#include "vector.h"

template<class T1, class T2>
class RbNode {
public:
    RbNode* parent;
    RbNode* left;
    RbNode* right;
    T1 key;
    T2 data;
    int color;
    RbNode(T1 key_in, T2 data_in) : key(key_in), data(data_in), color(0), left(NULL), right(NULL), parent(NULL) {}
    ~RbNode() {}
};
template<class T1, class T2>
class RbTree {
public:
    RbTree();
    ~RbTree();
    void LeftRotate(RbNode<T1, T2>* current);
    void RightRotate(RbNode<T1, T2>* current);
    void InsertNode(T1 key, T2 data);
    bool DeleteNode(T1 key);
    void FixTree(RbNode<T1, T2>* current);
    void EraseNode(RbNode<T1, T2>* delNode);
    void FixTreeDelete(RbNode<T1, T2>* current, RbNode<T1, T2>* currentFather);
    RbNode<T1, T2>* GetPrefix(RbNode<T1, T2>* current);
    RbNode<T1, T2>* GetSuffix(RbNode<T1, T2>* current);
    bool Check(RbNode<T1, T2>* current);
    int GetNum(RbNode<T1, T2>* current);
    RbNode<T1, T2>* FindNode(T1 key);
    Vector<T2> Find(T1 key);
    RbNode<T1, T2>* root = NULL;
};

template<class T1, class T2> RbTree<T1, T2>::RbTree() : root(NULL) {}

template<class T1, class T2> RbTree<T1, T2>::~RbTree() {}

template<class T1, class T2> void RbTree<T1, T2>::InsertNode(T1 key, T2 data) {
    RbNode<T1, T2>* tmpNode = root;
    if(!root) root = new RbNode<T1, T2>(key, data);
    while(tmpNode != NULL) {
        if(tmpNode->key < key) {
            if(tmpNode->right == NULL) {
                tmpNode->right = new RbNode<T1, T2>(key, data);
                tmpNode->right->color = 1;
                tmpNode->right->parent = tmpNode;
                if(tmpNode->color == 1) FixTree(tmpNode->right);
                break;
            } else tmpNode = tmpNode->right;
        } else {
            if(tmpNode->left == NULL) {
                tmpNode->left = new RbNode<T1, T2>(key, data);
                tmpNode->left->color = 1;
                tmpNode->left->parent = tmpNode;
                if(tmpNode->color == 1) FixTree(tmpNode->left);
                break;
            } else tmpNode = tmpNode->left;
        }
    }
}

template<class T1, class T2> void RbTree<T1, T2>::LeftRotate(RbNode<T1, T2>* current) {
    RbNode<T1, T2>* parent = current->parent;
    RbNode<T1, T2>* right = current->right;
    if(right->left) right->left->parent = current;
    right->parent = parent;
    if(parent == NULL) root = right;
    else if(current == parent->left) parent->left = right;
    else parent->right = right;
    current->parent = right;
    current->right = right->left;
    right->left = current;
}

template<class T1, class T2> void RbTree<T1, T2>::RightRotate(RbNode<T1, T2>* current) {
    RbNode<T1, T2>* parent = current->parent;
    RbNode<T1, T2>* left = current->left;
    if(left->right) left->right->parent = current;
    left->parent = parent;
    if(parent == NULL) root = left;
    else if(current == parent->left) parent->left = left;
    else parent->right = left;
    current->parent = left;
    current->left = left->right;
    left->right = current;
}

template<class T1, class T2> void RbTree<T1, T2>::FixTree(RbNode<T1, T2>* current) {
    RbNode<T1, T2>* tmpNode = current, *uncleNode;
    while(true) {
        if(!tmpNode->parent || tmpNode->parent->color != 1) {
            if(tmpNode == root) tmpNode->color = 0;
            break;
        }
        RbNode<T1, T2>* parentNode = tmpNode->parent;
        RbNode<T1, T2>* grandparentNode = parentNode->parent;
        if(grandparentNode) {
            if(parentNode == grandparentNode->left) {
                uncleNode = grandparentNode->right;
                if(uncleNode) {
                    if(uncleNode->color == 1) {
                        uncleNode->color = 0;
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        tmpNode = grandparentNode;
                    } else if(tmpNode == parentNode->right) {
                        tmpNode = parentNode;
                        LeftRotate(tmpNode);
                    } else {
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        RightRotate(grandparentNode);
                    }
                } else {
                    if(tmpNode == parentNode->left) {
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        RightRotate(grandparentNode);
                    } else {
                        tmpNode = parentNode;
                        LeftRotate(tmpNode);
                    }
                }
            } else {
                uncleNode = grandparentNode->left;
                if(uncleNode) {
                    if(uncleNode->color == 1) {
                        uncleNode->color = 0;
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        tmpNode = grandparentNode;
                    } else if(tmpNode == parentNode->right) {
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        LeftRotate(grandparentNode);
                    } else {
                        tmpNode = parentNode;
                        RightRotate(tmpNode);
                    }
                } else {
                    if(tmpNode == parentNode->left) {
                        tmpNode = parentNode;
                        RightRotate(tmpNode);
                    } else {
                        parentNode->color = 0;
                        grandparentNode->color = 1;
                        LeftRotate(grandparentNode);
                    }
                }
            }
        } else {
            parentNode->color = 0;
            tmpNode = parentNode;
        }
    }
}

template<class T1, class T2> void RbTree<T1, T2>::EraseNode(RbNode<T1, T2>* NodeDel) {
    if(NodeDel->left && NodeDel->right) {
        RbNode<T1, T2>* suffixNode = GetSuffix(NodeDel);
        NodeDel->key = suffixNode->key;
        NodeDel->data = suffixNode->data;
        EraseNode(suffixNode);
        return;
    } else {
        if(NodeDel == root) {
            root = root->left == NULL ? root->right : root->left;
            if(root) {
                root->parent = NULL;
                root->color = 0;
            }
            delete NodeDel;
            return;
        } else {
            RbNode<T1, T2>* parent = NodeDel->parent;
            RbNode<T1, T2>* child = NULL, *replace = NULL;
            child = NodeDel->left == NULL ? NodeDel->right : NodeDel->left;
            if(NodeDel == parent->left) parent->left = child;
            else parent->right = child;
            replace = child;
            if(child) {
                child->parent = parent;
                child->color = 0;
            } else if(NodeDel->color == 0) FixTreeDelete(replace, parent);
            delete NodeDel;
        }
    }
}

template<class T1, class T2> void RbTree<T1, T2>::FixTreeDelete(RbNode<T1, T2>* replace, RbNode<T1, T2>* parent) {
    RbNode<T1, T2>* brother = NULL;
    if(replace == root) return;
    if(replace == parent->left) brother = parent->right;
    else brother = parent->left;
    l0:
    if(brother->color == 0) {
        int left = 1, right = 1;
        if(!brother->left || brother->left->color == 0) left = 0;
        if(!brother->right || brother->right->color == 0) right = 0;
        if(left == 0 && right == 0) {
            if(parent->color == 0) {
                brother->color = 1;
                replace = parent;
                parent = parent->parent;
                FixTreeDelete(replace, parent);
            } else {
                brother->color = 1;
                parent->color = 0;
            }
        } else {
            if(brother == parent->left && left == 1 || brother == parent->right && right == 1) {
                if(brother == parent->left && left == 1) {
                    l1:
                    RbNode<T1, T2>* L = brother->left;
                    int color = parent->color;
                    parent->color = brother->color;
                    brother->color = color;
                    RightRotate(parent);
                    L->color = 0;
                }
                if(brother == parent->right && right == 1) {
                    l2:
                    RbNode<T1, T2>* L = brother->right;
                    int color = parent->color;
                    parent->color = brother->color;
                    brother->color = color;
                    LeftRotate(parent);
                    L->color = 0;
                }
            } else if(brother == parent->left && left == 0 || brother == parent->right && right == 0) {
                if(brother == parent->left && left == 0) {
                    RbNode<T1, T2>* tmp = brother->right;
                    int color = brother->color;
                    brother->color = brother->right->color;
                    brother->right->color = color;
                    LeftRotate(brother);
                    brother = tmp;
                    parent = brother->parent;
                    goto l1;
                } else if(brother == parent->right && right == 0) {
                    RbNode<T1, T2>* tmp = brother->left;
                    int color = brother->color;
                    brother->color = brother->left->color;
                    brother->left->color = color;
                    RightRotate(brother);
                    brother = tmp;
                    parent = brother->parent;
                    goto l2;
                }
            }
        }
    } else {
        if(brother == parent->left) {
            int color = parent->color;
            parent->color = brother->color;
            brother->color = color;
            RightRotate(parent);
            brother = parent->left;
            goto l0;
        } else if (brother == parent->right) {
            int color = parent->color;
            parent->color = brother->color;
            brother->color = color;
            LeftRotate(parent);
            brother = parent->right;
            goto l0;
        }
    }
}

template<class T1, class T2> bool RbTree<T1, T2>::DeleteNode(T1 key) {
    RbNode<T1, T2>* tmpNode = root;
    while(tmpNode != NULL && !(tmpNode->key == key)) {
        if(key > tmpNode->key) tmpNode = tmpNode->right;
        else tmpNode = tmpNode->left;
    }
    if(tmpNode) {
        EraseNode(tmpNode);
        return true;
    } else return false;
}

template<class T1, class T2> RbNode<T1, T2>* RbTree<T1, T2>::GetPrefix(RbNode<T1, T2>* current) {
    RbNode<T1, T2>* tmpNode = current->left;
    while(tmpNode) {
        if(tmpNode->right) tmpNode = tmpNode->right;
        else break;
    }
    return tmpNode;
}

template<class T1, class T2> RbNode<T1, T2>* RbTree<T1, T2>::GetSuffix(RbNode<T1, T2>* current) {
    RbNode<T1, T2>* tmpNode = current->right;
    while(tmpNode) {
        if(tmpNode->left) tmpNode = tmpNode->left;
        else break;
    }
    return tmpNode;
}

template<class T1, class T2> bool RbTree<T1, T2>::Check(RbNode<T1, T2>* current) {
    bool good = 1;
    if(current->left) {
        good &= Check(current->left) & (current->left->parent == current);
    }
    if(current->right) {
        good &= Check(current->right) & (current->right->parent == current); 
    }
    return good;
}

template<class T1, class T2> int RbTree<T1, T2>::GetNum(RbNode<T1, T2>* current) {
    int left = 0, right = 0;
    if(current->left) {
        left = GetNum(current->left);
    }
    if(current->right) {
        right = GetNum(current->right);
    }
    if(left == -1 || right == -1 || left != right) return -1;
    return left + (current->color == 0);
}

template<class T1, class T2> RbNode<T1, T2>* RbTree<T1, T2>::FindNode(T1 key) {
    RbNode<T1, T2>* tmpNode = root;
    while(tmpNode != NULL && !(tmpNode->key == key)) {
        if(tmpNode->key < key) tmpNode = tmpNode->right;
        else tmpNode = tmpNode->left;
    }
    return tmpNode;
}

template<class T1, class T2> Vector<T2> RbTree<T1, T2>::Find(T1 key) {
    Vector<T2> result;
    RbNode<T1, T2>* tmpNode = FindNode(key);
    if(tmpNode) {
        result.push_back(tmpNode->data);
        RbNode<T1, T2>* leftNode = GetPrefix(tmpNode);
        while(leftNode && key == leftNode->key) {
            result.push_back(leftNode->data);
            leftNode = GetPrefix(leftNode);
        }
        RbNode<T1, T2>* rightNode = GetSuffix(tmpNode);
        while(rightNode && key == rightNode->key) {
            result.push_back(rightNode->data);
            rightNode = GetSuffix(rightNode);
        }
    }
    return result;
}

#endif