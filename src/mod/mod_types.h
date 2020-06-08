#pragma once

#include <boost/call_traits.hpp>
#include <string>

namespace kekw {
namespace mod {

typedef int card_id_t;
typedef boost::call_traits<card_id_t>::param_type card_id_param_t;
typedef boost::call_traits<card_id_t>::const_reference card_id_ret_t;

typedef std::string card_defn_id_t;
typedef boost::call_traits<card_defn_id_t>::param_type card_defn_id_param_t;
typedef boost::call_traits<card_defn_id_t>::const_reference card_defn_id_ret_t;

}  // namespace mod
}  // namespace kekw
