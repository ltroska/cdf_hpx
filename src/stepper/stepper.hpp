#ifndef STEPPER_STEPPER_HPP
#define STEPPER_STEPPER_HPP

#include "server/stepper_server.hpp"

namespace stepper {

struct stepper
    : hpx::components::client_base<stepper, server::stepper_server>
{
    typedef hpx::components::client_base<stepper, server::stepper_server> base_type;

    stepper()
      : base_type(hpx::new_<server::stepper_server>
          (hpx::find_here(), hpx::get_num_localities_sync()))
    {
        hpx::register_with_basename(server::stepper_basename, get_id(), hpx::get_locality_id());
    }

    // construct new instances/wrap existing steppers from other localities
    stepper(hpx::id_type loc)
      : base_type(hpx::new_<server::stepper_server>
          (loc, hpx::get_num_localities_sync()))
    {
        hpx::register_with_basename(
            server::stepper_basename, get_id(), hpx::get_locality_id());
    }

    stepper(hpx::future<hpx::id_type> && id)
      : base_type(std::move(id))
    {}

    hpx::future<void> setup(io::config cfg)
    {
        server::stepper_server::setup_action act;
        return hpx::async(act, get_id(), cfg);
    }
};


}//namespace stepper

#endif
