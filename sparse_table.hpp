#ifndef SPARSE_TABLE_HPP
#define SPARSE_TABLE_HPP

#include "RMQ.hpp"

#include <vector>
#include <boost/multi_array.hpp>


template < typename T, typename Sequence = std::vector<T> >
class sparse_table
{
    typedef typename Sequence::distance_type distance_type;
    Sequence c;
    boost::multi_array<distance_type, 2> M;
    
public:
    typedef typename Sequence::value_type                value_type;
    typedef typename Sequence::reference                 reference;
    typedef typename Sequence::const_reference           const_reference;
    typedef typename Sequence::size_type                 size_type;
    typedef          Sequence                            container_type;

#if __cplusplus < 201103L
    explicit sparse_table(const Sequence& c = Sequence()) : c(c) { }
#else
    explicit sparse_table(const Sequence& c) : c(c) { }
    explicit sparse_table(Sequence&& c = Sequence()) : c(std::move(c)) { }
#endif

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    
    void push_back(const value_type& __x) { c.push_back(__x); }
    void push_front(const value_type& __x) { c.push_front(__x); }
    
#if __cplusplus >= 201103L
    void push_back(value_type&& __x) { c.push_back(std::move(__x)); }

    template<typename... Args>
    void emplace_back(Args && ... args)
    { c.emplace_back(std::forward<Args>(args)...); }
#endif

};

#endif
