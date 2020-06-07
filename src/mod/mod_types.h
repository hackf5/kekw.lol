#pragma once

#include<boost/call_traits.hpp>
#include <string>

namespace kekw {
namespace mod {
    typedef int card_id_t;
    typedef boost::call_traits<card_id_t>::param_type card_id_param_t;
    typedef boost::call_traits<card_id_t>::const_reference card_id_ret_t;

    typedef std::string card_type_id_t;
    typedef boost::call_traits<card_type_id_t>::param_type card_type_id_param_t;
    typedef boost::call_traits<card_type_id_t>::const_reference card_type_id_ret_t;
}
}