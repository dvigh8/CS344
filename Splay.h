#ifndef SPLAY_H
#define SPLAY_H

#include "tree.h"

enum childType {LEFT, ROOT, RIGHT, UNDEFINED};

template <class T>
class Splay : public Tree<T> {
public:
	// constructors
	Splay( const T &v )
		: Tree<T>(v), type(UNDEFINED) {}

	Splay( const T &v, childType t )
		: Tree<T>(v), type(t) {}

	Splay( const T &v, Splay<T> *l, Splay<T> *r )
		: Tree<T>(v,l,r,NULL), type(UNDEFINED) {}

	Splay( const T &v, Splay<T> *l, Splay<T> *r, Splay<T> *p )
		: Tree<T>(v,l,r,p), type(UNDEFINED) {}

	Splay( const T &v, Splay<T> *l, Splay<T> *r, Splay<T> *p, childType t )
		: Tree<T>(v,l,r,p), type(t) {}


	// cloners
	Splay( const Splay<T> &c )
		: Tree<T>(c.value, c.left, c.right, c.parent, c.height), type(c.type) {}

	Splay( Splay<T> *c )
		: Tree<T>(c->value, c->left, c->right, c->parent, c->height), type(c->type()) {}


	// accessors
	childType getType() { return type; }
	Splay<T> *getRoot(); // returns root of splay tree


	// mutators
	void setType( childType c ) { type = c; }


	// unvirtualized utilities
	Splay<T> * search( const T &v );
		// returns poihttp://askubuntu.com/questions/90214/how-to-set-default-programnter to updated tree (after search): root contains value

	Splay<T> * insert( const T &v );
		// returns pointer to updated tree (after insert): root contains value

	Splay<T> * remove( const T &v );
		// returns pointer to updated tree (after deletion): root contains parent

    Splay<T> * splayTree();
    // splays the value to the top of the tree

    Splay<T> * add(const T & v);

    Splay<T> * find(const T & v);

    Splay<T> * takeout(const T &v);

	void splay();
		// perform a splay operation on tree

private:
	childType type;

	// hidden utilities
	void splay_search( const T & v );
	void splay_insert( const T & v );
	//void splay_remove( const T & v, int side );
	Splay<T> * std_remove( const T & v );

	void updateSplayHeight();

	// hidden debug-helpers
	void splayPanic( string message );
	void debugPrint( string message );
};

template <class T>
Splay<T> * Splay<T>::splayTree()
{
    Splay<T> * this_parent = dynamic_cast<Splay<T>*>(this->getParent());

    if(this_parent == NULL){return this;}

    else if (this == this_parent->left){ // this will rotate the tree to the right

    Splay<T> * this_right = dynamic_cast<Splay<T>*>(this->right);
    Splay<T> * this_parent_parent = dynamic_cast<Splay<T>*>(this_parent->parent);

        if(this_parent_parent != NULL){

            if(this_parent_parent->left == this_parent){

                this_parent_parent->setLeft(this);
                this->setParent(this_parent_parent);
                this->setRight(this_parent);
                this_parent->setParent(this);
                this_parent->setLeft(this_right);
                if(this_right != NULL)this_right->setParent(this_parent);
            }
            else if (this_parent_parent->right == this_parent){
                this_parent_parent->setRight(this);
                this->setParent(this_parent_parent);
                this->setRight(this_parent);
                this_parent->setParent(this);
                this_parent->setLeft(this_right);
                if(this_right != NULL)this_right->setParent(this_parent);
            }
        }
        else{
        this->setParent(NULL);
        this->setRight(this_parent);
        this_parent->setParent(this);
        this_parent->setLeft(this_right);
        if(this_right != NULL)this_right->setParent(this_parent);
        }

    if(this->parent != NULL){this->splayTree();}
    else return this;
    }

    else if (this== this_parent->right){ //this will rotate the tree to the left

    Splay<T> * this_left = dynamic_cast<Splay<T>*>(this->left);
    Splay<T> * this_parent_parent = dynamic_cast<Splay<T>*>(this_parent->parent);

        if(this_parent_parent != NULL){

            if(this_parent_parent->left == this_parent){

                this_parent_parent->setLeft(this);
                this->setParent(this_parent_parent);
                this->setLeft(this_parent);
                this_parent->setParent(this);
                this_parent->setRight(this_left);
                if(this_left != NULL)this_left->setParent(this_parent);
            }
            else if(this_parent_parent->right == this_parent){
                this_parent_parent->setRight(this);
                this->setParent(this_parent_parent);
                this->setLeft(this_parent);
                this_parent->setParent(this);
                this_parent->setRight(this_left);
                if(this_left != NULL)this_left->setParent(this_parent);
            }
        }
        else{
        this->setParent(NULL);
        this->setLeft(this_parent);
        this_parent->setParent(this);
        this_parent->setRight(this_left);
        if(this_left != NULL)this_left->setParent(this_parent);
        }
    if(this->parent != NULL){this->splayTree();}
    else return this;
    }
}

