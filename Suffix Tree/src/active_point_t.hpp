#ifndef ACTIVE_POINT_T_HPP
#define ACTIVE_POINT_T_HPP
	namespace st {
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
	}
#endif
