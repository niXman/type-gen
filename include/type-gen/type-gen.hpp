
#ifndef __type_gen__type_gen_hpp
#define __type_gen__type_gen_hpp

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <functional>

/***************************************************************************/

#define TYPE_GEN_TYPE_WRAP_X(...) ((__VA_ARGS__)) TYPE_GEN_TYPE_WRAP_Y
#define TYPE_GEN_TYPE_WRAP_Y(...) ((__VA_ARGS__)) TYPE_GEN_TYPE_WRAP_X
#define TYPE_GEN_TYPE_WRAP_X0
#define TYPE_GEN_TYPE_WRAP_Y0

#define TYPE_GEN_TYPE_DECLARE_MEMBER_WITH_INIT(elem) \
    BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(0, elem)) /* type */ \
    BOOST_PP_TUPLE_ELEM(1, elem) /* var */ { BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(2, elem)) }; /* init */

#define TYPE_GEN_TYPE_DECLARE_MEMBER_WITHOUT_INIT(elem) \
    BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(0, elem)) /* type */ \
    BOOST_PP_TUPLE_ELEM(1, elem) /* var */ {};

#define TYPE_GEN_TYPE_DECLARE_MEMBER(unused0, unused1, idx, elem) \
    BOOST_PP_IF( \
         BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 3) \
        ,TYPE_GEN_TYPE_DECLARE_MEMBER_WITH_INIT \
        ,TYPE_GEN_TYPE_DECLARE_MEMBER_WITHOUT_INIT \
    )(elem)

#define TYPE_GEN_TYPE_ENUM_CTOR_ARGS_CB(unused0, unused1, idx, elem) \
    BOOST_PP_COMMA_IF(idx) \
        const typename std::remove_const< \
            typename std::remove_reference< \
                BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(0, elem)) \
            >::type \
        >::type & arg##idx

#define TYPE_GEN_TYPE_ENUM_CTOR_ARGS(seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
         TYPE_GEN_TYPE_ENUM_CTOR_ARGS_CB \
        ,~ \
        ,seq \
    )

#define TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB_COLON(idx, elem) \
    :BOOST_PP_TUPLE_ELEM(1, elem){arg##idx}

#define TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB_COMMA(idx, elem) \
    ,BOOST_PP_TUPLE_ELEM(1, elem){arg##idx}

#define TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB(unused0, unused1, idx, elem) \
    BOOST_PP_IF( \
         idx \
        ,TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB_COMMA \
        ,TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB_COLON \
    )(idx, elem)

#define TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST(seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
         TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST_CB \
        ,~ \
        ,seq \
    )

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_FOR_TI(tname, elem) \
    decltype(tname::BOOST_PP_TUPLE_ELEM(0, elem))

#define TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_NOT_FOR_TI(tname, elem) \
    BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(0, elem))

#define TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_CB(unused0, data, idx, elem) \
    BOOST_PP_COMMA_IF(idx) \
        BOOST_PP_IF( \
             BOOST_PP_TUPLE_ELEM(1, data) \
            ,TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_FOR_TI \
            ,TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_NOT_FOR_TI \
        )(BOOST_PP_TUPLE_ELEM(0, data), elem)

#define TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_IMPL(tname, for_ti, seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
        TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_CB \
        ,(tname, for_ti) \
        ,seq \
    )

#define TYPE_GEN_TYPE_GEN_TUPLE_ELEMS(tname, for_ti, tuplename, seq) \
    using tuplename = std::tuple<TYPE_GEN_TYPE_GEN_TUPLE_ELEMS_IMPL(tname, for_ti, seq)>;

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS_CB(unused0, data, unused1, elem) \
    f( \
         BOOST_PP_STRINGIZE( /* member name */ \
            BOOST_PP_IF( \
                 BOOST_PP_TUPLE_ELEM(0, data) \
                ,BOOST_PP_TUPLE_ELEM(0, elem) \
                ,BOOST_PP_TUPLE_ELEM(1, elem) \
            ) \
        ) \
        ,BOOST_PP_IF( \
             BOOST_PP_TUPLE_ELEM(0, data) \
            ,m_o \
            ,BOOST_PP_TUPLE_ELEM(1, data) \
        ).BOOST_PP_IF( \
             BOOST_PP_TUPLE_ELEM(0, data) \
            ,BOOST_PP_TUPLE_ELEM(0, elem) \
            ,BOOST_PP_TUPLE_ELEM(1, elem) \
        ) /* member var */ \
    );

