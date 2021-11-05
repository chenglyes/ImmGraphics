#ifndef __IMMGRAPHICS_HEADER_UTILS__
#define __IMMGRAPHICS_HEADER_UTILS__

#include <cstring>

#ifdef _NO_ASSERT
    #define _ASSERT
#endif

#ifndef _ASSERT
    #include <cassert>
    #define _ASSERT assert
#endif // !_ASSERT

namespace ImmGraphics
{

    /**
     * @brief A lightweight container that supports dynamic growth
     *  instead of std::vector<>.
     */
    template<typename T>
    class Container
    {
    public:
        Container() : m_size(0), m_capacity(0), m_data(nullptr) {}
        Container(unsigned size): Container() { Resize(size); memset(m_data, 0, size * sizeof(T)); }
        Container(const Container<T>& src): Container() { operator=(src); }
        Container<T>& operator=(const Container<T>& src)
        {
            Clear();
            Resize(src.m_size);
            memcpy(m_data, src.m_data, m_size * sizeof(T));
            return *this;
        }
        ~Container() { if (m_data) free(m_data); }

        void Resize(unsigned size) { if (size > m_capacity) Reserve(growCapacity(size)); m_size = size; }
        void Reserve(unsigned capacity)
        {
            if (m_capacity >= capacity) return;
            T* data = (T*)malloc(capacity * sizeof(T));
            _ASSERT(data && "Memory allocation failed.");
            memcpy(data, m_data, m_size * sizeof(T));
            free(m_data);
            m_data = data;
            m_capacity = capacity;
        }
        void Clear()
        {
            if (m_data)
            {
                m_size = 0;
                m_capacity = 0;
                free(m_data);
                m_data = nullptr;
            }
        }
        void SwapTo(Container& other)
        {
            unsigned tmp = other.m_size;
            other.m_size = m_size;
            m_size = tmp;

            tmp = other.m_capacity;
            other.m_capacity = m_capacity;
            m_capacity = tmp;

            T* ptmp = other.m_data;
            other.m_data = m_data;
            m_data = ptmp;
        }

        void PushBack(const T& element)
        {
            if (m_size == m_capacity) Reserve(growCapacity(m_size + 1));
            memcpy(m_data + m_size, &element, sizeof(element));
            ++m_size;
        }
        void PopBack() { _ASSERT(m_size > 0 && "The container is empty."); --m_size; }

        bool isEmpty() const { return m_size == 0; }
        unsigned getSize() const { return m_size; }
        unsigned getByteSize() const { return m_size * sizeof(T); }
        unsigned getCapacity() const { return m_capacity; }

        T& operator[](unsigned i) { _ASSERT(i < m_size && "The index is out of bounds."); return m_data[i]; }
        const T& operator[](unsigned i) const { _ASSERT(i < m_size && "The index is out of bounds."); return m_data[i]; }

        T& getFront() { _ASSERT(m_size > 0 && "The container is empty."); return m_data[0]; }
        const T& getFront() const { _ASSERT(m_size > 0 && "The container is empty."); return m_data[0]; }
        T& getBack() { _ASSERT(m_size > 0 && "The container is empty."); return m_data[m_size - 1]; }
        const T& getBack() const { _ASSERT(m_size > 0 && "The container is empty."); return m_data[m_size - 1]; }

        T* begin() { return m_data; }
        const T* begin() const { return m_data; }
        T* end() { return m_data + m_size; }
        const T* end() const { return m_data + m_size; }
        
    private:
        unsigned growCapacity(unsigned capacity)
        {
            unsigned new_capacity = m_capacity ? (m_capacity + m_capacity >> 1) : 8;
            return new_capacity > capacity ? new_capacity : capacity;
        }

    private:
        unsigned m_size;
        unsigned m_capacity;
        T * m_data;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_UTILS__
