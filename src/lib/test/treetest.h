
#ifndef TREETEST_H
#define TREETEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../tree.h"


/**
 * Unit tests for the tree class.
 *
 * Untested methods:
 *		value()					(trivial)
 *		child_begin()			(trivial)
 *		child_end()				(trivial)
 *		operator<<()
 */
class TreeTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TreeTest);
	CPPUNIT_TEST(Constructor);
	CPPUNIT_TEST(add_child);
	CPPUNIT_TEST(crop);
	CPPUNIT_TEST(clear);
	CPPUNIT_TEST(empty);
	CPPUNIT_TEST_SUITE_END();

public:
	void Constructor();
	void add_child();
	void crop();
	void clear();
	void empty();
};


#endif
