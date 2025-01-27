/* Test for Unicode Algorithms Implementation.
 * License: Public Domain or MIT - choose whatever you want.
 * See LICENSE.md */

test_constexpr std::u16string test_iter_rev_utf8to16(std::string_view str)
{
    auto view = una::ranges::utf8_view<std::string_view, una::detail::impl_iter_error>{str};

    std::u32string codepoints;
    for (auto it = view.end(); it != view.begin();)
    {
        --it;
        codepoints.push_back(*it);
    }

    // NOTE: Use manual reverse here when constexpr test because constexpr std::reverse
    // is kinda annoying (produces many false positives) in GCC with -Wstrict-overflow=2
#if !defined(TEST_MODE_CONSTEXPR)
    std::reverse(codepoints.begin(), codepoints.end());
#else
    for(std::size_t i = 0, j = codepoints.size() - 1; i < (codepoints.size() / 2); ++i, --j)
    {
            char32_t temp = codepoints[j];
            codepoints[j] = codepoints[i];
            codepoints[i] = temp;
    }
#endif

    return codepoints | una::ranges::to_utf16<std::u16string>();
}

test_constexpr std::string test_iter_rev_utf16to8(std::u16string_view str)
{
    auto view = una::ranges::utf16_view<std::u16string_view, una::detail::impl_iter_error>{str};

    std::u32string codepoints;
    for (auto it = view.end(); it != view.begin();)
    {
        --it;
        codepoints.push_back(*it);
    }

    // NOTE: See comment in the function above for details
#if !defined(TEST_MODE_CONSTEXPR)
    std::reverse(codepoints.begin(), codepoints.end());
#else
    for(std::size_t i = 0, j = codepoints.size() - 1; i < (codepoints.size() / 2); ++i, --j)
    {
            char32_t temp = codepoints[j];
            codepoints[j] = codepoints[i];
            codepoints[i] = temp;
    }
#endif

    return codepoints | una::ranges::to_utf8<std::string>();
}

test_constexpr bool test_lenient_iter_rev_utf8to16()
{
    TESTX(test_iter_rev_utf8to16("ABC") == u"ABC");
    TESTX(test_iter_rev_utf8to16("\xD0\x90\xD0\x91\xD0\x92") == u"\x0410\x0411\x0412");
    TESTX(test_iter_rev_utf8to16("\xEF\xBC\xA1\xEF\xBC\xA2\xEF\xBC\xA3") == u"\xFF21\xFF22\xFF23");
    TESTX(test_iter_rev_utf8to16("\xF0\x9D\x90\x80\xF0\x9D\x90\x81\xF0\x9D\x90\x82") == u"\xD835\xDC00\xD835\xDC01\xD835\xDC02");

    TESTX(test_iter_rev_utf8to16("\xC0\x80") == u"\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xE0\x80\x80") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xC1\xBF") == u"\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xE0\x81\xBF") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x80\x81\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\x80\x81\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x80\x81\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xE0\x82\x80") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x80\x82\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\x80\x82\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x80\x82\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xE0\x9F\xBF") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x80\x9F\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\x80\x9F\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x80\x9F\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF0\x80\xA0\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\x80\xA0\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x80\xA0\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF0\x8F\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF8\x80\xBF\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x8F\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF8\x80\x90\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x80\x90\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF8\x84\xBF\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x84\x8F\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF4\x90\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFB\xBF\xBF\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFD\xBF\xBF\xBF\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xED\xA0\x80") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xED\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xED\xA0\xBD\xED\xB2\xA9") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF8\x84\x90\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFC\x80\x84\x90\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x8D\xA0\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x8D\xBF\xBF") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x8D\xA0\xBD\xF0\x8D\xB2\xA9") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\x80") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80") == u"\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\x80\x80\x80\x80\x80\x80\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xC2\xB6\x80") == u"\x00B6\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xE2\x98\x83\x80") == u"\x2603\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x9F\x92\xA9\x80") == u"\xD83D\xDCA9\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFB\xBF\xBF\xBF\xBF\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFD\xBF\xBF\xBF\xBF\xBF\x80") == u"\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xC2") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xE2") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xE2\x98") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x9F") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x9F\x92") == u"\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xF4\x90\x80") == u"\xFFFD\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xF0\x80\xA0") == u"\xFFFD\xFFFD\xFFFD");

    TESTX(test_iter_rev_utf8to16("\xFE") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFE\x80") == u"\xFFFD\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFF") == u"\xFFFD");
    TESTX(test_iter_rev_utf8to16("\xFF\x80") == u"\xFFFD\xFFFD");

    return true;
}

test_constexpr bool test_lenient_iter_rev_utf16to8()
{
    TESTX(test_iter_rev_utf16to8(u"ABC") == "ABC");
    TESTX(test_iter_rev_utf16to8(u"\xFF21\xFF22\xFF23") == "\xEF\xBC\xA1\xEF\xBC\xA2\xEF\xBC\xA3");
    TESTX(test_iter_rev_utf16to8(u"\xD835\xDC00\xD835\xDC01\xD835\xDC02") == "\xF0\x9D\x90\x80\xF0\x9D\x90\x81\xF0\x9D\x90\x82");

    TESTX(test_iter_rev_utf16to8(u"\xD800") == "\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xD800\xDBFF") == "\xEF\xBF\xBD\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xD800\xD800\xDBFF") == "\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD");

    TESTX(test_iter_rev_utf16to8(u"\xD800\x0057") == "\xEF\xBF\xBD\x57");
    TESTX(test_iter_rev_utf16to8(u"\xD800\x0057\xDBFF\xFF37") == "\xEF\xBF\xBD\x57\xEF\xBF\xBD\xEF\xBC\xB7");
    TESTX(test_iter_rev_utf16to8(u"\xD800\x0057\xD800\xFF37\xDBFF\x0057") == "\xEF\xBF\xBD\x57\xEF\xBF\xBD\xEF\xBC\xB7\xEF\xBF\xBD\x57");

    TESTX(test_iter_rev_utf16to8(u"\xDC00") == "\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xDC00\xDFFF") == "\xEF\xBF\xBD\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xDC00\xDC00\xDFFF") == "\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD");

    TESTX(test_iter_rev_utf16to8(u"\xDC00\xD835") == "\xEF\xBF\xBD\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xDC00\xD835\xDC01\xD835") == "\xEF\xBF\xBD\xF0\x9D\x90\x81\xEF\xBF\xBD");
    TESTX(test_iter_rev_utf16to8(u"\xDC00\xD835\xDC01\xD835\xDC02\xD835") == "\xEF\xBF\xBD\xF0\x9D\x90\x81\xF0\x9D\x90\x82\xEF\xBF\xBD");

    TESTX(test_iter_rev_utf16to8(u"\x0041\x0042\xD800\x0043") == "\x41\x42\xEF\xBF\xBD\x43");
    TESTX(test_iter_rev_utf16to8(u"\x0041\x0042\xDC00\x0043") == "\x41\x42\xEF\xBF\xBD\x43");
    TESTX(test_iter_rev_utf16to8(u"\x0041\x0042\xD800\xFF37\x0043") == "\x41\x42\xEF\xBF\xBD\xEF\xBC\xB7\x43");

    return true;
}

