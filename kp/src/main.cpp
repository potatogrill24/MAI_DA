#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

struct PersistentSegmentTree {
    struct Node {
        int count;
        Node* left;
        Node* right;
        Node(int count = 0, Node* left = nullptr, Node* right = nullptr)
            : count(count), left(left), right(right) {}
    };

    Node* build(int l, int r) {
        if (l == r) {
            return new Node();
        }
        int mid = (l + r) / 2;
        return new Node(0, build(l, mid), build(mid + 1, r));
    }

    Node* update(Node* node, int l, int r, int pos, int value) {
        if (l == r) {
            return new Node(node->count + value);
        }
        int mid = (l + r) / 2;
        if (pos <= mid) {
            return new Node(node->count + value, update(node->left, l, mid, pos, value), node->right);
        } else {
            return new Node(node->count + value, node->left, update(node->right, mid + 1, r, pos, value));
        }
    }

    int query(Node* node, int l, int r, int ql, int qr) {
        if (!node || ql > r || qr < l) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return node->count;
        }
        int mid = (l + r) / 2;
        return query(node->left, l, mid, ql, qr) + query(node->right, mid + 1, r, ql, qr);
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<int, int, int>> events; 
    std::vector<int> ys;

    for (int i = 0; i < n; ++i) {
        int l, r, h;
        std::cin >> l >> r >> h;
        events.emplace_back(l, 1, h);     
        events.emplace_back(r, -1, h);
        ys.push_back(h);
    }

    std::vector<std::pair<int, int>> points(m); 
    for (int i = 0; i < m; ++i) {
        std::cin >> points[i].first >> points[i].second;
    }

    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    auto getCompressedY = [&](int y) {
        return lower_bound(ys.begin(), ys.end(), y) - ys.begin();
    };

    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) {
            return std::get<0>(a) < std::get<0>(b); 
        }  
        return std::get<1>(a) > std::get<1>(b); 
    });

    PersistentSegmentTree pst;
    std::vector<PersistentSegmentTree::Node*> roots;
    roots.push_back(pst.build(0, ys.size() - 1));

    
    for (const auto& [x, type, value] : events) {
        if (type == 1) {
            int h = getCompressedY(value);
            roots.push_back(pst.update(roots.back(), 0, ys.size() - 1, h, 1));
        } else if (type == -1) {
            int h = getCompressedY(value);
            roots.push_back(pst.update(roots.back(), 0, ys.size() - 1, h, -1));
        }
    }

    
    std::vector<int> results(m); 
    bool flag = false;

    for (int i = 0; i < m; i++) {
        int px = points[i].first;
        int py = points[i].second;
        int compressedY = getCompressedY(py);

        int version = std::upper_bound(events.begin(), events.end(), std::make_tuple(px, 0, 0), [](const auto& first, const auto& second) {
            if (std::get<0>(first) != std::get<0>(second)) {
                return std::get<0>(first) < std::get<0>(second);
            }
            return std::get<1>(first) > std::get<1>(second);
        }) - events.begin();

        if (py == ys[compressedY]) {
            results[i] = pst.query(roots[version], 0, ys.size() - 1, compressedY + 1, ys.size() - 1);
        }
        else {
            results[i] = pst.query(roots[version], 0, ys.size() - 1, compressedY, ys.size() - 1);
        }
    }

    for (int res : results) {
        std::cout << res << "\n";
    }

    return 0;
}
