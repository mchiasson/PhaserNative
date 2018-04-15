#pragma once

#include <cassert>

#include "Constructor.h"
#include "Context.h"

#include "./Type.h"
#include "./Utils/Helpers.h"

#include "PushObjectInspector.h"

namespace duk { namespace details {

// Shared ptr constructor

template <int StackIdx, class C, class ... A>
struct ConstructorDispatcher {
    duk_ret_t dispatch(std::shared_ptr<C> (*constructor)(A...), duk::Context &d) {
        std::shared_ptr<C> res = call(constructor, d, std::index_sequence_for<A...>{});
        Type<std::shared_ptr<ClearType<C>>>::push(d, res);
        return 1;
    }

    template<std::size_t ... I>
    std::shared_ptr<C> call(std::shared_ptr<C> (*constructor)(A...), duk::Context &d, std::index_sequence<I...>) {
        return constructor(getArg<A, I + StackIdx>(d)...);
    }

    template <class AA, int Index>
    static ClearType<AA> getArg(duk::Context &d) {
        ClearType<AA> instance;
        Type<ClearType<AA>>::get(d, instance, Index);
        return instance;
    }
};

template <class C, class ... A>
inline void Constructor<C, A...>::push(duk::Context &d, std::shared_ptr<C> (*constructor) (A...)) {
    assert(constructor);

    duk_push_c_function(d, func, sizeof...(A));
    duk_push_pointer(d, (void*)constructor);
    duk_put_prop_string(d, -2, DUK_HIDDEN_SYMBOL("func_ptr"));
}

template <class C, class ... A>
inline duk_ret_t Constructor<C, A...>::func(duk_context *d) {
   if (!duk_is_constructor_call(d)) {
      duk_error(d, DUK_RET_TYPE_ERROR, "Constructor must be called with 'new'.");
      return DUK_RET_TYPE_ERROR;
   }
    
   duk_push_global_stash(d);
   duk_get_prop_string(d, -1, "self_ptr");
   duk::Context *ctx = reinterpret_cast<duk::Context*>(duk_get_pointer(d, -1));
   duk_pop_2(d);

   assert(ctx);

   duk_push_current_function(d);
   duk_get_prop_string(d, -1, DUK_HIDDEN_SYMBOL("func_ptr"));
   TFunc f = reinterpret_cast<TFunc>(duk_get_pointer(d, -1));
   duk_pop_2(d);

   assert(f);

   ConstructorDispatcher<0, C, A...> dispatcher;
   return dispatcher.dispatch(f, *ctx);
}


// Unique ptr constructor

template <int StackIdx, class C, class ... A>
struct ConstructorDispatcherUnique {
    duk_ret_t dispatch(std::unique_ptr<C> (*constructor)(A...), duk::Context &d) {
        std::unique_ptr<C> res = call(constructor, d, std::index_sequence_for<A...>{});
        Type<std::unique_ptr<ClearType<C>>>::push(d, std::move(res));
        return 1;
    }

    template<std::size_t ... I>
    std::unique_ptr<C> call(std::unique_ptr<C> (*constructor)(A...), duk::Context &d, std::index_sequence<I...>) {
        return constructor(getArg<A, I + StackIdx>(d)...);
    }

    template <class AA, int Index>
    static ClearType<AA> getArg(duk::Context &d) {
        ClearType<AA> instance;
        Type<ClearType<AA>>::get(d, instance, Index);
        return instance;
    }
};

template <class C, class ... A>
inline void ConstructorUnique<C, A...>::push(duk::Context &d, std::unique_ptr<C> (*constructor) (A...)) {
    assert(constructor);

    duk_push_c_function(d, func, sizeof...(A));
    duk_push_pointer(d, (void*)constructor);
    duk_put_prop_string(d, -2, DUK_HIDDEN_SYMBOL("func_ptr"));
}

template <class C, class ... A>
inline duk_ret_t ConstructorUnique<C, A...>::func(duk_context *d) {
   if (!duk_is_constructor_call(d)) {
      duk_error(d, DUK_RET_TYPE_ERROR, "Constructor must be called with 'new'.");
      return DUK_RET_TYPE_ERROR;
   }
    
   duk_push_global_stash(d);
   duk_get_prop_string(d, -1, "self_ptr");
   duk::Context *ctx = reinterpret_cast<duk::Context*>(duk_get_pointer(d, -1));
   duk_pop_2(d);

   assert(ctx);

   duk_push_current_function(d);
   duk_get_prop_string(d, -1, DUK_HIDDEN_SYMBOL("func_ptr"));
   TFunc f = reinterpret_cast<TFunc>(duk_get_pointer(d, -1));
   duk_pop_2(d);

   assert(f);

   ConstructorDispatcherUnique<0, C, A...> dispatcher;
   return dispatcher.dispatch(f, *ctx);
}

// Variable Argument Constructor-Factory

template <class C>
struct ConstructorVargsDispatcher {
    duk_ret_t dispatch(std::shared_ptr<C> (*factory)(duk::Context &ctx, duk_idx_t nargs), duk_idx_t nargs, duk::Context &d) {
        std::shared_ptr<C> res = call(factory, nargs, d);
        if (!res) {
            duk_error(d, DUK_RET_SYNTAX_ERROR, "Invalid Argument(s)");
            return DUK_RET_SYNTAX_ERROR;
        }
        Type<std::shared_ptr<ClearType<C>>>::push(d, res);
        return 1;
    }

    std::shared_ptr<C> call(std::shared_ptr<C> (*factory)(duk::Context &ctx, duk_idx_t nargs), duk_idx_t nargs, duk::Context &d) {
        return factory(d, nargs);
    }

    template <class AA, int Index>
    static ClearType<AA> getArg(duk::Context &d) {
        ClearType<AA> instance;
        Type<ClearType<AA>>::get(d, instance, Index);
        return instance;
    }
};

template <class C>
inline void ConstructorVargs<C>::push(duk::Context &d, ConstructorVargs<C>::TFactory factory) {
    assert(factory);
    duk_push_c_function(d, func, DUK_VARARGS);
    duk_push_pointer(d, (void*)factory);
    duk_put_prop_string(d, -2, DUK_HIDDEN_SYMBOL("factory_ptr"));
}

template <class C>
duk_ret_t ConstructorVargs<C>::func(duk_context *d) {
    if (!duk_is_constructor_call(d)) {
       duk_error(d, DUK_RET_TYPE_ERROR, "Constructor must be called with 'new'.");
       return DUK_RET_TYPE_ERROR;
    }

    duk_push_global_stash(d);
    duk_get_prop_string(d, -1, "self_ptr");
    duk::Context *ctx = reinterpret_cast<duk::Context*>(duk_get_pointer(d, -1));
    duk_pop_2(d);

    assert(ctx);

    duk_push_current_function(d);
    duk_get_prop_string(d, -1, DUK_HIDDEN_SYMBOL("factory_ptr"));
    TFactory f = reinterpret_cast<TFactory>(duk_get_pointer(d, -1));
    duk_pop_2(d);

    assert(f);

    duk_idx_t nargs = duk_get_top(d);

    ConstructorVargsDispatcher<C> dispatcher;
    return dispatcher.dispatch(f, nargs, *ctx);
}

}}
