#include "Sampler.h"

namespace game
{

Sampler::Sampler()
    : m_Handle(0u, [](auto sampler) { ::glDeleteSamplers(1, &sampler); })
{
    ::glCreateSamplers(1, &m_Handle);
}

::GLuint Sampler::Native_Handle() const
{
    return m_Handle;
}

}
