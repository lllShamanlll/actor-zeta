#include <cassert>

#include <actor-zeta.hpp>
#include <actor-zeta/base/message.hpp>

using actor_zeta::base::address_t;
using actor_zeta::base::message;

int main() {
    message msg;

    assert(msg == false);

    message msg_1(address_t(), "1", int(1));

    assert(msg_1 == true);

    assert(msg_1.command() == "1");

    assert(msg_1.body<int>() == 1);

    return 0;
}