#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;
using Iterator = BinarySearchTree<int>::Iterator;

TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

// Test empty, size, height
TEST(test_empty_size_height) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_EQUAL(tree.height(), 0);

    tree.insert(5);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_EQUAL(tree.height(), 1);

    tree.insert(4);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 2);
    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(6);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 3);
    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);
    ASSERT_EQUAL(tree.height(), 5);

    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 9);
    ASSERT_EQUAL(tree.height(), 5);

    tree.insert(15);
    tree.insert(10);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 11);
    ASSERT_EQUAL(tree.height(), 7);

    tree.insert(11);
    tree.insert(12);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 13);
    ASSERT_EQUAL(tree.height(), 9);

    BinarySearchTree<int> tree2;
    BinarySearchTree<int> copy2 = tree2;
    ASSERT_TRUE(copy2.empty());
    ASSERT_TRUE(copy2.size() == 0);
    ASSERT_TRUE(copy2.height() == 0);

    tree2.insert(1);
    ASSERT_TRUE(copy2.empty());
    ASSERT_TRUE(copy2.size() == 0);
    ASSERT_TRUE(copy2.height() == 0);
    ASSERT_FALSE(tree2.empty());
    ASSERT_EQUAL(tree2.size(), 1);
    ASSERT_EQUAL(tree2.height(), 1);

    tree2.insert(2);
    tree2.insert(3);
    tree2.insert(4);
    tree2.insert(5);
    tree2.insert(6);
    ASSERT_TRUE(copy2.empty());
    ASSERT_TRUE(copy2.size() == 0);
    ASSERT_TRUE(copy2.height() == 0);
    ASSERT_FALSE(tree2.empty());
    ASSERT_EQUAL(tree2.size(), 6);
    ASSERT_EQUAL(tree2.height(), 6);
}

TEST(test_copy_destroy) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(4);
    tree.insert(6);

    BinarySearchTree<int> copy = tree;
    ASSERT_EQUAL(copy.size(), 3);
    ASSERT_EQUAL(copy.height(), 2);
    ASSERT_FALSE(copy.empty());
    
    auto it = copy.begin();
    ASSERT_EQUAL(*it, 4);
    ++it;
    ASSERT_EQUAL(*it, 5);
    ++it;
    ASSERT_EQUAL(*it, 6);

    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    ASSERT_EQUAL(copy.size(), 3);
    ASSERT_EQUAL(copy.height(), 2);
    ASSERT_FALSE(copy.empty());

    BinarySearchTree<int> copy2(tree);
    ASSERT_EQUAL(copy2.size(), 6);
    ASSERT_EQUAL(copy2.height(), 5);
    ASSERT_FALSE(copy.empty());

    copy2 = copy;
    ASSERT_EQUAL(copy2.size(), 3);
    ASSERT_EQUAL(copy2.height(), 2);
    ASSERT_FALSE(copy2.empty());


    BinarySearchTree<int> tree3;
    BinarySearchTree<int> copy3 = tree3;
    ASSERT_TRUE(copy3.empty());

    copy3.insert(2);
    copy3.insert(1);
    copy3.insert(3);


    tree3.insert(10);
    tree3.insert(11);
    tree3.insert(9);
    tree3.insert(15);
    tree3.insert(6);

    copy3 = tree3;
    ASSERT_FALSE(copy3.empty());
    ASSERT_EQUAL(copy3.size(), 5);
    ASSERT_EQUAL(copy3.height(), 3);

    Iterator it3 = copy3.begin();
    ASSERT_EQUAL(*it3, 6);
    ++it3;
    ASSERT_EQUAL(*it3, 9);
    ++it3;
    ASSERT_EQUAL(*it3, 10);
    ++it3;
    ASSERT_EQUAL(*it3, 11);
    ++it3;
    ASSERT_EQUAL(*it3, 15);

    BinarySearchTree<int> empty;
    copy3 = empty;
    ASSERT_TRUE(copy3.empty());
}

