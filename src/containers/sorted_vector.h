#ifndef CONTAINERS__SORTED_VECTOR_H
#define CONTAINERS__SORTED_VECTOR_H

#include <functional>
#include <vector>

#include <set>

#include "assertions.h"

/**
 * checks inserted values if they are sorted
 */
template<typename T, typename Compare = std::less<T> >
class SortedVector :
        public std::vector<T>
{
public:
    typedef std::vector<T>  base;
    typedef Compare         compare;

    typedef std::binary_negate<compare>  neg_compare;

    typedef typename base::iterator    iterator;
    typedef typename base::size_type   size_type;
    typedef typename base::value_type  value_type;

    /** default constructor */
    SortedVector(const compare& comp = compare()) :
        base(),
        comp_(comp),
        neg_comp_(comp_)
    {
    }

    /**
     * @see std::vector::insert()
     */
    void insert(iterator __position,
                size_type __n,
                const value_type &__x)
    {
        precondition(sorted_position(__position, __x));
        base::insert(__position, __n, __x);
    }

    /**
     * @see std::vector::insert()
     */
    template<typename _InputIterator>
    void insert(iterator __position, _InputIterator __first, _InputIterator __last)
    {
        precondition(sorted_position(__position, *__first)
                     && sorted_position(__position, *__last)
                     && (std::adjacent_find(this->begin(), this->end(), neg_comp_) == this->end()));
        base::insert(__position, __first, __last);
    }

    /**
     * @see std::vector::push_back()
     */
    void push_back(const value_type& __x)
    {
        precondition(sorted_position(this->end(), __x));
        base::push_back(__x);
    }
private:
    compare      comp_;
    neg_compare  neg_comp_;

    bool sorted_position(iterator __position,
                         const value_type& __x) const
    {
        return (this->empty()
                || ((__position == this->end())
                     && comp_(this->back(), __x))
                || (comp_(*__position, __x)
                    && ((__position + 1 == this->end())
                        || comp_(__x, *(__position + 1)))
                    )
                );
    }
};

#endif // CONTAINERS__SORTED_VECTOR_H
