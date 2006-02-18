
#ifndef MOCK_TREE_H
#define MOCK_TREE_H


#include "../tree.h"


/**
 * This class is used for testing some aspects of the tree class since we need
 * access to it's private members.
 */
template <typename NodeObject>
class mock_tree : public tree<NodeObject> {
public:
	typedef typename tree<NodeObject>::child_iterator child_iterator;
	typedef typename tree<NodeObject>::const_child_iterator
										const_child_iterator;

	mock_tree(const NodeObject &object) : tree<NodeObject>(object) {}
	bool child_added(const NodeObject &object) const;
};


/**
 * Check whether @a object is in a child node of the tree.
 *
 * @param object		object to look for
 *
 * @return				True if @a object is in a child node, false otherwise.
 */
template <typename NodeObject>
bool mock_tree<NodeObject>::child_added(const NodeObject &object) const {
	for (const_child_iterator child_tree_i=tree<NodeObject>::child_begin();
		 child_tree_i!=tree<NodeObject>::child_end(); ++child_tree_i) {
		if ((*child_tree_i).value() == object)
			return true;
	}

	return false;
}


#endif
