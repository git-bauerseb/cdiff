#include "cdiff.h"

// Template for performing longest common subsequence on strings.
// Used in the character by character mode
template std::vector<std::vector<int>> Diff::lcs<std::string>(const std::string& seq1, const std::string& seq2);
template std::vector<DiffOperation<char>> Diff::determine_operations(std::string& seq1,
                                         std::string& seq2,
                                         std::vector<std::vector<int>> &lcs_table,
                                         int &num_unchanged);

// Template for performing longest common subsequence on hashes.
// Used for line mode, where the hash represents the string of characters in the line.
template std::vector<std::vector<int>> Diff::lcs<std::vector<long long>>(const std::vector<long long>& seq1, const std::vector<long long>& seq2);
template std::vector<DiffOperation<long long>> Diff::determine_operations(std::vector<long long>& seq1,
                                         std::vector<long long>& seq2,
                                         std::vector<std::vector<int>> &lcs_table,
                                         int &num_unchanged);


// DIFF OPERATION
template<typename ElType>
std::ostream& operator<<(std::ostream& o, const DiffOperation<ElType>& op) {
    switch (op.get_op_type()) {
        case DiffOpType::ADDITION: o << "ADDITION "; break;
        case DiffOpType::DELETION: o << "DELETION "; break;
        case DiffOpType::UNCHANGED: o << "UNCHANGED "; break;
    }

    o << op.get_manipulated() << " at " << op.get_position();
    return o;
}

template<typename DT>
DiffOpType DiffOperation<DT>::get_op_type() const {
    return m_op_type;
}

template<typename DT>
int DiffOperation<DT>::get_position() const {
    return m_position;
}

template<typename DT>
DT DiffOperation<DT>::get_manipulated() const {
    return m_manipulated;
}


// DIFF
// Public
Diff::Diff(std::string &first, std::string &second) : m_first{first}, m_second{second} {}

Diff::Diff(std::string first, std::string second) : m_first{std::move(first)}, m_second{std::move(second)} {}

std::string Diff::verbose_diff() {
    int num_unchanged = 0;

    auto lcs_table = lcs(m_first, m_second);
    auto operations = determine_operations(m_first, m_second, lcs_table, num_unchanged);


    std::string diff_string{};
    diff_string.resize(2 * (operations.size() - num_unchanged) + num_unchanged);

    int o_pos = 0;
    for (auto it = operations.rbegin(); it != operations.rend(); it++) {
        switch (it->get_op_type()) {
            case DiffOpType::ADDITION:
                diff_string[o_pos] = '+';
                diff_string[o_pos+1] = it->get_manipulated();
                o_pos += 2;
                break;
            case DiffOpType::DELETION:
                diff_string[o_pos] = '-';
                diff_string[o_pos+1] = it->get_manipulated();
                o_pos += 2;
                break;
            case DiffOpType::UNCHANGED:
                diff_string[o_pos] = it->get_manipulated();
                o_pos++;
                break;
        }
    }

    return diff_string;
}

std::string Diff::line_diff() {
    compute_line_hashes(m_first_hashes, m_first, true);
    compute_line_hashes(m_second_hashes, m_second, false);

    int num_unchanged = 0;

    auto lcs_table = lcs(m_first_hashes, m_second_hashes);
    auto ops = determine_operations(m_first_hashes, m_second_hashes,lcs_table, num_unchanged);

    std::string patch{};

    for (auto it = ops.rbegin(); it != ops.rend(); it++) {
        switch (it->get_op_type()) {
            case DiffOpType::ADDITION:
                patch += '+';
                break;
            case DiffOpType::DELETION:
                patch += '-';
                break;
            case DiffOpType::UNCHANGED: break;
        }

        if (m_line_hash_lookup.count(it->get_manipulated())) {
            auto line_info = m_line_hash_lookup[it->get_manipulated()];

            std::string s(std::get<0>(line_info),
                    std::get<0>(line_info) + std::get<1>(line_info));

            patch += ' ' + s;

            if (it != (ops.rend()-1)) {
                patch += '\n';
            }
        }
    }

    return patch;
}

// Private
void Diff::compute_line_hashes(std::vector<long long>& hash_vec,
                               std::string& lines, bool first) {
    const int n = lines.size();

    char* begin = &lines[0];
    char* end = &lines[0];

    long long hash_value = 0;
    long long p_pow = 1;
    const int p = 251;
    const int m = 1e9 + 9;

    for (int i = 0; i <= n; i++) {
        if (*end == '\n' || i == (n)) {
            hash_vec.push_back(hash_value);
            m_line_hash_lookup.insert({hash_value, {begin, end-begin, first}});
            end++;
            begin = end;
            hash_value = 0;
            p_pow = 1;
        } else {
            hash_value = (hash_value + ((int)*end)*p_pow) % m;
            p_pow = (p_pow * p) % m;
            end++;
        }
    }
}

template<typename Seq>
std::vector<std::vector<int>> Diff::lcs(const Seq& seq1, const Seq& seq2) {
    int n = seq1.size();
    int m = seq2.size();

    std::vector<std::vector<int>> c (m+1, std::vector<int>(n+1, 0));

    for (int j = 1; j < m+1; j++) {
        for (int i = 1; i < n+1; i++) {
            if (seq1[i-1] == seq2[j-1]) {
                c[j][i] = c[j-1][i-1] + 1;
            } else {
                c[j][i] = std::max(c[j-1][i], c[j][i-1]);
            }
        }
    }

    return c;
}

template<typename Seq>
std::vector<DiffOperation<typename Seq::value_type>> Diff::determine_operations(
        Seq& seq1,
        Seq& seq2,
        std::vector<std::vector<int>>& lcs_table, int& num_unchanged) {
    int i = seq1.size();
    int j = seq2.size();

    typedef typename Seq::value_type ElType;

    std::vector<DiffOperation<ElType>> operations;

    while (i != 0 || j != 0) {
        if (i == 0) {
            operations.push_back({DiffOpType::ADDITION, seq2[j-1], j-1});
            j--;
        } else if (j == 0) {
            operations.push_back({DiffOpType::DELETION, seq1[i-1], i-1});
            i--;
        } else if (seq1[i-1] == seq2[j-1]) {
            operations.push_back({DiffOpType::UNCHANGED, seq1[i-1], i-1});
            num_unchanged++;
            i--;
            j--;
        } else if (lcs_table[j-1][i] > lcs_table[j][i-1]) {
            operations.push_back({DiffOpType::ADDITION, seq2[j-1], j-1});
            j--;
        } else {
            operations.push_back({DiffOpType::DELETION, seq1[i-1], i-1});
            i--;
        }
    }

    return operations;
}