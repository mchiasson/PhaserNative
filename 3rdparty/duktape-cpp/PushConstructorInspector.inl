#pragma once

#include "PushConstructorInspector.h"

#include "Method.h"

namespace duk { namespace details {

inline PushConstructorInspector::PushConstructorInspector(duk::Context &ctx) : _ctx(ctx) {}

template <class C, class ... A>
inline void PushConstructorInspector::construct(std::shared_ptr<C> (*constructor) (A...)) {
    if (!_hasConstructor) {
        Constructor<C, A...>::push(_ctx, constructor);
        _hasConstructor = true;
    }
}

template <class C, class ... A>
inline void PushConstructorInspector::construct(std::unique_ptr<C> (*constructor) (A...)) {
    if (!_hasConstructor) {
        ConstructorUnique<C, A...>::push(_ctx, constructor);
        _hasConstructor = true;
    }
}

template <class C>
inline void PushConstructorInspector::construct(std::shared_ptr<C> (*factory)(duk::Context &ctx, duk_idx_t nargs)) {
    if (!_hasConstructor) {
        ConstructorVargs<C>::push(_ctx, factory);
        _hasConstructor = true;
    }
}

template <class A>
inline void PushConstructorInspector::static_property(const char *name, StaticGetter<A> getter, StaticSetter<A> setter) {
    duk_push_string(_ctx, name);
    PushMethod(_ctx, getter);
    PushMethod(_ctx, setter);
    duk_def_prop(
        _ctx,
        -4,
        DUK_DEFPROP_HAVE_GETTER |
        DUK_DEFPROP_HAVE_SETTER
    );
}

template <class A>
inline void PushConstructorInspector::static_property(const char *name, StaticGetter<A> getter) {
    duk_push_string(_ctx, name);
    PushMethod(_ctx, getter);
    duk_def_prop(
        _ctx,
        -3,
        DUK_DEFPROP_HAVE_GETTER
    );
}

template <class R, class ... A>
inline void PushConstructorInspector::function(const char *name, R(*method)(A...)) {
    PushMethod(_ctx, method);
    duk_put_prop_string(_ctx, -2, name);
}

inline void PushConstructorInspector::function_vargs(const char *name, duk_ret_t (*functionSelector)(duk::Context &ctx, duk_idx_t nargs)){
    PushFunctionVargs(_ctx, functionSelector);
    duk_put_prop_string(_ctx, -2, name);
}


}}
