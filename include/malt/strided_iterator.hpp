//
// Created by fatih on 5/26/17.
//

#pragma once

#include <vector>
#include <cstdint>

namespace malt {
    template<class Of>
    Of* strided_advance(Of* ptr, std::size_t stride, std::size_t count = 1)
    {
        uint8_t* p = reinterpret_cast<uint8_t*>(ptr);
        p += stride*count;
        return reinterpret_cast<Of*>(p);
    }

    template<class BaseT>
    struct erased_container
    {
        BaseT* m_begin = nullptr, * m_end = nullptr;
        std::size_t m_stride = 0;

        const erased_container* m_next;

        static const erased_container* get_sentinel()
        {
            static erased_container cont(3);
            return &cont;
        }

        erased_container()
                :m_begin{}, m_end{}, m_stride{}, m_next{get_sentinel()}
        {
        }

        erased_container(BaseT* begin, BaseT* end, std::size_t stride)
                :m_begin{begin}, m_end{end}, m_stride{stride}, m_next{get_sentinel()}
        {
        }

        erased_container(const erased_container&) = default;
        erased_container(erased_container&&) = default;

        erased_container& operator=(const erased_container& rhs)
        {
            m_begin = rhs.m_begin;
            m_end = rhs.m_end;
            m_stride = rhs.m_stride;
            return *this;
        }

        erased_container& operator=(erased_container&& rhs)
        {
            m_begin = rhs.m_begin;
            m_end = rhs.m_end;
            m_stride = rhs.m_stride;
            return *this;
        }
    private:
        erased_container(int){}
    };

    template<class ActualT, class InternalT = ActualT>
    class stride_iterator;

    template<class T, class InternalT>
    stride_iterator<T, InternalT> begin(const erased_container<InternalT>& c);

    template<class T, class InternalT>
    stride_iterator<T, InternalT> end(const erased_container<InternalT>& c);

    template<class ExternalT, class InternalT>
    class stride_iterator : public std::iterator<std::forward_iterator_tag, InternalT>
    {
        static_assert(std::is_base_of<InternalT, ExternalT>{}, "Internal type must be the base of External type");
        const erased_container<InternalT>* m_current;
        InternalT* m_pos;

    public:
        stride_iterator(const erased_container<InternalT>& cont)
        {
            m_current = &cont;
            m_pos = m_current->m_begin;

            while (m_pos==m_current->m_end && m_current->m_next) {
                m_current = m_current->m_next;
                m_pos = m_current->m_begin;
            }
        }

        ExternalT& operator*()
        {
            return static_cast<ExternalT&>(*m_pos);
        }

        stride_iterator& operator++()
        {
            m_pos = strided_advance(m_pos, m_current->m_stride);

            while (m_pos==m_current->m_end && m_current->m_next) {
                m_current = m_current->m_next;
                m_pos = m_current->m_begin;
            }

            return *this;
        }

        bool operator==(const stride_iterator& other)
        { return other.m_pos==m_pos && other.m_current==m_current; }

        bool operator!=(const stride_iterator& other)
        { return other.m_pos!=m_pos && other.m_current!=m_current; }
    };

    template<class ExternalT, class InternalT>
    stride_iterator<ExternalT, InternalT> begin(const erased_container<InternalT>& c)
    {
        return {c};
    }

    template<class T, class InternalT>
    stride_iterator<T, InternalT> end(const erased_container<InternalT>& c)
    {
        return {*c.get_sentinel()};
    }

    template<class Base, class T>
    erased_container<Base> get_container(std::vector<T>& vec)
    {
        if (vec.empty()) {
            return { nullptr, nullptr, sizeof(T) };
        }
        Base* first = &vec.front();
        Base* end = &vec.back() + 1;
        return erased_container<Base>{first, end, sizeof(T)};
    }

    template<class T>
    struct erased_containers
    {
        erased_container<T> m_begin_container;
        std::vector<erased_container<T>> m_middle_containers;
        erased_container<T> m_end_container;
    };

    template<class ExternalT, class InternalT = ExternalT>
    class erased_range
    {
        stride_iterator<ExternalT, InternalT> m_begin;
    public:
        erased_range(const erased_container<InternalT>& from)
                :m_begin(malt::begin<ExternalT, InternalT>(from))
        { }

        stride_iterator<ExternalT, InternalT>& begin()
        {
            return m_begin;
        }

        stride_iterator<ExternalT, InternalT> end()
        {
            return {*erased_container<InternalT>::get_sentinel()};
        };
    };
}

