#include <gtest/gtest.h>

#include <unebi/chars.hpp>

#include "tools.hpp"

namespace unb = unebi;

template <class T>
struct SingleEOF : public ::testing::Test {
    template <class Tester>
    inline constexpr static bool charTesterConcept(Tester) {
        return unb::char_tester<Tester, T>;
    }
    template <class Tester>
    inline constexpr static bool test(Tester const& tester, T const val) {
        return tester(val);
    }
};

constexpr unb::chartest_disjunction eol_CrOrLf{unb::eol_cr, unb::eol_lf};
constexpr auto greaterEq_a = unb::make_chartest_single<std::greater_equal>('a');
constexpr auto lessEq_z = unb::make_chartest_single<std::less_equal>('z');
constexpr unb::chartest_conjunction lowerLetter{greaterEq_a, lessEq_z};

TYPED_TEST_SUITE(SingleEOF, testutil::TestCharTypes);

TYPED_TEST(SingleEOF, Concept) {
    static_assert(TestFixture::charTesterConcept(unb::eol_cr), "char_tester concept: eol_cf");
    static_assert(TestFixture::charTesterConcept(unb::eol_lf), "char_tester concept: eol_lf");
    static_assert(TestFixture::charTesterConcept(eol_CrOrLf), "char_tester concept: eol_CrOrLf");

    SUCCEED();
}

TYPED_TEST(SingleEOF, Equals) {
    static_assert(TestFixture::test(unb::eol_cr, (TypeParam)'\x0d'), "equal 0d");
    static_assert(!TestFixture::test(unb::eol_cr, (TypeParam)'\x0a'), "ne 0a");
    static_assert(TestFixture::test(unb::eol_lf, (TypeParam)'\x0a'), "equal 0d");
    static_assert(!TestFixture::test(unb::eol_lf, (TypeParam)'\x0d'), "ne 0d");
    static_assert(TestFixture::test(eol_CrOrLf, (TypeParam)'\x0a'), "disjunction 0a");
    static_assert(TestFixture::test(eol_CrOrLf, (TypeParam)'\x0d'), "disjunction 0d");
    static_assert(!TestFixture::test(eol_CrOrLf, (TypeParam)'\x10'), "disjunction false 10");
    static_assert(TestFixture::test(greaterEq_a, (TypeParam)'a'), "gte 'a'");
    static_assert(TestFixture::test(greaterEq_a, (TypeParam)'b'), "gte 'b'");
    static_assert(!TestFixture::test(greaterEq_a, (TypeParam)'`'), "gte '`'");
    static_assert(TestFixture::test(lessEq_z, (TypeParam)'z'), "lte 'z'");
    static_assert(TestFixture::test(lessEq_z, (TypeParam)'y'), "lte 'y'");
    static_assert(!TestFixture::test(lessEq_z, (TypeParam)'{'), "lte '{'");
    static_assert(TestFixture::test(lowerLetter, (TypeParam)'a'), "lowerLetter 'a'");
    static_assert(TestFixture::test(lowerLetter, (TypeParam)'h'), "lowerLetter 'h'");
    static_assert(TestFixture::test(lowerLetter, (TypeParam)'z'), "lowerLetter 'z'");
    static_assert(!TestFixture::test(lowerLetter, (TypeParam)'`'), "lowerLetter '`'");
    static_assert(!TestFixture::test(lowerLetter, (TypeParam)'{'), "lowerLetter '{'");

    SUCCEED();
}

TEST(SingleChar, VariousComparison) {
    auto eq_a0 = unb::chartest_single<char, std::equal_to>{'\xa0'};
    ASSERT_TRUE(eq_a0(u8'\xa0'));
    ASSERT_TRUE(eq_a0(u'\xa0'));
    ASSERT_TRUE(eq_a0(U'\xa0'));
    ASSERT_TRUE(eq_a0(L'\xa0'));
    ASSERT_TRUE(eq_a0(0xa0));
}

TEST(MultipleChar, Op0) {
    auto v = std::make_tuple('a', unb::eol_crlf.initialState());
    ASSERT_FALSE(unb::eol_crlf(v));
    get<0>(v) = '\x0d';
    ASSERT_FALSE(unb::eol_crlf(v));
    get<0>(v) = '\x0a';
    ASSERT_TRUE(unb::eol_crlf(v));
}

TEST(MultipleChar, Op1) {
    constexpr auto st = unb::eol_crlf.initialState();
    constexpr auto r0 = unb::eol_crlf('a', st);
    static_assert(!get<0>(r0), "eol_crlf: falase");
    auto [r1, st0] = unb::eol_crlf('\x0d', get<1>(r0));
    ASSERT_FALSE(r1);
    std::tie(r1, st0) = unb::eol_crlf('\x0a', st0);
    ASSERT_TRUE(r1);
    std::tie(r1, st0) = unb::eol_crlf('\x0a', st0);
    ASSERT_FALSE(r1);

}