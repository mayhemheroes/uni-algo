/* Test for Unicode Algorithms Implementation.
 * License: Public Domain or MIT - choose whatever you want.
 * See LICENSE.md */

void test_ascii_collate()
{
    std::string str_ascii;
    for(char32_t c = 0; c <= 0x7F; c++)
        str_ascii.push_back(static_cast<char>(c));

    // Shuffle
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(str_ascii.begin(), str_ascii.end(), std::mt19937{static_cast<std::mt19937::result_type>(seed)});

    std::string test_ascii = str_ascii;
    std::string test_utf8 = str_ascii;
    std::string test_utf16 = str_ascii;

    // Binary sort for reference (for debugging)
    //std::sort(test_ascii.begin(), test_ascii.end());
    //std::sort(test_utf8.begin(), test_utf8.end());
    //std::sort(test_utf16.begin(), test_utf16.end());

    // Test all collate functions to make sure the result is consistent

    std::sort(test_ascii.begin(), test_ascii.end(), [](char a, char b) {
        return unx::casesens::collate_ascii<char>(std::string{a}, std::string{b}) < 0;
    });

    std::sort(test_utf8.begin(), test_utf8.end(), [](char a, char b) {
        return uni::casesens::collate_utf8<char>(std::string{a}, std::string{b}) < 0;
    });

    std::sort(test_utf16.begin(), test_utf16.end(), [](char a, char b) {
        return uni::casesens::collate_utf16<char16_t>(std::u16string{static_cast<char16_t>(a)}, std::u16string{static_cast<char16_t>(b)}) < 0;
    });

    std::string test_group_ascii = test_ascii;
    std::string test_group_utf8 = test_utf8;
    std::string test_group_utf16 = test_utf16;

    auto it_ascii = std::unique(test_group_ascii.begin(), test_group_ascii.end(), [](char a, char b) {
        return unx::caseless::collate_ascii<char>(std::string{a}, std::string{b}) == 0;
    });
    test_group_ascii.erase(it_ascii, test_group_ascii.end());

    auto it_utf8 = std::unique(test_group_utf8.begin(), test_group_utf8.end(), [](char a, char b) {
        return uni::caseless::collate_utf8<char>(std::string{a}, std::string{b}) == 0;
    });
    test_group_utf8.erase(it_utf8, test_group_utf8.end());

    auto it_utf16 = std::unique(test_group_utf16.begin(), test_group_utf16.end(), [](char a, char b) {
        return uni::caseless::collate_utf16<char16_t>(std::u16string{static_cast<char16_t>(a)}, std::u16string{static_cast<char16_t>(b)}) == 0;
    });
    test_group_utf16.erase(it_utf16, test_group_utf16.end());

    auto func_make_readable = [](const std::string& str) -> std::string
    {
        std::string result;

        std::for_each(str.cbegin(), str.cend(), [&result](char c) {
            if (c == 0x00)
                result += ' ';
            else if (c <= 0x1F || c == 0x7F || c == '"' || c == '\\')
            {
                char hex[16] = {};
                std::snprintf(hex, 16, "\\x%02X", c);
                result += hex;
            }
            else
                result += std::string{c};
        });

        return result;
    };

    std::string result_ascii = func_make_readable(test_ascii);
    std::string result_utf8 = func_make_readable(test_utf8);
    std::string result_utf16 = func_make_readable(test_utf16);

    std::string result_group_ascii = func_make_readable(test_group_ascii);
    std::string result_group_utf8 = func_make_readable(test_group_utf8);
    std::string result_group_utf16 = func_make_readable(test_group_utf16);

    std::string valid_result =
    " \x01\x02\x03\x04\x05\x06\x07\x08\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F\x09\x0A\x0B\x0C\x0D"
    " !\x22#%&'()*+,-./:;<=>?@[\x5C]^_`{|}~$0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";

    valid_result = func_make_readable(valid_result);

    // For reference valid result with binary sort
    // " \x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
    // " !\x22#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\x5C]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F";

    std::string valid_result_group =
    " \x01\x02\x03\x04\x05\x06\x07\x08\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F\x09\x0A\x0B\x0C\x0D"
    " !\x22#%&'()*+,-./:;<=>?@[\x5C]^_`{|}~$0123456789abcdefghijklmnopqrstuvwxyz";

    valid_result_group = func_make_readable(valid_result_group);

    TESTX(valid_result == result_ascii);
    TESTX(valid_result == result_utf8);
    TESTX(valid_result == result_utf16);

    TESTX(valid_result_group == result_group_ascii);
    TESTX(valid_result_group == result_group_utf8);
    TESTX(valid_result_group == result_group_utf16);
}

#if 0
// This is the generator that what used to generate small ascii collate tables from DUCET data (stage1_order/stage2_order)
void generator_ascii_collate()
{
    std::u32string str;
    std::u32string str_result;

    for(char32_t c = 0; c <= 0x7F; c++)
        str.push_back(uni::detail::stages_order(c));

    str_result.resize(str.size());

    for(char32_t c = 0; c <= 0x7F; c++)
    {
        char32_t v = 0xFFFFFFFF;
        char32_t index = 0;
        for(char32_t i = 0; i <= 0x7F; i++)
        {
            if (str[i] < v)
            {
                v = str[i];
                index = i;
            }
        }

        str[index] = 0xFFFFFFFF;
        str_result[index] = c+1;
    }

    // For caseless table
    /*for (char32_t c = 0; c <= 0x7F; c++)
    {
        if (c >= 'A' && c <= 'Z')
            str2[c] = str2[c ^ 32];
    }*/

    for (char32_t c: str_result)
    {
        if (c < 10)
            std::cout << "  " << c << ", ";
        else if (c < 100)
            std::cout << " " << c << ", ";
        else
            std::cout << c << ", ";
    }
}
#endif