#define TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS(for_ti, obj, seq) \
    template<typename F> /* void(const char *memname, const T &v) */ \
    void apply(F &&f) const { \
        BOOST_PP_SEQ_FOR_EACH_I( \
             TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS_CB \
            ,(for_ti, obj) \
            ,seq \
        ) \
    } \
    template<typename F> /* void(const char *memname, T &v) */ \
    void apply(F &&f) { \
        BOOST_PP_SEQ_FOR_EACH_I( \
             TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS_CB \
            ,(for_ti, obj) \
            ,seq \
        ) \
    }

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL_CB(unused, data, idx, elem) \
    BOOST_PP_IF(idx, &&, ) \
        BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), m_o, BOOST_PP_TUPLE_ELEM(1, data)) \
            .BOOST_PP_IF( \
                 BOOST_PP_TUPLE_ELEM(0, data) \
                ,BOOST_PP_TUPLE_ELEM(0, elem) \
                ,BOOST_PP_TUPLE_ELEM(1, elem) \
            ) \
            == \
                BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), r.m_o, r) \
                    .BOOST_PP_IF( \
                         BOOST_PP_TUPLE_ELEM(0, data) \
                        ,BOOST_PP_TUPLE_ELEM(0, elem) \
                        ,BOOST_PP_TUPLE_ELEM(1, elem) \
                    )

#define TYPE_GEN_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, for_ti, obj, seq) \
    bool operator== (const BOOST_PP_IF(for_ti, ::typegen::typeinfo<tname>, tname) &r) const { \
        return \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 TYPE_GEN_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL_CB \
                ,(for_ti, obj) \
                ,seq \
            ) \
        ; \
    } \
    bool operator!= (const BOOST_PP_IF(for_ti, ::typegen::typeinfo<tname>, tname) &r) const { \
        return !this->operator==(r); \
    }

/***************************************************************************/

#define TYPE_GEN_TYPE_ENUMERATE_MEMBERS_CB(unused, data, idx, elem) \
    BOOST_PP_COMMA_IF(idx) \
        std::ref( \
            BOOST_PP_TUPLE_ELEM(1, data) \
            . \
            BOOST_PP_IF( \
                 BOOST_PP_TUPLE_ELEM(0, data) \
                ,BOOST_PP_TUPLE_ELEM(0, elem) \
                ,BOOST_PP_TUPLE_ELEM(1, elem) \
            ) \
        )

#define TYPE_GEN_TYPE_ENUMERATE_MEMBERS(for_ti, obj, seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
         TYPE_GEN_TYPE_ENUMERATE_MEMBERS_CB \
        ,(for_ti, obj) \
        ,seq \
    )

#define TYPE_GEN_TYPE_GEN_GET_MEMBERS(for_ti, obj, seq) \
    template< \
         std::size_t I \
        ,typename T = typename std::tuple_element<I, types_as_tuple>::type> \
    T& get_member() { \
        auto t = std::make_tuple(TYPE_GEN_TYPE_ENUMERATE_MEMBERS(for_ti, obj, seq)); \
        return std::get<I>(t); \
    } \
    template< \
         std::size_t I \
        ,typename T = typename std::tuple_element<I, types_as_tuple>::type> \
    const T& get_member() const { \
        const auto t = std::make_tuple(TYPE_GEN_TYPE_ENUMERATE_MEMBERS(for_ti, obj, seq)); \
        return std::get<I>(t); \
    }

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_COMPARE_CB(unused0, data, unused1, elem) \
    if ( BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(1, data)., ) \
            BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(1, elem)) \
        != \
        r. BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(1, data)., ) \
            BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(1, elem)) \
    ) { \
        res = false; \
        f( \
            BOOST_PP_STRINGIZE( \
                BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(1, elem)) \
            ) \
            ,BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(1, data)., ) \
                BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(1, elem)) \
            \
            ,r. BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(1, data)., ) \
                BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(1, elem)) \
        ); \
    }

#define TYPE_GEN_TYPE_GEN_COMPARE(tname, for_ti, obj, seq) \
    template<typename F> /* void(const char *memname, const T &l, const T &r) */ \
    bool compare(const BOOST_PP_IF(for_ti, ::typegen::typeinfo<tname>, tname) &r, F &&f) const { \
        bool res = true; \
        BOOST_PP_SEQ_FOR_EACH_I( \
             TYPE_GEN_TYPE_GEN_COMPARE_CB \
            ,(for_ti, obj) \
            ,seq \
        ) \
        return res; \
    }

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS_CB(unused, data, idx, elem) \
    & BOOST_PP_TUPLE_ELEM(1, data) \
        .BOOST_PP_IF( \
             BOOST_PP_TUPLE_ELEM(0, data) \
            ,BOOST_PP_TUPLE_ELEM(0, elem) \
            ,BOOST_PP_TUPLE_ELEM(1, elem) \
        )

#define TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS(for_ti, obj, seq) \
    template<typename Archive> \
    void serialize(Archive &ar) const { \
        ar \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS_CB \
                ,(for_ti, obj)\
                ,seq \
            ) \
        ; \
    } \
    template<typename Archive> \
    void serialize(Archive &ar) { \
        ar \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS_CB \
                ,(for_ti, obj) \
                ,seq \
            ) \
        ; \
    } \

