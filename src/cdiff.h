#ifndef CDIFF_CDIFF_H
#define CDIFF_CDIFF_H

#include <iostream>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

enum class DiffOpType {
    ADDITION,       // Represents an operation where a character was inserted (+C -> C)
    DELETION,       // Represents an operation where a character was deleted (-C -> )
    UNCHANGED       // Represents an operation where a character was not modified (C -> C)
};

template<typename DT>
class DiffOperation {
    public:
        DiffOperation(DiffOpType type, DT obj, int pos)
            : m_position(pos), m_op_type(type), m_manipulated(obj) {}

        DiffOpType get_op_type() const;
        int get_position() const;
        DT get_manipulated() const;

    private:
        int m_position;
        DT m_manipulated;
        DiffOpType m_op_type;
};


// Class representing diff operation between two strings.
class Diff {

    public:
        Diff(std::string& first, std::string& second);
        Diff(std::string first, std::string second);

        // Compute a verbose patch string, that is a string where every
        // deletion and insertion is represented. For very large strings,
        // this becomes unreadable and consumes too much memory.
        std::string verbose_diff();


        std::string line_diff();

    private:


        void compute_line_hashes(std::vector<long long>& hash_vec, std::string& lines, bool first);

        // Compute the length of the longest common subsequence between
        // the two strings.
        template<typename Seq>
        std::vector<std::vector<int>> lcs(const Seq& seq1, const Seq& seq2);


        // Given the sequences for which the longest common subsequence was computed
        // and the corresponding table, computes the sequence of addition and delete
        // operations necessary to transform the first sequence to the second sequence.
        // Returns the resulting diff operations.
        template<typename Seq>
        std::vector<DiffOperation<typename Seq::value_type>> determine_operations(
                Seq& seq1,
                Seq& seq2,
                std::vector<std::vector<int>>& lcs_table,
                int& num_unchanged);

        std::string m_first;
        std::string m_second;

        // Hashes for the lines in the first file
        std::vector<long long> m_first_hashes;

        // Hashes for the lines in the second file
        std::vector<long long> m_second_hashes;

        std::unordered_map<long long, std::tuple<char*, int, bool>> m_line_hash_lookup;
};

#endif
