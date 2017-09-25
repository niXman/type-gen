
#include <type-gen/type-gen.hpp>

#include <iostream>

#include <vector>
#include <string>
#include <type_traits>

/***************************************************************************/

TYPE_GEN_TYPE(
    type0
    ,
    ((int), a)
    ((double), b, (3.14))
)

/***************************************************************************/

bool test_init() {
    type0 t0;

    int v{};
    if ( t0.a != v ) {
        return false;
    }

    if ( t0.b != 3.14 ) {
        return false;
    }

    return true;
}

/***************************************************************************/

bool test_equal() {
    type0 t0, t1;

    if ( t0 != t1 ) {
        return false;
    }

    t1.a = 1; t1.b = 2;

    if ( t0 == t1 ) {
        return false;
    }

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
    if ( !ok || cnt != 0 ) {
        return false;
    }

    cnt = 0;
    t1.a = 1; t1.b = 2;

    std::vector<std::string> memnames;
    ok = t0.compare(t1,
        [&](const char *memname, const auto &, const auto &) {
            ++cnt;
            memnames.push_back(memname);
        }
    );
    if ( ok || cnt != t0._members_count() ) {
        return false;
    }
    if ( cnt != memnames.size() ) {
        return false;
    }
    if ( memnames[0] != "a" || memnames[1] != "b" ) {
        return false;
    }

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

    if ( memnames.size() != t0._members_count() || memaddrs.size() != t0._members_count() ) {
        return false;
    }

    if ( memnames[0] != "a" || memnames[1] != "b" ) {
        return false;
    }

    if ( memaddrs[0] != &(t0.a) || memaddrs[1] != &(t0.b) ) {
        return false;
    }

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

    if ( s.addrs.size() != t0._members_count() ) {
        return false;
    }

    if ( s.addrs[0] != &(t0.a) || s.addrs[1] != &(t0.b) ) {
        return false;
    }

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
    ((int), a)
    ((double), b)
)

bool test_typeinfo_equal() {
    type1 t0{}, t1{};

    typegen::typeinfo<type1> ti0(t0);
    typegen::typeinfo<type1> ti1(t1);

    if ( ti0 != ti1 ) {
        return false;
    }

    t0.a = 1; t0.b = 2;

    if ( ti0 == ti1 ) {
        return false;
    }
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
    if ( !ok || cnt != 0 ) {
        return false;
    }

    cnt = 0;
    t1.a = 1; t1.b = 2;

    std::vector<std::string> memnames;
    ok = ti0.compare(ti1,
        [&](const char *memname, const auto &, const auto &) {
            ++cnt;
            memnames.push_back(memname);
        }
    );
    if ( ok || cnt != ti0._members_count() ) {
        return false;
    }
    if ( cnt != memnames.size() ) {
        return false;
    }
    if ( memnames[0] != "a" || memnames[1] != "b" ) {
        return false;
    }

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

    if ( memnames.size() != ti0._members_count() || memaddrs.size() != ti0._members_count() ) {
        return false;
    }

    if ( memnames[0] != "a" || memnames[1] != "b" ) {
        return false;
    }

    if ( memaddrs[0] != &(t0.a) || memaddrs[1] != &(t0.b) ) {
        return false;
    }

    return true;
}

/***************************************************************************/

#define TEST(...) \
    if ( !(__VA_ARGS__) ) { \
        std::cerr << "expression error(" __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) "): \"" #__VA_ARGS__ "\"" << std::endl; \
        std::abort(); \
    }

int main() {
    TEST(test_init() == true)
    TEST(test_equal() == true)
    TEST(test_compare() == true)
    TEST(test_apply() == true)
    TEST(test_serialize() == true)

    TEST(test_typeinfo_equal() == true)
    TEST(test_typeinfo_compare() == true)
    TEST(test_typeinfo_apply() == true)
}

/***************************************************************************/
