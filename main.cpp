
#include <type-gen/type-gen.hpp>

#include <iostream>

#include <vector>
#include <string>
#include <type_traits>

#define TEST_EXPR(cond) \
    if ( cond ) { \
        std::cerr << __FILE__ << "(" << __LINE__ << "): expression error: " << #cond << std::endl; \
        return false; \
    }

/***************************************************************************/

TYPE_GEN_TYPE(
    type0
    ,
    ((int), a)
    ((double), b, (3.14))
)

/***************************************************************************/

TYPE_GEN_TYPE(
    init_type0
    ,
    ((const int), a)
    ((const double), b)
    ((const std::array<int, 2>), c)
)
TYPE_GEN_TYPE(
    init_type1
    ,
    ((const int), a, (1))
    ((const double), b, (3.14))
    ((const std::array<int, 2>), c, ({2,3}))
)
TYPE_GEN_TYPE(
    init_type2
    ,
    ((const int &), a)
    ((const double &), b)
    ((const std::array<int, 2> &), c)
)

bool test_init() {
    type0 t0;

    int v{};
    TEST_EXPR(t0.a != v);

    TEST_EXPR(t0.b != 3.14);

    init_type2 it2{1, 3.14, {{2,3}}};

    return true;
}

/***************************************************************************/

bool test_equal() {
    type0 t0, t1;

    TEST_EXPR(t0 != t1);

    t1.a = 1; t1.b = 2;

    TEST_EXPR(t0 == t1);

    return true;
}

/***************************************************************************/

bool test_compare() {
    type0 t0, t1;

    std::size_t cnt{};
    bool ok = t0.compare(t1,
        [&cnt](const char *, const auto &, const auto &) {
            ++cnt;
        }
    );
    TEST_EXPR(!ok || cnt != 0);

    cnt = 0;
    t1.a = 1; t1.b = 2;

    std::vector<std::string> memnames;
    ok = t0.compare(t1,
        [&](const char *memname, const auto &, const auto &) {
            ++cnt;
            memnames.push_back(memname);
        }
    );
    TEST_EXPR(ok || cnt != t0._members_count());

    TEST_EXPR(cnt != memnames.size());

    TEST_EXPR(memnames[0] != "a" || memnames[1] != "b");

    return true;
}

/***************************************************************************/

bool test_apply() {
    type0 t0;

    std::vector<std::string> memnames;
    std::vector<const void*> memaddrs;

    t0.apply(
        [&](const char *memname, const auto &v){
            memnames.push_back(memname);
            memaddrs.push_back(&v);
        }
    );

    TEST_EXPR(memnames.size() != t0._members_count() || memaddrs.size() != t0._members_count());

    TEST_EXPR(memnames[0] != "a" || memnames[1] != "b");

    TEST_EXPR(memaddrs[0] != &(t0.a) || memaddrs[1] != &(t0.b));

    return true;
}

/***************************************************************************/

struct serializer {
    template<typename T>
    serializer& operator& (T &v) {
        return save(v);
    }

    template<typename T>
    serializer& save(T &v, typename std::enable_if<std::is_fundamental<T>::value>::type* = 0) {
        addrs.push_back(&v);

        return *this;
    }
    template<typename T>
    serializer& save(T &v, typename std::enable_if<!std::is_fundamental<T>::value>::type* = 0) {
        v.serialize(*this);

        return *this;
    }

    std::vector<const void *> addrs;
};

bool test_serialize() {
    type0 t0;

    serializer s;
    s & t0;

    TEST_EXPR(s.addrs.size() != t0._members_count());

    TEST_EXPR(s.addrs[0] != &(t0.a) || s.addrs[1] != &(t0.b));

    return true;
}

/***************************************************************************/

struct type1 {
    int a;
    double b;
};

TYPE_GEN_ADAPT(
     type1
    ,
    (a)
    (b)
)

bool test_typeinfo_equal() {
    type1 t0{}, t1{};

    typegen::typeinfo<type1> ti0(t0);
    typegen::typeinfo<type1> ti1(t1);

    TEST_EXPR(ti0 != ti1);

    t0.a = 1; t0.b = 2;

    TEST_EXPR(ti0 == ti1);

    return true;
}

/***************************************************************************/

bool test_typeinfo_compare() {
    type1 t0{}, t1{};

    typegen::typeinfo<type1> ti0(t0);
    typegen::typeinfo<type1> ti1(t1);

    std::size_t cnt{};
    bool ok = ti0.compare(ti1,
        [&cnt](const char *, const auto &, const auto &) {
            ++cnt;
        }
    );
    TEST_EXPR(!ok || cnt != 0);

    cnt = 0;
    t1.a = 1; t1.b = 2;

    std::vector<std::string> memnames;
    ok = ti0.compare(ti1,
        [&](const char *memname, const auto &, const auto &) {
            ++cnt;
            memnames.push_back(memname);
        }
    );
    TEST_EXPR(ok || cnt != ti0._members_count());

    TEST_EXPR(cnt != memnames.size());

    TEST_EXPR(memnames[0] != "a" || memnames[1] != "b");

    return true;
}

/***************************************************************************/

bool test_typeinfo_apply() {
    type1 t0{};

    typegen::typeinfo<type1> ti0(t0);

    std::vector<std::string> memnames;
    std::vector<const void*> memaddrs;

    ti0.apply(
        [&](const char *memname, const auto &v){
            memnames.push_back(memname);
            memaddrs.push_back(&v);
        }
    );

    TEST_EXPR(memnames.size() != ti0._members_count() || memaddrs.size() != ti0._members_count());

    TEST_EXPR(memnames[0] != "a" || memnames[1] != "b");

    TEST_EXPR(memaddrs[0] != &(t0.a) || memaddrs[1] != &(t0.b));

    return true;
}

/***************************************************************************/

int main() {
    TEST_EXPR(test_init() != true);
    TEST_EXPR(test_equal() != true)
    TEST_EXPR(test_compare() != true)
    TEST_EXPR(test_apply() != true)
    TEST_EXPR(test_serialize() != true)

    TEST_EXPR(test_typeinfo_equal() != true)
    TEST_EXPR(test_typeinfo_compare() != true)
    TEST_EXPR(test_typeinfo_apply() != true)
}

/***************************************************************************/