/***************************************************************************/

#define TYPE_GEN_TYPE_GEN_METAINFO_ENUM_MEMBERS_CB(unused, for_ti, idx, elem) \
    BOOST_PP_COMMA_IF(idx) \
        BOOST_PP_STRINGIZE( \
            BOOST_PP_IF( \
                 for_ti \
                ,BOOST_PP_TUPLE_ELEM(0, elem) \
                ,BOOST_PP_TUPLE_ELEM(1, elem) \
            ) \
        )

#define TYPE_GEN_TYPE_GEN_METAINFO(tname, for_ti, seq) \
    static constexpr const char* _typename() { return BOOST_PP_STRINGIZE(tname); } \
    static constexpr std::size_t _members_count() { return BOOST_PP_SEQ_SIZE(seq); } \
    \
    static constexpr const char *_meta_members_names[] = { \
        BOOST_PP_SEQ_FOR_EACH_I( \
             TYPE_GEN_TYPE_GEN_METAINFO_ENUM_MEMBERS_CB \
            ,for_ti \
            ,seq \
        ) \
        , nullptr \
    }; \
    static constexpr const char* const* _members_names() { \
        return _meta_members_names; \
    } \
    \
    static constexpr const char* _member_name(std::size_t n) { return _members_names()[n]; }


/***************************************************************************/

#define TYPE_GEN_TYPE_IMPL(tname, seq, ...) \
    struct tname { \
        /* user code expanded here */ \
        __VA_ARGS__ \
        \
        tname() = default; \
        tname(const tname &r) = default; \
        tname(tname &&r) = default; \
        \
        tname& operator= (const tname &r) = default; \
        tname& operator= (tname &&r) = default; \
        \
        tname(TYPE_GEN_TYPE_ENUM_CTOR_ARGS(seq)) \
            TYPE_GEN_TYPE_ENUM_CTOR_INIT_LIST(seq) \
        {} \
        \
        /* members */ \
        BOOST_PP_SEQ_FOR_EACH_I( \
             TYPE_GEN_TYPE_DECLARE_MEMBER \
            ,~ \
            ,seq \
        ) \
        \
        TYPE_GEN_TYPE_GEN_TUPLE_ELEMS(tname, 0, types_as_tuple, seq) \
        \
        TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS(0, (*this), seq) \
        \
        TYPE_GEN_TYPE_GEN_GET_MEMBERS(0, (*this), seq) \
        \
        TYPE_GEN_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, 0, (*this), seq) \
        \
        TYPE_GEN_TYPE_GEN_COMPARE(tname, 0, (*this), seq) \
        \
        TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS(0, (*this), seq) \
        \
        TYPE_GEN_TYPE_GEN_METAINFO(tname, 0, seq) \
    };

#define TYPE_GEN_TYPE( \
     tname /* type name */ \
    ,seq /* members sequence */ \
    ,... /* user code */ \
) \
    TYPE_GEN_TYPE_IMPL( \
         tname \
        ,BOOST_PP_CAT(TYPE_GEN_TYPE_WRAP_X seq, 0) \
        ,__VA_ARGS__ \
    )

/***************************************************************************/

namespace typegen {
template<typename>
struct typeinfo;
} // ns typegen

#define TYPE_GEN_ADAPT_IMPL(tname, seq) \
namespace typegen { \
    \
    template<> \
    struct typeinfo<tname> { \
        typeinfo(tname &o) \
            :m_o{o} \
        {} \
        \
        TYPE_GEN_TYPE_GEN_TUPLE_ELEMS(tname, 1, types_as_tuple, seq) \
        \
        TYPE_GEN_TYPE_GEN_APPLY_FUNCTIONS(1, m_o, seq) \
        \
        TYPE_GEN_TYPE_GEN_GET_MEMBERS(1, m_o, seq) \
        \
        TYPE_GEN_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, 1, m_o, seq) \
        \
        TYPE_GEN_TYPE_GEN_COMPARE(tname, 1, m_o, seq) \
        \
        TYPE_GEN_TYPE_GEN_SERIALIZE_FUNCTIONS(1, m_o, seq) \
        \
        TYPE_GEN_TYPE_GEN_METAINFO(tname, 1, seq) \
        \
        tname &m_o; \
    }; \
    \
} // ns typegen

#define TYPE_GEN_ADAPT( \
     tname /* type name */ \
    ,seq /* members sequence */ \
) \
    TYPE_GEN_ADAPT_IMPL( \
         tname \
        ,BOOST_PP_CAT(TYPE_GEN_TYPE_WRAP_X seq, 0) \
    )

/***************************************************************************/

#endif // __type_gen__type_gen_hpp
