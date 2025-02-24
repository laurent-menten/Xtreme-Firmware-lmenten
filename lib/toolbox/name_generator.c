#include "name_generator.h"

#include <stdio.h>
#include <stdint.h>
#include <furi_hal_rtc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <furi.h>
#include <xtreme/xtreme.h>

const char* const name_generator_left[] = {
    "super",
    "big",
    "little",
    "liquid",
    "unknown",
    "thin",
    "thick",
    "great",
    "my",
    "mini",
    "ultra",
    "haupt",
    "small",
    "random",
    "strange",
};

const char* const name_generator_right[] = {
    "maslina",
    "sus",
    "anomalija",
    "artefact",
    "monolit",
    "burer",
    "sidorovich",
    "habar",
    "radar",
    "borov",
    "pda",
    "konserva",
    "aptechka",
    "door",
    "thing",
    "stuff",
};

void name_generator_make_auto_datetime(
    char* name,
    size_t max_name_size,
    const char* prefix,
    FuriHalRtcDateTime* custom_time) {
    if(!furi_hal_rtc_is_flag_set(FuriHalRtcFlagRandomFilename)) {
        name_generator_make_detailed_datetime(name, max_name_size, prefix, custom_time);
    } else {
        name_generator_make_random_prefixed(name, max_name_size, prefix);
    }
}

void name_generator_make_auto(char* name, size_t max_name_size, const char* prefix) {
    name_generator_make_auto_datetime(name, max_name_size, prefix, NULL);
}

void name_generator_make_random_prefixed(char* name, size_t max_name_size, const char* prefix) {
    furi_assert(name);
    furi_assert(max_name_size);

    uint8_t name_generator_left_i = rand() % COUNT_OF(name_generator_left);
    uint8_t name_generator_right_i = rand() % COUNT_OF(name_generator_right);

    if(xtreme_settings.file_naming_prefix_after) {
        snprintf(
            name,
            max_name_size,
            "%s-%s%s%s",
            name_generator_left[name_generator_left_i],
            name_generator_right[name_generator_right_i],
            prefix ? "_" : "",
            prefix ? prefix : "");
    } else {
        snprintf(
            name,
            max_name_size,
            "%s%s%s-%s",
            prefix ? prefix : "",
            prefix ? "_" : "",
            name_generator_left[name_generator_left_i],
            name_generator_right[name_generator_right_i]);
    }

    // Set first symbol to upper case
    if(islower((int)name[0])) name[0] = name[0] - 0x20;
}

void name_generator_make_random(char* name, size_t max_name_size) {
    name_generator_make_random_prefixed(name, max_name_size, NULL);
}

void name_generator_make_detailed_datetime(
    char* name,
    size_t max_name_size,
    const char* prefix,
    FuriHalRtcDateTime* custom_time) {
    furi_assert(name);
    furi_assert(max_name_size);
    furi_assert(prefix);

    FuriHalRtcDateTime dateTime;
    if(custom_time) {
        dateTime = *custom_time;
    } else {
        furi_hal_rtc_get_datetime(&dateTime);
    }

    if(xtreme_settings.file_naming_prefix_after) {
        snprintf(
            name,
            max_name_size,
            "%.4d-%.2d-%.2d_%.2d,%.2d,%.2d_%s",
            dateTime.year,
            dateTime.month,
            dateTime.day,
            dateTime.hour,
            dateTime.minute,
            dateTime.second,
            prefix);
    } else {
        snprintf(
            name,
            max_name_size,
            "%s_%.4d-%.2d-%.2d_%.2d,%.2d,%.2d",
            prefix,
            dateTime.year,
            dateTime.month,
            dateTime.day,
            dateTime.hour,
            dateTime.minute,
            dateTime.second);
    }

    // Set first symbol to upper case
    if(islower((int)name[0])) name[0] = name[0] - 0x20;
}

void name_generator_make_detailed(char* name, size_t max_name_size, const char* prefix) {
    name_generator_make_detailed_datetime(name, max_name_size, prefix, NULL);
}
