#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/uni_algo/conv.h"
#include "../include/uni_algo/case.h"
#include "../include/uni_algo/norm.h"
#include "../include/uni_algo/ranges_grapheme.h"
#include "../include/uni_algo/ranges_word.h"

uint8_t buffer[8196];

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size > 0) {
        uint8_t opt = Data[0];
        memset(buffer, 0, 8196);
        memcpy(buffer, Data + 1, Size - 1);
        switch (opt) {
            case 0:
                una::utf8to16<uint8_t, char16_t>(buffer);
                break;
            case 1:
                una::cases::to_uppercase_utf8<uint8_t>(buffer);
                break;
            case 2:
                una::cases::to_lowercase_utf8<uint8_t>(buffer);
                break;
            case 3:
                una::cases::to_casefold_utf8<uint8_t>(buffer);
                break;
            case 4:
                una::cases::to_titlecase_utf8<uint8_t>(buffer);
                break;
            case 5:
                una::norm::to_nfc_utf8<uint8_t>(buffer);
                break;
            case 6:
                una::norm::is_nfc_utf8<uint8_t>(buffer);
                break;
            case 7:
                una::views::word::utf8(buffer);
                break;
            case 8:
                una::views::word_only::utf8(buffer);
                break;
        }
    }

    return 0;
}