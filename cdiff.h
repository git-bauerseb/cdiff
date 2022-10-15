#ifndef CDIFF_CDIFF_H
#define CDIFF_CDIFF_H

#include <string>
#include <vector>
#include <memory>

enum class DiffOpType {
    ADDITION,       // Represents an operation where a character was inserted (+C -> C)
    DELETION,       // Represents an operation where a character was deleted (-C -> )
    UNCHANGED       // Represents an operation where a character was not modified (C -> C)
};

class DiffOperation {
    public:
        DiffOperation(DiffOpType type, char obj, int pos) : m_position(pos), m_op_type(type), m_manipulated(obj) {}

        DiffOpType get_op_type() const;
        int get_position() const;
        char get_char() const;

    private:
        int m_position;
        char m_manipulated;
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
        std::string verbose_patch();


    private:

        // Compute the length of the longest common subsequence between
        // the two strings.
        void lcs();

        // Construct the operations necessary for retrieving the second string
        // from the first one.
        void determine_operations(int* num_unchanged);

        std::string m_first;
        std::string m_second;
        std::vector<DiffOperation> m_operations;
        std::vector<std::vector<int>> m_lcs_table;
};


#endif
