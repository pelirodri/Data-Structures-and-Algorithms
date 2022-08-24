#include "suffix_tree.hpp"

#include <numeric>

st::suffix_tree::suffix_tree(const std::string &str) :
	str(str + '$'),
	global_end(0),
	remaining_suffixes(0),
	root(std::make_unique<suffix_tree_node_t>()),
	active_point(std::make_unique<active_point_t>()) {
	build_tree();
}

bool st::suffix_tree::contains_substring(const std::string &sub_str) const {
	return contains_substring(sub_str, root);
}

int st::suffix_tree::find_first_substring(const std::string &sub_str) const {
	return find_substring(sub_str, root, first);
}

int st::suffix_tree::find_last_substring(const std::string &sub_str) const {
	return find_substring(sub_str, root, last);
}

size_t st::suffix_tree::count_substring_occurrences(const std::string &sub_str) const {
	return count_substring_occurrences(sub_str, root);
}

void st::suffix_tree::reset(const std::string &new_str) {
	str = new_str + '$';
	global_end = 0;
	remaining_suffixes = 0;

	root = std::make_unique<suffix_tree_node_t>();
	active_point = std::make_unique<active_point_t>();

	build_tree();
}

void st::suffix_tree::build_tree() {
	root->children[str.front()] = std::make_unique<suffix_tree_node_t>(0, &global_end);
	active_point->node = root.get();

	for (int i = 1; i < str.length(); i++) {
		handle_phase(i);
	}

	set_suffix_indexes(root, 0);
}

void st::suffix_tree::handle_phase(int phase_num) {
	remaining_suffixes++;
	global_end++;

	last_internal_node = nullptr;

	while (remaining_suffixes > 0) {
		if (!handle_phase_extension(phase_num)) {
			return;
		}
	}
}

bool st::suffix_tree::handle_phase_extension(int phase_num) {
	if (active_point->length == 0) {
		active_point->edge = phase_num;
	}

	auto &child = active_point->node->children[get_active_edge_char()];

	if (!child) {
		child = std::make_unique<suffix_tree_node_t>(phase_num, &global_end);
	} else {
		std::unique_ptr<active_point_t> new_active_point = walk_down_toward_node(child);

		if (new_active_point) {
			active_point = std::move(new_active_point);
			return true;
		}

		if (str[child->start + active_point->length] == str[phase_num]) {
			handle_rule_3();
			return false;
		}

		split_node(child, phase_num);
	}

	remaining_suffixes--;
	adjust_active_point_after_rule_2(phase_num);

	return true;
}

inline unsigned char st::suffix_tree::get_active_edge_char() const {
	return str[active_point->edge];
}

std::unique_ptr<st::active_point_t> st::suffix_tree::walk_down_toward_node(const tree_node_ptr &node) const {
	std::unique_ptr<active_point_t> new_active_point;

	size_t child_edge_length = node->get_edge_length();

	if (active_point->length >= child_edge_length) {
		new_active_point = std::make_unique<active_point_t>(*active_point.get());

		new_active_point->node = node.get();
		new_active_point->edge += child_edge_length;
		new_active_point->length -= child_edge_length;
	}

	return new_active_point;
}

void st::suffix_tree::handle_rule_3() {
	active_point->length++;

	if (active_point->node != root.get()) {
		add_suffix_link_to_node(active_point->node);
	}
}

void st::suffix_tree::add_suffix_link_to_node(const suffix_tree_node_t *node) {
	if (last_internal_node) {
		last_internal_node->suffix_link = const_cast<suffix_tree_node_t *>(node);
	}
}

void st::suffix_tree::split_node(tree_node_ptr &node, int phase_num) {
	int *internal_end = new int ((node->start + active_point->length) - 1);
	auto internal_node = std::make_unique<suffix_tree_node_t>(node->start, internal_end);

	node->start += active_point->length;

	internal_node->children[str[node->start]] = std::move(node);
	internal_node->children[str[phase_num]] = std::make_unique<suffix_tree_node_t>(phase_num, &global_end);
	internal_node->suffix_link = root.get();

	add_suffix_link_to_node(internal_node.get());
	last_internal_node = internal_node.get();

	active_point->node->children[get_active_edge_char()] = std::move(internal_node);
}

void st::suffix_tree::adjust_active_point_after_rule_2(int phase_num) {
	if (active_point->node == root.get()) {
		if (active_point->length > 0) {
			active_point->edge = (phase_num - remaining_suffixes) + 1;
			active_point->length--;
		}
	} else {
		active_point->node = active_point->node->suffix_link;
	}
}

void st::suffix_tree::set_suffix_indexes(tree_node_ptr &node, size_t total_edge_length) {
	bool is_node_leaf = true;

	for (auto &child : node->children) {
		if (child) {
			is_node_leaf = false;
			set_suffix_indexes(child, total_edge_length + child->get_edge_length());
		}
	}

	if (is_node_leaf) {
		node->suffix_index = str.length() - total_edge_length;
	}
}

bool st::suffix_tree::contains_substring(const std::string &sub_str, const tree_node_ptr &parent) const {
	const auto &child = parent->children[sub_str.front()];

	if (!child) {
		return false;
	}

	size_t edge_length = child->get_edge_length();

	if (str.compare(child->start, std::min(edge_length, sub_str.length()), sub_str, 0, edge_length) != 0) {
		return false;
	}

	if (edge_length < sub_str.length()) {
		return contains_substring(sub_str.substr(edge_length), child);
	}

	return true;
}

int st::suffix_tree::find_substring(const std::string &sub_str, const tree_node_ptr &parent, index_t index) const {
	const auto &child = parent->children[sub_str.front()];

	if (!child) {
		return -1;
	}

	size_t edge_length = child->get_edge_length();

	if (str.compare(child->start, std::min(edge_length, sub_str.length()), sub_str, 0, edge_length) != 0) {
		return -1;
	}

	if (edge_length < sub_str.length()) {
		return find_substring(sub_str.substr(edge_length), child, index);
	}

	return find_suffix_index_from_node(child, index);
}

int st::suffix_tree::find_suffix_index_from_node(const tree_node_ptr &node, index_t index) const {
	if (node->suffix_index != -1) {
		return node->suffix_index;
	}

	auto acc_op = [&](int last_idx, const auto &child) {
		if (!child) {
			return last_idx;
		}

		int cur_idx = find_suffix_index_from_node(child, index);
		return index == first ? std::min(last_idx, cur_idx) : std::max(last_idx, cur_idx);
	};

	return std::accumulate(node->children.begin(), node->children.end(), index == first ? str.length() : -1, acc_op);
}

size_t st::suffix_tree::count_substring_occurrences(const std::string &sub_str, const tree_node_ptr &parent) const {
	const auto &child = parent->children[sub_str.front()];

	if (!child) {
		return 0;
	}

	size_t edge_length = child->get_edge_length();

	if (str.compare(child->start, std::min(edge_length, sub_str.length()), sub_str, 0, edge_length) != 0) {
		return 0;
	}

	if (edge_length < sub_str.length()) {
		return count_substring_occurrences(sub_str.substr(edge_length), child);
	}

	return count_leaf_nodes_from_node(child);
}

size_t st::suffix_tree::count_leaf_nodes_from_node(const tree_node_ptr &node) const {
	if (node->suffix_index != -1) {
		return 1;
	}

	return std::accumulate(node->children.begin(), node->children.end(), 0, [&](size_t leaf_count, const auto &child) {
		return child ? leaf_count + count_leaf_nodes_from_node(child) : leaf_count;
	});
}
