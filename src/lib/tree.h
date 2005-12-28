
#ifndef TREE_H
#define TREE_H


#include <vector>
#include <stdexcept>

#include <iostream>			/// @todo Remove.


/**
 * Generic tree class.
 * All children of a tree node are unique.
 *
 * This class uses STL-like naming convention (most notably small characters
 * and underscores in method names).
 */
template <typename NodeObject>
class tree {
public:
	///
	typedef typename std::vector<tree<NodeObject> >::iterator child_iterator;
	///
	typedef typename std::vector<tree<NodeObject> >::const_iterator
											const_child_iterator;

	tree(const NodeObject &object);
	
	tree<NodeObject> &add_child(const NodeObject &object);
	void crop(const child_iterator &pos);
	void clear();
	bool empty() const;

	NodeObject &value() { return node_object; }
	const NodeObject &value() const { return node_object; }

	inline child_iterator child_begin() { return children.begin(); }
	inline child_iterator child_end() { return children.end(); }
	inline const_child_iterator child_begin() const { return children.begin(); }
	inline const_child_iterator child_end() const { return children.end(); }

	/// This is a forward declaration of mock_tree that is derived from this
	/// class for use in testing.
	template <typename U> class mock_tree;
	/// The mock_tree sub class is a friend of this class because it needs
	/// to have access to the private members of tree, for use in testing.
	friend class mock_tree<NodeObject>;

//private:
	/// @todo Figure out if it's better to store trees in a vector of
	/// shared_ptr:s. This can significantly improve crop() excution time,
	/// especially for large trees.
	std::vector<tree<NodeObject> > children;
	NodeObject node_object;
};



/**
 * The constructor makes a copy of the object passed in and stores it at the
 * created node.
 *
 * @param object		the object to store at the top of the new tree
 */
template <typename NodeObject>
inline tree<NodeObject>::tree(const NodeObject &object) {
	node_object = object;
}


template <typename T>
std::ostream& operator<<(std::ostream& out_stream, const tree<T>& t) {
	typedef typename tree<T>::const_child_iterator const_child_iterator;
	
	std::out_stream << &(t) << " ";
	
	for (const_child_iterator child_tree_i=t.child_begin();
		 child_tree_i!=t.child_end(); ++child_tree_i) {
		std::out_stream << *child_tree_i;
	}

	return std::out_stream;
}


/**
 * Add a child node to this tree with a copy of @a object as it's value.
 *
 * @param object		reference to the object that will become a child of
 * 						the node through which this method was called
 *
 * @returns				Reference to the child node.
 */
template <typename NodeObject>
tree<NodeObject> &tree<NodeObject>::add_child(const NodeObject &object) {
	// create a child tree.. the push_back() method will make a copy so there
	// is no need to allocate the object manually
	tree<NodeObject> child(object);
	children.push_back(child);

	return children.back();		// return reference to the inserted element
}


/**
 * Delete all children of the current node, except for the one specified by
 * the @a position iterator. After this method the iterator is invalidated.
 *
 * @param position		iterator specifying the child tree to keep
 *
 * @throw out_of_range	This exception is thrown if the @a child_tree was not
 * 						a child of the caller.
 *
 * @todo Figure out if it's better to pass position by value, as done in
 *		 other container classes.
 */
template <typename NodeObject>
void tree<NodeObject>::crop(const child_iterator &position) {
	if (position < children.begin() || position >= children.end())
		throw out_of_range("child iterator out of range");

	// get the child we will keep, erase all children, then put the child
	// we will keep back in
	tree<NodeObject> child_to_keep = *position;
	children.clear();
	children.push_back(child_to_keep);
}


/**
 * Delete all children of the current node.
 */
template <typename NodeObject>
inline void tree<NodeObject>::clear() {
	children.clear();
}


/**
 * Check whether the tree has any children.
 *
 * @return				True if the tree is empty, false if it's not.
 */
template <typename NodeObject>
inline bool tree<NodeObject>::empty() const {
	return children.empty();
}


#endif
