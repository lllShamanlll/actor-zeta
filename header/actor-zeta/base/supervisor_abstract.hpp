#pragma once

#include <actor-zeta/base/communication_module.hpp>
#include <actor-zeta/detail/pmr/memory_resource.hpp>

namespace actor_zeta { namespace base {
    class supervisor_abstract : public communication_module {
    public:
        supervisor_abstract(detail::pmr::memory_resource*, std::string, int64_t );
        template<class Supervisor>
        supervisor_abstract(Supervisor*supervisor, std::string type, int64_t actor_id)
            : supervisor_abstract(static_cast<supervisor_abstract*>(supervisor),std::move(type),actor_id){}
        ~supervisor_abstract() override;
        auto executor() noexcept -> executor::abstract_executor*;
        auto resource() const -> detail::pmr::memory_resource*;
        auto address() noexcept -> address_t;

    protected:
        using communication_module::add_handler;
        virtual auto executor_impl() noexcept -> executor::abstract_executor* = 0;
        auto set_current_message(message_ptr) -> void;
        auto current_message_impl() -> message* final;

    private:
        supervisor_abstract(supervisor_abstract*, std::string, int64_t );
        message* current_message_;
        detail::pmr::memory_resource* memory_resource_;
    };

    template<class Supervisor>
    class cooperative_supervisor : public  supervisor_abstract {
    public:
        using supervisor_abstract::executor;
        using supervisor_abstract::resource;
        using supervisor_abstract::address;
        using supervisor_abstract::supervisor_abstract;
    protected:
        template<
            class Actor,
            class Inserter,
            class... Args,
            class = type_traits::enable_if_t<std::is_base_of<actor_abstract, Actor>::value>>
        auto spawn_actor(const Inserter& inserter, Args&&... args) -> address_t {
            auto allocate_byte = sizeof(Actor);
            auto allocate_byte_alignof = alignof(Actor);
            void* buffer = resource()->allocate(allocate_byte, allocate_byte_alignof);
            auto* actor = new (buffer) Actor(static_cast<Supervisor*>(this), std::forward<Args>(args)...);
            auto address = actor->address();
            inserter(actor);
            return address;
        }

        template<
            class SupervisorChildren,
            class Inserter,
            class... Args,
            class = type_traits::enable_if_t<std::is_base_of<supervisor_abstract, SupervisorChildren>::value>>
        auto spawn_supervisor(const Inserter& inserter, Args&&... args) -> address_t {
            auto allocate_byte = sizeof(SupervisorChildren);
            auto allocate_byte_alignof = alignof(SupervisorChildren);
            void* buffer = resource()->allocate(allocate_byte, allocate_byte_alignof);
            auto* supervisor = new (buffer) SupervisorChildren(static_cast<Supervisor*>(this), std::forward<Args>(args)...);
            auto address = supervisor->address();
            inserter(supervisor);
            return address;
        }

    };

}} // namespace actor_zeta::base
