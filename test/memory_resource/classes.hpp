#pragma once

#include <actor-zeta/detail/pmr/memory_resource.hpp>

using actor_zeta::detail::pmr::memory_resource;

class derived_from_memory_resource
    : public memory_resource {
public:
    explicit derived_from_memory_resource(unsigned i = 0u)
        : id(i) {}

    virtual ~derived_from_memory_resource() { destructor_called = true; }

    virtual void* do_allocate(std::size_t bytes, std::size_t alignment) {
        do_allocate_called = true;
        do_allocate_bytes = bytes;
        do_allocate_alignment = alignment;
        return do_allocate_return;
    }

    virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) {
        do_deallocate_called = true;
        do_deallocate_p = p;
        do_deallocate_bytes = bytes;
        do_deallocate_alignment = alignment;
    }

    virtual bool do_is_equal(const memory_resource& other) const noexcept {
        do_is_equal_called = true;
        do_is_equal_other = &other;
        return static_cast<const derived_from_memory_resource&>(other).id == this->id;
    }

    void reset() {
        destructor_called = false;

        do_allocate_return = 0;
        do_allocate_called = false;
        do_allocate_bytes = 0u;
        do_allocate_alignment = 0u;

        do_deallocate_called = false;
        do_deallocate_p = 0;
        do_deallocate_bytes = 0u;
        do_deallocate_alignment = 0u;

        do_is_equal_called = false;
        do_is_equal_other = 0;
    }

    static bool destructor_called;

    unsigned id;
    void* do_allocate_return;
    mutable bool do_allocate_called;
    mutable std::size_t do_allocate_bytes;
    mutable std::size_t do_allocate_alignment;

    mutable bool do_deallocate_called;
    mutable void* do_deallocate_p;
    mutable std::size_t do_deallocate_bytes;
    mutable std::size_t do_deallocate_alignment;

    mutable bool do_is_equal_called;
    mutable const memory_resource* do_is_equal_other;
};

bool derived_from_memory_resource::destructor_called = false;
