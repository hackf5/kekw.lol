#pragma once

namespace kekw {
namespace tools {

template <typename TContainer, typename TConstIterator>
typename TContainer::iterator remove_constness(TContainer& c, TConstIterator it) {
    return c.erase(it, it);
}

}  // namespace tools
}  // namespace kekw
