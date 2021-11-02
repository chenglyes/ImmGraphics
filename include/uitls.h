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
     * @brief A light and fast container based on dynamic array.
     *  It supports push and traverse method only now.
     */
    template<typename T>
    class Container
    {
    public:
        Container(unsigned size)
        {
            _ASSERT(size > 0 && "An empty container can not be defined.");
            m_data = (T*)malloc(size * sizeof(T));
            _ASSERT(m_data && "An error occurred while allocating memory.");
            m_size = 0;
            m_reserved = size;
        }

        ~Container()
        {
            _ASSERT(m_data && "An error occurred while releasing memory.");
            for (unsigned i = 0; i < m_reserved; ++i)
            {
                delete (m_data + i);
            }
        }

        void Resize(unsigned size)
        {
            _ASSERT(size > 0 && "An empty container can not be defined.");
            T* old_data = m_data;
            unsigned old_reserved = m_reserved;
            m_data = (T*)malloc(size * sizeof(T));
            m_size = m_size <= size ? m_size : size;
            m_reserved = size;
            memcpy(m_data, old_data, m_size * sizeof(T));
            for (unsigned i = 0; i < old_reserved; ++i)
            {
                delete (old_data + i);
            }
        }

        void PushBack(const T& element)
        {
            _ASSERT(m_size < m_reserved && "The container is full, not enough to push a new element.");
            *(m_data + m_size) = element;
            m_size++;
        }

        T* begin() { return m_data; }
        T* end() { return m_data + m_size; }
        const T* begin() const { return m_data; }
        const T* end() const { return m_data + m_size; }

    private:
        unsigned m_size;
        unsigned m_reserved;
        T * m_data;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_UTILS__
