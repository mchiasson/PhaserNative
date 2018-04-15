#pragma once

#include "Context.h"

#include "EmptyInspector.h"
#include "Constructor.h"

namespace duk { namespace details {

class PushConstructorInspector: public EmptyInspector {
public:
    PushConstructorInspector(duk::Context &ctx);

    template <class C, class ... A>
    void construct(std::shared_ptr<C> (*constructor) (A...));

    template <class C, class ... A>
    void construct(std::unique_ptr<C> (*constructor) (A...));

    template <class C>
    void construct(std::shared_ptr<C> (*factory)(duk::Context &ctx, duk_idx_t nargs));

    template <class A>
    void static_property(const char *name, StaticGetter<A> getter, StaticSetter<A> setter);

    template <class A>
    void static_property(const char *name, StaticGetter<A> getter);

    template <class R, class ... A>
    void function(const char *name, R(*method)(A...));

    void function_vargs(const char *name, duk_ret_t (*methodSelector)(duk::Context &ctx, duk_idx_t nargs));

private:
    duk::Context &_ctx;
    bool _hasConstructor = false;
};

}}
