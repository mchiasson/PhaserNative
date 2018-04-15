#include "../Type.h"

namespace duk {

template <>
struct Type<int8_t> {
    static void push(duk::Context &d, int8_t val) { duk_push_int(d, val); }
    static void get(duk::Context &d, int8_t &val, duk_idx_t index) { val = static_cast<int8_t>(duk_require_int(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<uint8_t> {
    static void push(duk::Context &d, uint8_t val) { duk_push_uint(d, val); }
    static void get(duk::Context &d, uint8_t &val, duk_idx_t index) { val = static_cast<int8_t>(duk_require_uint(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<int16_t> {
    static void push(duk::Context &d, int16_t val) { duk_push_int(d, val); }
    static void get(duk::Context &d, int16_t &val, duk_idx_t index) { val = static_cast<int16_t>(duk_require_int(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<uint16_t> {
    static void push(duk::Context &d, uint16_t val) { duk_push_uint(d, val); }
    static void get(duk::Context &d, uint16_t &val, duk_idx_t index) { val = static_cast<uint16_t>(duk_require_uint(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<int32_t> {
    static void push(duk::Context &d, int32_t val) { duk_push_int(d, val); }
    static void get(duk::Context &d, int32_t &val, duk_idx_t index) { val = static_cast<int32_t>(duk_require_int(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<uint32_t> {
    static void push(duk::Context &d, uint32_t val) { duk_push_uint(d, val); }
    static void get(duk::Context &d, uint32_t &val, duk_idx_t index) { val = static_cast<uint32_t>(duk_require_uint(d, index)); }
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<const char *> {
    static void push(duk::Context &d, const char *val) {duk_push_string(d, val);}
    static void get(duk::Context &d, const char *&val, int index) { val = duk_require_string(d, index);}
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<float> {
    static void push(duk::Context &d, float val) { duk_push_number(d, val);}
    static void get(duk::Context &d, float &val, int index) {val = static_cast<float>(duk_require_number(d, index));}
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<double> {
    static void push(duk::Context &d, double val) { duk_push_number(d, val); }
    static void get(duk::Context &d, double &val, int index) { val = duk_require_number(d, index);}
    static constexpr bool isPrimitive() { return true; };
};

template <>
struct Type<bool> {
    static void push(duk::Context &d, bool val) { duk_push_boolean(d, val);}
    static void get(duk::Context &d, bool &val, int index) { val = bool(duk_require_boolean(d, index));}
    static constexpr bool isPrimitive() { return true; };
};

}
