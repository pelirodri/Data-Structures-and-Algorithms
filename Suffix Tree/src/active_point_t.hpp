#ifndef ACTIVE_POINT_T_HPP
#define ACTIVE_POINT_T_HPP
	namespace st {
		struct active_point_t {
			active_point_t(suffix_tree_node_t *node = nullptr, unsigned char edge = -1, size_t length = 0) :
				node(node), edge(edge), length(length) {}

			active_point_t(active_point_t *active_point) :
				node(active_point->node), edge(active_point->edge), length(active_point->length) {}

			suffix_tree_node_t *node;
			unsigned char edge;
			size_t length;
		};
	}
#endif
