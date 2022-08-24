#ifndef SUFFIX_TREE_NODE_T_HPP
#define SUFFIX_TREE_NODE_T_HPP
	#include <array>

	namespace st {
		struct suffix_tree_node_t {
			suffix_tree_node_t(int start = -1, int *end = nullptr) :
				start(start), end(end), suffix_link(nullptr), suffix_index(-1) {}

			~suffix_tree_node_t() {
				if (end && suffix_index == -1) {
					delete end;
				}
			}

			suffix_tree_node_t(suffix_tree_node_t &&) noexcept = default;
			suffix_tree_node_t &operator=(suffix_tree_node_t &&) noexcept = default;

			int start;
			int *end;

			std::array<std::unique_ptr<suffix_tree_node_t>, 256> children;
			suffix_tree_node_t *suffix_link;

			int suffix_index;

			inline size_t get_edge_length() const {
				return end ? (*end - start) + 1 : 0;
			}
		};
	}
#endif
