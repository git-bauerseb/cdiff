#include "cdiff.h"

// DIFF OPERATION
DiffOpType DiffOperation::get_op_type() const {
    return m_op_type;
}

int DiffOperation::get_position() const {
    return m_position;
}

char DiffOperation::get_char() const {
    return m_manipulated;
}


// DIFF
// Public
Diff::Diff(std::string &first, std::string &second) : m_first{first}, m_second{second} {}

Diff::Diff(std::string first, std::string second) : m_first{first}, m_second{second} {}

std::string Diff::verbose_patch() {

    int num_unchanged = 0;

    if (m_operations.empty()) {
        determine_operations(&num_unchanged);
    }

    std::string diff_string{};
    diff_string.resize(2 * (m_operations.size() - num_unchanged) + num_unchanged);


    int o_pos = 0;
    for (auto it = m_operations.rbegin(); it != m_operations.rend(); it++) {
        switch (it->get_op_type()) {
            case DiffOpType::ADDITION:
                diff_string[o_pos] = '+';
                diff_string[o_pos+1] = it->get_char();
                o_pos += 2;
                break;
            case DiffOpType::DELETION:
                diff_string[o_pos] = '-';
                diff_string[o_pos+1] = it->get_char();
                o_pos += 2;
                break;
            case DiffOpType::UNCHANGED:
                diff_string[o_pos] = it->get_char();
                o_pos++;
                break;
        }
    }

    return diff_string;
}


// Private
void Diff::lcs() {

    int n = m_first.size();
    int m = m_second.size();

    std::vector<std::vector<int>> c (m+1, std::vector<int>(n+1, 0));

    for (int j = 1; j < m+1; j++) {
        for (int i = 1; i < n+1; i++) {
            if (m_first[i-1] == m_second[j-1]) {
                c[j][i] = c[j-1][i-1] + 1;
            } else {
                c[j][i] = std::max(c[j-1][i], c[j][i-1]);
            }
        }
    }

    m_lcs_table = std::move(c);
}

void Diff::determine_operations(int* num_unchanged) {

    lcs();

    int i = m_first.size();
    int j = m_second.size();


    while (i != 0 || j != 0) {

        // Character is present in second string but not in first
        //  => Addition in second string
        if (i == 0) {
            m_operations.emplace_back(DiffOpType::ADDITION, m_second[j-1], j-1);
            j--;
        } else if (j == 0) {
            m_operations.emplace_back(DiffOpType::DELETION, m_first[i-1], i-1);
            i--;
        } else if (m_first[i-1] == m_second[j-1]) {
            m_operations.emplace_back(DiffOpType::UNCHANGED, m_first[i-1], i-1);
            num_unchanged++;
            i--;
            j--;
        } else if (m_lcs_table[j-1][i] > m_lcs_table[j][i-1]) {
            m_operations.emplace_back(DiffOpType::ADDITION, m_second[j-1], j-1);
            j--;
        } else {
            m_operations.emplace_back(DiffOpType::DELETION, m_first[i-1], i-1);
            i--;
        }
    }
}