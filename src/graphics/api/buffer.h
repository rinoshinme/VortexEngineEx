#pragma once

#include <vector>
#include <string>


namespace vortex
{
    struct BufferElement
    {
        const std::string name;

    };

    struct BufferLayout
    {
    private:
        std::vector<BufferElement> m_Elements;

    public:
        BufferLayout();
    };

    class VertexBuffer
    {
    public:
        
        
    };

}