template <class T>
Splay<T> * Splay<T>::add(const T & v)
{
	//The value should be inserted to the right.
	if(this->getValue() < v){
		if(this->getRight() == NULL){
			this->setRight(new Splay<T>(v, NULL, NULL, this));
		}
		else{
			Splay<T>* right = dynamic_cast<Splay<T>*>(this->getRight());
			this->setRight(right->add(v));
		}
	}

	//The value should be inserted to the left.
	else if(this->getValue() > v){
		if(this->getLeft() == NULL){
			this->setLeft( new Splay<T>(v, NULL, NULL, this));
		}
		else{
			Splay<T>* left = dynamic_cast<Splay<T>*>(this->getLeft());
			this->setLeft(left->add(v));
		}
	}

	return this;
}


template <class T>
Splay<T> * Splay<T>::find(const T &v)
{

	//Here is the value;
	if(this->getValue() == v)
		return this;

	//It's to the left.
	else if(this->getValue() > v && this->getLeft() != NULL){
		Splay<T>* left = dynamic_cast<Splay<T>*>(this->getLeft());
		return left->find(v);
	}

	//It's to the right;
	else if(this->getValue() < v && this->getRight() != NULL){
		Splay<T>* right = dynamic_cast<Splay<T>*>(this->getRight());
		return right->find(v);
	}

	//That value isn't in the tree.
	else
		return this;
}


template <class T>
Splay<T>* Splay<T>::insert(const T &v){
	//The value should be inserted to the right.
    Splay<T> * thing;
	this->add(v);

	thing = this->find(v);
    thing->splayTree();
	return thing;

}

template <class T>
Splay<T> * Splay<T>::search(const T & v)
{
    Splay<T> * value = this->find(v);
    return value->splayTree();

}

template <class T>
Splay<T>* Splay<T>::remove(const T &v){

    Splay<T> * found = this->find(v);
    found->splayTree();

    	//This node is the one that we want to delete.
	if(found->getValue() == v){
		//The node is a leaf node.
		if(found->getRight() == NULL && found->getLeft() == NULL){
			//This node is root.

            return NULL;
		}
        //Node has only a left child.
		else if(found->getLeft() != NULL && found->getRight() == NULL){
			Splay<T>* child = dynamic_cast<Splay<T>*>(found->getLeft());

			//This node is the root.
				child->setParent(NULL);
				delete found;
				return child;
		}
		//Node has only a right child.
		else if(found->getRight() != NULL && found->getLeft() ==NULL){
			Splay<T>* child = dynamic_cast<Splay<T>*>(found->getRight());

			//This node is the root.
				child->setParent(NULL);
				delete found;
				return child;
		}
		//Node has two children.
        else if(found->getRight() != NULL && found->getLeft() != NULL){

			Splay<T>* successor = dynamic_cast<Splay<T>*>(found->successor());

			T val = successor->getValue();
			found->setValue(val);
            if(successor->right != NULL){

                successor->right->setParent(successor->parent);
                successor->parent->setLeft(successor->right);

            }
            else{successor->parent->setLeft(NULL);}
			delete successor;

			return found;
		}
	}
	return found;
}
#endif