TEST(test_find_insert_min_max_minGreater) {
    BinarySearchTree<int> tree;
    auto it = tree.insert(2);
    ASSERT_EQUAL(*it, 2);
    ++it;
    ASSERT_EQUAL(it, tree.end());

    it = tree.insert(1);
    tree.insert(3);
    ASSERT_EQUAL(*it, 1);
    ASSERT_EQUAL(it, tree.min_element());
    ++it;
    ASSERT_EQUAL(*it, 2);
    ASSERT_EQUAL(it, tree.min_greater_than(1));
    ++it;
    ASSERT_EQUAL(*it, 3);
    ASSERT_EQUAL(it, tree.max_element());
    ASSERT_EQUAL(it, tree.min_greater_than(2));
    ++it;
    ASSERT_EQUAL(it, tree.min_greater_than(3));
    ASSERT_EQUAL(tree.end(), tree.min_greater_than(3));

    tree.insert(9);
    tree.insert(5);
    tree.insert(4);
    tree.insert(12);

    ASSERT_EQUAL(*tree.max_element(), 12);
    ASSERT_EQUAL(*tree.min_greater_than(5), 9);
    ASSERT_EQUAL(*tree.min_greater_than(3), 4);
    ASSERT_EQUAL(*tree.min_greater_than(4), 5);
    ASSERT_EQUAL(*tree.min_greater_than(9), 12);


    BinarySearchTree<int> tree2;
    Iterator it2 = tree2.find(1);
    ASSERT_TRUE(it2 == tree2.end());
    it2 = tree2.find(0);
    ASSERT_TRUE(it2 == tree2.end());

    tree2.insert(5);
    tree2.insert(2);
    tree2.insert(9);
    it2 = tree2.find(10);
    ASSERT_TRUE(it2 == tree2.end());
    it2 = tree2.find(5);
    ASSERT_TRUE(*it2 == 5);
    ++it2;
    ASSERT_TRUE(*it2 == 9);

    tree2.insert(10);
    tree2.insert(-4);
    it2 = tree2.find(-4);
    ASSERT_TRUE(*it2 == -4);
    it2 = tree2.find(10);
    ASSERT_TRUE(*it2 == 10);

    BinarySearchTree<int> empty;
    tree2 = empty;

    it2 = tree2.find(5);
    ASSERT_TRUE(it2 == tree2.end());
    it2 = tree2.min_element();
    ASSERT_TRUE(it2 == tree2.end());
    it2 = tree2.max_element();
    ASSERT_TRUE(it2 == tree2.end());
    it2 = tree2.min_greater_than(0);
    ASSERT_TRUE(it2 == tree2.end());


    BinarySearchTree<int> tree3;
    Iterator it3 = tree3.insert(5);
    ASSERT_TRUE(*it3 == 5);
    tree3.insert(9);
    ++it3;
    ASSERT_TRUE(*it3 == 9);

    it3 = tree3.insert(0);
    ASSERT_EQUAL(*it3, 0);
    ++++it3;
    ASSERT_EQUAL(*it3, 9);

    it3 = tree3.max_element();
    ASSERT_EQUAL(*it3, 9);
    it3 = tree3.min_element();
    ASSERT_EQUAL(*it3, 0);
    it3 = tree3.min_greater_than(9);
    ASSERT_TRUE(it3 == tree3.end());
    it3 = tree3.min_greater_than(0);
    ASSERT_EQUAL(*it3, 5);
    it3 = tree3.min_greater_than(7);
    ASSERT_EQUAL(*it3, 9);
}

TEST(test_sorting_invariant_traversal) {
  BinarySearchTree<int> tree;
  tree.insert(2);
  tree.insert(1);
  tree.insert(3);
  tree.insert(4);
  
  
  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "2 1 3 4 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "1 2 3 4 ");

  BinarySearchTree<int> tree2;
  tree2.insert(5);
  tree2.insert(3);
  tree2.insert(4);
  tree2.insert(1);
  tree2.insert(2);
  tree2.insert(8);
  tree2.insert(10);
  tree2.insert(7);

  ostringstream oss_preorder2;
  tree2.traverse_preorder(oss_preorder2);
  cout << "preorder" << endl;
  cout << oss_preorder2.str() << endl << endl;
  ASSERT_TRUE(oss_preorder2.str() == "5 3 1 2 4 8 7 10 ");

  ostringstream oss_inorder2;
  tree2.traverse_inorder(oss_inorder2);
  cout << "inorder" << endl;
  cout << oss_inorder2.str() << endl << endl;
  ASSERT_TRUE(oss_inorder2.str() == "1 2 3 4 5 7 8 10 ");

  // Sorting
  BinarySearchTree<int> tree3;
  tree3.insert(2);
  tree3.insert(3);
  tree3.insert(1);
  Iterator it3 = tree3.begin();
  ++it3;
  *it3 = 7;
  ASSERT_FALSE(tree3.check_sorting_invariant());
  ++it3;
  *it3 = 100;
  ASSERT_TRUE(tree3.check_sorting_invariant());
  it3 = tree3.begin();
  *it3 = -100;
  ASSERT_TRUE(tree3.check_sorting_invariant());
}


TEST_MAIN()
