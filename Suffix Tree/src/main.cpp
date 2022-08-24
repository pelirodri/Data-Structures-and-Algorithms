#include "suffix_tree.hpp"

#include <iostream>

static void test_contains() {
	st::suffix_tree st ("test");
	std::cout << std::boolalpha << "'test' contains 'tex': " << st.contains_substring("tex") << '\n';

	st.reset("shit");
	std::cout << std::boolalpha << "'shit' contains 'it': " << st.contains_substring("it") << '\n';

	st.reset("Test");
	std::cout << std::boolalpha << "'Test' contains 'E': " << st.contains_substring("E") << '\n';

	st.reset("abcdabctabzcx");
	std::cout << std::boolalpha << "'abcdabctabzcx' contains 'bz': " << st.contains_substring("bz") << '\n';
}

static void test_first_of() {
	st::suffix_tree st ("test");
	std::cout << "First occurrence of 'est' in 'test': " << st.find_first_substring("est") << '\n';

	st.reset("Shit");
	std::cout << "First occurrence of 'S' in 'Shit': " << st.find_first_substring("S") << '\n';

	st.reset("tesT");
	std::cout << "First occurrence of 'sT' in 'tesT': " << st.find_first_substring("sT") << '\n';

	st.reset("abcdabctabzcxx");
	std::cout << "First occurrence of 'c' in 'abcdabctabzcxx': " << st.find_first_substring("c") << '\n';
}

static void test_last_of() {
	st::suffix_tree st ("abcdabctabzcxx");

	std::cout << "Last occurrence of 'a' in 'abcdabctabzcxx': " << st.find_last_substring("a") << '\n';
	std::cout << "Last occurrence of 'b' in 'abcdabctabzcxx': " << st.find_last_substring("b") << '\n';
	std::cout << "Last occurrence of 'x' in 'abcdabctabzcxx': " << st.find_last_substring("x") << '\n';
	std::cout << "Last occurrence of 'z' in 'abcdabctabzcxx': " << st.find_last_substring("z") << '\n';
}

static void test_count_of() {
	st::suffix_tree st ("abcdabctabzcxx");

	std::cout << "Count of 'a' in 'abcdabctabzcxx': " << st.count_substring_occurrences("a") << '\n';
	std::cout << "Count of 'b' in 'abcdabctabzcxx': " << st.count_substring_occurrences("b") << '\n';
	std::cout << "Count of 'c' in 'abcdabctabzcxx': " << st.count_substring_occurrences("c") << '\n';
	std::cout << "Count of 'z' in 'abcdabctabzcxx': " << st.count_substring_occurrences("z") << '\n';
}

static void test_suffix_tree() {
	test_contains();
	std::cout << '\n';
	test_first_of();
	std::cout << '\n';
	test_last_of();
	std::cout << '\n';
	test_count_of();	
}

int main(int argc, char **argv) {
	test_suffix_tree();
	return 0;
}
