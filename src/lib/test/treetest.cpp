
#include <string>
#include "treetest.h"
#include "mock_tree.h"
#include "mocknode.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TreeTest, TreeTest::SuiteName());


/**
 * tree class constructor tests.
 */
void TreeTest::Constructor() {
	// create a tree with a basic node object
	tree<int> int_tree(int(0));
	CPPUNIT_ASSERT_EQUAL(int(0), int_tree.value());

	// create a tree with a more complex node object
	MockNode mock_node;
	mock_node.SetValue(1);
	tree<MockNode> mock_tree(mock_node);
	//CPPUNIT_ASSERT_EQUAL(mock_node, mock_tree.value());

	// create a tree with a more complex node object from the stl
	std::string string_node("string node");
	tree<std::string> string_tree(string_node);
	CPPUNIT_ASSERT_EQUAL(string_node, string_tree.value());
}



/**
 *
 */
void TreeTest::add_child() {
	mock_tree<int> t(0);

	// add a child
	CPPUNIT_ASSERT(!t.child_added(1));
	t.add_child(1);
	CPPUNIT_ASSERT(t.child_added(1));

	// add a second child
	CPPUNIT_ASSERT(!t.child_added(2));
	t.add_child(2);
	CPPUNIT_ASSERT(t.child_added(2));

	// add a third child and a child to that child
	CPPUNIT_ASSERT(!t.child_added(3));
	CPPUNIT_ASSERT(!t.child_added(4));
	t.add_child(3).add_child(4);
	CPPUNIT_ASSERT(t.child_added(3));
	CPPUNIT_ASSERT(!t.child_added(4)); // only the third child should be visible
	
}


/**
 * crop() method test.
 */
void TreeTest::crop() {
	mock_tree<int> t(0);
	t.add_child(1);
	t.add_child(2);
	t.add_child(3);
	t.add_child(4);

	// remove all children except the second one
	tree<int>::child_iterator i = t.child_begin();
	++i;
	CPPUNIT_ASSERT_NO_THROW(t.crop(i));
	CPPUNIT_ASSERT_EQUAL(1, t.child_end() - t.child_begin());
	CPPUNIT_ASSERT_EQUAL(2, (*i).value());

}


/**
 * clear() method test. Depends on tree::empty(), end tree::add_child().
 */
void TreeTest::clear() {
	tree<int> t(1);

	CPPUNIT_ASSERT(t.empty());

	t.clear();
	CPPUNIT_ASSERT(t.empty());

	t.add_child(2);
	CPPUNIT_ASSERT(!t.empty());

	t.add_child(3).add_child(4);
	CPPUNIT_ASSERT(!t.empty());
}


/**
 * empty() method test. Test whether a tree has any children.
 */
void TreeTest::empty() {
	tree<int> t(4);

	CPPUNIT_ASSERT(t.empty());

	t.add_child(5);
	CPPUNIT_ASSERT(!t.empty());
}
