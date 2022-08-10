/* Test for Unicode Algorithms Implementation.
 * License: Public Domain or MIT - choose whatever you want.
 * See LICENSE.md */

void test_prop()
{
    std::size_t count_surrogate      = 0;
    std::size_t count_control        = 0;
    std::size_t count_noncharacter   = 0;
    std::size_t count_private_use    = 0;
    std::size_t count_invalid        = 0;
    std::size_t count_invalid_scalar = 0;

    for (char32_t c = 0; c <= 0x110002; ++c) // 0x110002 - 0x10FFFF == 3 means test 3 invalid code points
    {
        if (c == 0x110002)
            c = 0xFFFFFFFF;

        // Cross test functions that use Unicode data table and don't use it
        TESTX(uni::codepoint::is_surrogate(c)    == uni::codepoint::prop(c).Surrogate());
        TESTX(uni::codepoint::is_private_use(c)  == uni::codepoint::prop(c).Private_Use());
        TESTX(uni::codepoint::is_control(c)      == uni::codepoint::prop(c).Control());
        TESTX(uni::codepoint::is_noncharacter(c) == uni::codepoint::prop(c).Noncharacter_Code_Point());
        TESTX(uni::codepoint::is_reserved(c)     == uni::codepoint::prop(c).Reserved());

        // Count stable code points
        // https://www.unicode.org/policies/stability_policy.html#Property_Value
        // X property is an immutable code point property,
        // which means that its property values for all Unicode code points will never change.
        if (uni::codepoint::is_surrogate(c))     count_surrogate++;
        if (uni::codepoint::is_private_use(c))   count_private_use++;
        if (uni::codepoint::is_control(c))       count_control++;
        if (uni::codepoint::is_noncharacter(c))  count_noncharacter++;
        if (!uni::codepoint::is_valid(c))        count_invalid++;
        if (!uni::codepoint::is_valid_scalar(c)) count_invalid_scalar++;

        // Other functions
        TESTX(uni::codepoint::is_alphabetic(c) == uni::codepoint::prop(c).Alphabetic());
        TESTX(uni::codepoint::is_numeric(c)    == uni::codepoint::prop(c).Numeric());
        TESTX(uni::codepoint::is_whitespace(c) == uni::codepoint::prop(c).White_Space());

        TESTX(uni::codepoint::prop(c).Surrogate()   == uni::codepoint::prop(c).General_Category_Cs());
        TESTX(uni::codepoint::prop(c).Private_Use() == uni::codepoint::prop(c).General_Category_Co());
        TESTX(uni::codepoint::prop(c).Control()     == uni::codepoint::prop(c).General_Category_Cc());

        if (uni::codepoint::prop(c).Alphabetic() ||
            uni::codepoint::prop(c).Numeric())
            TESTX(uni::codepoint::is_alphanumeric(c));
        else
            TESTX(!uni::codepoint::is_alphanumeric(c));

        if (uni::codepoint::prop(c).General_Category_Zl() ||
            uni::codepoint::prop(c).General_Category_Zp() ||
            uni::codepoint::prop(c).General_Category_Cf())
            TESTX(uni::codepoint::prop(c).Format());
        else
            TESTX(!uni::codepoint::prop(c).Format());

        if (uni::codepoint::prop(c).General_Category_Cn() &&
            !uni::codepoint::prop(c).Noncharacter_Code_Point())
            TESTX(uni::codepoint::prop(c).Reserved());
        else
            TESTX(!uni::codepoint::prop(c).Reserved());

        if (uni::codepoint::prop(c).General_Category_Zl() ||
            uni::codepoint::prop(c).General_Category_Zp() ||
            uni::codepoint::prop(c).General_Category_Cc() ||
            uni::codepoint::prop(c).General_Category_Cf() ||
            uni::codepoint::prop(c).General_Category_Cs() ||
            uni::codepoint::prop(c).General_Category_Co() ||
            uni::codepoint::prop(c).General_Category_Cn())
            TESTX(!uni::codepoint::prop(c).Graphic());
        else
            TESTX(uni::codepoint::prop(c).Graphic());

        if (uni::codepoint::is_valid(c) && !uni::codepoint::is_surrogate(c))
            TESTX(uni::codepoint::is_valid_scalar(c));
        else
            TESTX(!uni::codepoint::is_valid_scalar(c));

        if (c > 0xFFFF && c <= 0x10FFFF)
            TESTX(uni::codepoint::is_supplementary(c));
        else
            TESTX(!uni::codepoint::is_supplementary(c));

        if (c >= 0xD800 && c <= 0xDFFF)
            TESTX(uni::codepoint::is_surrogate(c));
        else
            TESTX(!uni::codepoint::is_surrogate(c));

        if (c == 0xFFFFFFFF)
            break;
    }

    // Must be the same in all Unicode versions
    // https://en.wikipedia.org/wiki/Unicode_character_property#General_Category
    // https://en.wikipedia.org/w/index.php?title=Unicode_character_property&oldid=1066524639#General_Category
    TESTX(count_surrogate      == 2048);
    TESTX(count_private_use    == 137468);
    TESTX(count_control        == 65);
    TESTX(count_noncharacter   == 66);

    TESTX(count_invalid        == 3);
    TESTX(count_invalid_scalar == count_surrogate + count_invalid);

    TESTX(uni::codepoint::is_valid(0));
    TESTX(uni::codepoint::is_valid(0xD800));
    TESTX(uni::codepoint::is_valid(0xDBFF));
    TESTX(uni::codepoint::is_valid(0xDC00));
    TESTX(uni::codepoint::is_valid(0xDFFF));
    TESTX(uni::codepoint::is_valid(0xFFFD));
    TESTX(uni::codepoint::is_valid(0xFFFE));
    TESTX(uni::codepoint::is_valid(0xFFFF));
    TESTX(uni::codepoint::is_valid(0x10FFFF));
    TESTX(!uni::codepoint::is_valid(0x110000));
    TESTX(!uni::codepoint::is_valid(0xFFFFFFFF));

    TESTX(uni::codepoint::is_valid_scalar(0));
    TESTX(!uni::codepoint::is_valid_scalar(0xD800));
    TESTX(!uni::codepoint::is_valid_scalar(0xDBFF));
    TESTX(!uni::codepoint::is_valid_scalar(0xDC00));
    TESTX(!uni::codepoint::is_valid_scalar(0xDFFF));
    TESTX(uni::codepoint::is_valid_scalar(0xFFFD));
    TESTX(uni::codepoint::is_valid_scalar(0xFFFE));
    TESTX(uni::codepoint::is_valid_scalar(0xFFFF));
    TESTX(uni::codepoint::is_valid_scalar(0x10FFFF));
    TESTX(!uni::codepoint::is_valid_scalar(0x110000));
    TESTX(!uni::codepoint::is_valid_scalar(0xFFFFFFFF));

    TESTX(uni::codepoint::is_alphabetic(U'Q'));
    TESTX(!uni::codepoint::is_alphabetic(U'1'));
    TESTX(!uni::codepoint::is_alphabetic(0));
    TESTX(!uni::codepoint::is_alphabetic(0x110000));
    TESTX(!uni::codepoint::is_alphabetic(0xFFFFFFFF));

    TESTX(!uni::codepoint::is_numeric(U'R'));
    TESTX(uni::codepoint::is_numeric(U'2'));
    TESTX(!uni::codepoint::is_numeric(0));
    TESTX(!uni::codepoint::is_numeric(0x110000));
    TESTX(!uni::codepoint::is_numeric(0xFFFFFFFF));

    TESTX(uni::codepoint::is_alphanumeric(U'S'));
    TESTX(uni::codepoint::is_alphanumeric(U'3'));
    TESTX(!uni::codepoint::is_alphanumeric(0));
    TESTX(!uni::codepoint::is_alphanumeric(0x110000));
    TESTX(!uni::codepoint::is_alphanumeric(0xFFFFFFFF));

    TESTX(uni::codepoint::is_whitespace(U' '));
    TESTX(uni::codepoint::is_whitespace(0x3000));
    TESTX(!uni::codepoint::is_whitespace(0));
    TESTX(!uni::codepoint::is_whitespace(0x110000));
    TESTX(!uni::codepoint::is_whitespace(0xFFFFFFFF));

    TESTX(uni::codepoint::is_control(0));
    TESTX(uni::codepoint::is_control(0x1F));
    TESTX(!uni::codepoint::is_control(0x20));
    TESTX(uni::codepoint::is_control(0x7F));
    TESTX(uni::codepoint::is_control(0x9F));
    TESTX(!uni::codepoint::is_control(0xFFFF));
    TESTX(!uni::codepoint::is_control(0x10FFFF));
    TESTX(!uni::codepoint::is_control(0x110000));
    TESTX(!uni::codepoint::is_control(0xFFFFFFFF));

    TESTX(!uni::codepoint::is_noncharacter(0));
    TESTX(!uni::codepoint::is_noncharacter(0xFFFD));
    TESTX(uni::codepoint::is_noncharacter(0xFFFE));
    TESTX(uni::codepoint::is_noncharacter(0xFFFF));
    TESTX(uni::codepoint::is_noncharacter(0x10FFFE));
    TESTX(uni::codepoint::is_noncharacter(0x10FFFF));
    TESTX(!uni::codepoint::is_noncharacter(0x110000));
    TESTX(!uni::codepoint::is_noncharacter(0xFFFFFFFF));

    TESTX(!uni::codepoint::is_private_use(0));
    TESTX(uni::codepoint::is_private_use(0xE000));
    TESTX(uni::codepoint::is_private_use(0xF8FF));
    TESTX(!uni::codepoint::is_private_use(0xFFFF));
    TESTX(uni::codepoint::is_private_use(0xF0000));
    TESTX(uni::codepoint::is_private_use(0xFFFFD));
    TESTX(uni::codepoint::is_private_use(0x100000));
    TESTX(uni::codepoint::is_private_use(0x10FFFD));
    TESTX(!uni::codepoint::is_private_use(0x10FFFF));
    TESTX(!uni::codepoint::is_private_use(0x110000));
    TESTX(!uni::codepoint::is_private_use(0xFFFFFFFF));

    TESTX(!uni::codepoint::is_surrogate(0));
    TESTX(!uni::codepoint::is_surrogate(0xFFFF));
    TESTX(uni::codepoint::is_surrogate(0xD800));
    TESTX(uni::codepoint::is_surrogate(0xDBFF));
    TESTX(uni::codepoint::is_surrogate(0xDC00));
    TESTX(uni::codepoint::is_surrogate(0xDFFF));
    TESTX(!uni::codepoint::is_surrogate(0x10FFFF));
    TESTX(!uni::codepoint::is_surrogate(0x110000));
    TESTX(!uni::codepoint::is_surrogate(0xFFFFFFFF));

    TESTX(!uni::codepoint::is_supplementary(0));
    TESTX(!uni::codepoint::is_supplementary(0xFFFF));
    TESTX(uni::codepoint::is_supplementary(0x10000));
    TESTX(uni::codepoint::is_supplementary(0x10FFFF));
    TESTX(!uni::codepoint::is_supplementary(0x110000));
    TESTX(!uni::codepoint::is_supplementary(0xFFFFFFFF));

    // First code point from UnicodeData.txt with such General_Category

    TESTX(uni::codepoint::prop(0x0041).General_Category_Lu());
    TESTX(uni::codepoint::prop(0x0061).General_Category_Ll());
    TESTX(uni::codepoint::prop(0x01C5).General_Category_Lt());
    TESTX(uni::codepoint::prop(0x02B0).General_Category_Lm());
    TESTX(uni::codepoint::prop(0x00AA).General_Category_Lo());
    TESTX(uni::codepoint::prop(0x0300).General_Category_Mn());
    TESTX(uni::codepoint::prop(0x0903).General_Category_Mc());
    TESTX(uni::codepoint::prop(0x0488).General_Category_Me());
    TESTX(uni::codepoint::prop(0x0030).General_Category_Nd());
    TESTX(uni::codepoint::prop(0x16EE).General_Category_Nl());
    TESTX(uni::codepoint::prop(0x00B2).General_Category_No());
    TESTX(uni::codepoint::prop(0x005F).General_Category_Pc());
    TESTX(uni::codepoint::prop(0x002D).General_Category_Pd());
    TESTX(uni::codepoint::prop(0x0028).General_Category_Ps());
    TESTX(uni::codepoint::prop(0x0029).General_Category_Pe());
    TESTX(uni::codepoint::prop(0x00AB).General_Category_Pi());
    TESTX(uni::codepoint::prop(0x00BB).General_Category_Pf());
    TESTX(uni::codepoint::prop(0x0021).General_Category_Po());
    TESTX(uni::codepoint::prop(0x002B).General_Category_Sm());
    TESTX(uni::codepoint::prop(0x0024).General_Category_Sc());
    TESTX(uni::codepoint::prop(0x005E).General_Category_Sk());
    TESTX(uni::codepoint::prop(0x00A6).General_Category_So());
    TESTX(uni::codepoint::prop(0x0020).General_Category_Zs());
    TESTX(uni::codepoint::prop(0x2028).General_Category_Zl());
    TESTX(uni::codepoint::prop(0x2029).General_Category_Zp());
    TESTX(uni::codepoint::prop(0x0000).General_Category_Cc());
    TESTX(uni::codepoint::prop(0x00AD).General_Category_Cf());
    TESTX(uni::codepoint::prop(0xD800).General_Category_Cs());
    TESTX(uni::codepoint::prop(0xE000).General_Category_Co());
    // Random unassigned code point (change to something else if it will be assigned)
    TESTX(uni::codepoint::prop(0x0A7777).General_Category_Cn());

    // Why did you do that? Change them back. They must be stable.
    static_assert(uni::detail::impl_General_Category_Lu == 1);
    static_assert(uni::detail::impl_General_Category_Ll == 2);
    static_assert(uni::detail::impl_General_Category_Lt == 3);
    static_assert(uni::detail::impl_General_Category_Lm == 4);
    static_assert(uni::detail::impl_General_Category_Lo == 5);
    static_assert(uni::detail::impl_General_Category_Mn == 6);
    static_assert(uni::detail::impl_General_Category_Mc == 7);
    static_assert(uni::detail::impl_General_Category_Me == 8);
    static_assert(uni::detail::impl_General_Category_Nd == 9);
    static_assert(uni::detail::impl_General_Category_Nl == 10);
    static_assert(uni::detail::impl_General_Category_No == 11);
    static_assert(uni::detail::impl_General_Category_Pc == 12);
    static_assert(uni::detail::impl_General_Category_Pd == 13);
    static_assert(uni::detail::impl_General_Category_Ps == 14);
    static_assert(uni::detail::impl_General_Category_Pe == 15);
    static_assert(uni::detail::impl_General_Category_Pi == 16);
    static_assert(uni::detail::impl_General_Category_Pf == 17);
    static_assert(uni::detail::impl_General_Category_Po == 18);
    static_assert(uni::detail::impl_General_Category_Sm == 19);
    static_assert(uni::detail::impl_General_Category_Sc == 20);
    static_assert(uni::detail::impl_General_Category_Sk == 21);
    static_assert(uni::detail::impl_General_Category_So == 22);
    static_assert(uni::detail::impl_General_Category_Zs == 23);
    static_assert(uni::detail::impl_General_Category_Zl == 24);
    static_assert(uni::detail::impl_General_Category_Zp == 25);
    static_assert(uni::detail::impl_General_Category_Cc == 26);
    static_assert(uni::detail::impl_General_Category_Cf == 27);
    static_assert(uni::detail::impl_General_Category_Cs == 28);
    static_assert(uni::detail::impl_General_Category_Co == 29);
    static_assert(uni::detail::impl_General_Category_Cn == 0);
}