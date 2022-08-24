#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP
	#include <array>
	#include <string>

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

		struct active_point_t {
			active_point_t(suffix_tree_node_t *node = nullptr, unsigned char edge = 0, size_t length = 0) :
				node(node), edge(edge), length(length) {}

			~active_point_t() = default;

			active_point_t(const active_point_t &other) : node(other.node), edge(other.edge), length(other.length) {}
			active_point_t &operator=(const active_point_t &other) = delete;

			active_point_t(active_point_t &&other) noexcept = delete;
			active_point_t &operator=(active_point_t &&other) noexcept = delete;

			suffix_tree_node_t *node;
			unsigned char edge;
			size_t length;
		};

		class suffix_tree {
			public:
				suffix_tree(const std::string &);

				bool contains_substring(const std::string &) const;
				int find_first_substring(const std::string &) const;
				int find_last_substring(const std::string &) const;
				size_t count_substring_occurrences(const std::string &) const;
				void reset(const std::string &);
			private:
				enum index_t {
					first,
					last
				};
				
				using tree_node_ptr = std::unique_ptr<suffix_tree_node_t>;

				std::string str;
				int global_end;
				int remaining_suffixes;
				std::unique_ptr<suffix_tree_node_t> root;
				std::unique_ptr<active_point_t> active_point;
				suffix_tree_node_t *last_internal_node;

				void build_tree();
				void handle_phase(int);
				bool handle_phase_extension(int);
				unsigned char get_active_edge_char() const;
				std::unique_ptr<active_point_t> walk_down_toward_node(const tree_node_ptr &) const;
				void handle_rule_3();
				void add_suffix_link_to_node(const suffix_tree_node_t *);
				void split_node(tree_node_ptr &, int);
				void adjust_active_point_after_rule_2(int);
				void set_suffix_indexes(tree_node_ptr &, size_t);
				bool contains_substring(const std::string &, const tree_node_ptr &) const;
				int find_substring(const std::string &, const tree_node_ptr &, index_t) const;
				int find_suffix_index_from_node(const tree_node_ptr &, index_t) const;
				size_t count_substring_occurrences(const std::string &, const tree_node_ptr &) const;
				size_t count_leaf_nodes_from_node(const tree_node_ptr &) const;
		};
	}
#endif
