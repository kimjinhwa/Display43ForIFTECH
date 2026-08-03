#include "./lv_i18n.h"


////////////////////////////////////////////////////////////////////////////////
// Define plural operands
// http://unicode.org/reports/tr35/tr35-numbers.html#Operands

// Integer version, simplified

#define UNUSED(x) (void)(x)

static inline uint32_t op_n(int32_t val) { return (uint32_t)(val < 0 ? -val : val); }
static inline uint32_t op_i(uint32_t val) { return val; }
// always zero, when decimal part not exists.
static inline uint32_t op_v(uint32_t val) { UNUSED(val); return 0;}
static inline uint32_t op_w(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_f(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_t(uint32_t val) { UNUSED(val); return 0; }

static lv_i18n_phrase_t en_gb_singulars[] = {
    {"Input_OC", "Input_OC"},
    {"Inverter_OC", "Inverter OC"},
    {"Vdc_link_OV", "Vdc link OV"},
    {"CONVERTER_OFF", "CONVERTER OFF"},
    {"DC_DC_OFF", "DC/DC OFF"},
    {"Converter_GDU", "Converter GDU"},
    {"Inverter_GDU", "Inverter_GDU"},
    {"DC_DC_GDU", "DC/DC GDU"},
    {"Common_GDU", "Common GDU"},
    {"Earth_fault", "Earth fault"},
    {"Fuse_Open", "Fuse Open"},
    {"Module_OT", "Module OT"},
    {"EEPROM_Error", "EEPROM Error"},
    {"Input_current_limit_fault", "Input current limit_fault"},
    {"Vdc_link_OV_limit fault", "Vdc_link_OV_limit_fault"},
    {"Vdc_link_UV_limit_fault", "Vdc_link UV limit fault"},
    {"input_under_voltage_fault", "input under voltage fault"},
    {"input_over_voltage_fault", "input over voltage fault"},
    {"Input_Frequency_fault", "Input Frequency fault"},
    {"Inverter_Frequency_fault", "Inverter Frequency fault"},
    {"utility_line_failure", "utility line failure"},
    {"battery_current_limit_fault", "battery current limit fault"},
    {"Battery_OV_limit_fault", "Battery OV limit fault"},
    {"battery_UV_limit_fault", "battery UV limit fault"},
    {"inverter_output_voltage_fault", "inverter output voltage fault"},
    {"Inverter_over_load_fault", "Inverter over load fault"},
    {"Inverter_over_load_over_time", "Inverter over load over time"},
    {"Bypass_mode", "Bypass mode"},
    {"controll", "CONTROLL"},
    {"measure", "MEASURE"},
    {"screen_set", "SET"},
    {"info", "ALARM"},
    {"Language", "언어"},
    {"English", "한국어"},
    {"Brigtness", "Brigtness"},
    {"OFFTIME", "OFF TIME(Sec)"},
    {"Year", "Year"},
    {"Month", "Month"},
    {"Day", "Month"},
    {"Hour", "Hour"},
    {"Minute", "Minute"},
    {"Second", "Second"},
    {"SET", "SET"},
    {"SetVTVol", "Bat Voltage"},
    {"SetVTCurrent", "Bat Current"},
    {"SetOutVol", "OUTPUT Voltage"},
    {"SetBatLowVol", "Bat. Low Voltage"},
    {"Back", "Back"},
    {"run", "RUN"},
    {"stop", "STOP"},
    {"Uninterruptable_Power_Supply", "무정전전원공급장치"},
    {"dcLink", "DC LINK"},
    {"Battery", "BATTERY"},
    {"Inverter", "INVERTER"},
    {"output", "OUTPUT"},
    {"input", "INPUT"},
    {"Mesurement", "Mesurement"},
    {"LogData", "Log Data"},
    {"before", "Prev"},
    {"next", "Next"},
    {"alarmStatus", "Alarm Status"},
    {"alarmHistory", "Alarm History"},
    {NULL, NULL} // End mark
};



static uint8_t en_gb_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t en_gb_lang = {
    .locale_name = "en-GB",
    .singulars = en_gb_singulars,

    .locale_plural_fn = en_gb_plural_fn
};

static lv_i18n_phrase_t ko_kr_singulars[] = {
    {"Input_OC", "입력 과전류"},
    {"Inverter_OC", "인버터 과전류"},
    {"Vdc_link_OV", "직류링크 과전압"},
    {"CONVERTER_OFF", "컨버터 정지"},
    {"DC_DC_OFF", "DC/DC 정지"},
    {"Converter_GDU", "컨버터 게이트 이상"},
    {"Inverter_GDU", "인버터 게이트 이상"},
    {"DC_DC_GDU", "DC-DC 게이트 이상"},
    {"Common_GDU", "N상 게이트 이상"},
    {"Earth_fault", "지락 고장"},
    {"Fuse_Open", "배터리 퓨즈 이상"},
    {"Module_OT", "모듈 과온도"},
    {"EEPROM_Error", "EEPROM Error"},
    {"Input_current_limit_fault", "입력 과전류 제한"},
    {"Vdc_link_OV_limit fault", "직류링크 과전압 제한초과"},
    {"Vdc_link_UV_limit_fault", "직류링크 저전압 제한초과"},
    {"input_under_voltage_fault", "입력 저전압"},
    {"input_over_voltage_fault", "입력 고전압"},
    {"Input_Frequency_fault", "입력 주파수 이상"},
    {"Inverter_Frequency_fault", "인버터 주파수 이상"},
    {"utility_line_failure", "정전"},
    {"battery_current_limit_fault", "배터리 전류 제한 이상"},
    {"Battery_OV_limit_fault", "배터리 과전압 제한 이상"},
    {"battery_UV_limit_fault", "배터리 저전압 제한 이상"},
    {"inverter_output_voltage_fault", "인버터 전압 이상"},
    {"Inverter_over_load_fault", "인버터 과부하 이상"},
    {"Inverter_over_load_over_time", "인버터 과부하 시간초과"},
    {"Bypass_mode", "바이패스 운전모드"},
    {"controll", "제  어"},
    {"measure", "계  측"},
    {"screen_set", "설  정"},
    {"info", "경  보"},
    {"Language", "Language"},
    {"English", "English"},
    {"Brigtness", "밝기"},
    {"OFFTIME", "꺼짐시간"},
    {"Year", "년"},
    {"Month", "월"},
    {"Day", "일"},
    {"Hour", "시"},
    {"Minute", "분"},
    {"Second", "초"},
    {"SET", "설 정"},
    {"SetVTVol", "축전지전압설정"},
    {"SetVTCurrent", "축전지전류설정"},
    {"SetOutVol", "출력전압설정"},
    {"SetBatLowVol", "축전지저전압설정"},
    {"Back", "뒤로"},
    {"run", "운 전"},
    {"stop", "정 지"},
    {"Uninterruptable_Power_Supply", "무정전전원공급장치"},
    {"dcLink", "직류링크"},
    {"Battery", "축 전 지"},
    {"Inverter", "인  버  터"},
    {"output", "출        력"},
    {"input", "입        력"},
    {"Mesurement", "계 측 값"},
    {"LogData", "경보이력"},
    {"before", "이 전"},
    {"next", "다 음"},
    {"alarmStatus", "경보상태"},
    {"alarmHistory", "알람이력"},
    {NULL, NULL} // End mark
};



static uint8_t ko_kr_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t ko_kr_lang = {

    .locale_name = "ko-KR",
    .singulars = ko_kr_singulars,

    .locale_plural_fn = ko_kr_plural_fn
};

const lv_i18n_language_pack_t lv_i18n_language_pack[] = {
    &en_gb_lang,
    &ko_kr_lang,
    NULL // End mark
};

////////////////////////////////////////////////////////////////////////////////


// Internal state
static const lv_i18n_language_pack_t * current_lang_pack;
static const lv_i18n_lang_t * current_lang;


/**
 * Reset internal state. For testing.
 */
void __lv_i18n_reset(void)
{
    current_lang_pack = NULL;
    current_lang = NULL;
}

/**
 * Set the languages for internationalization
 * @param langs pointer to the array of languages. (Last element has to be `NULL`)
 */
int lv_i18n_init(const lv_i18n_language_pack_t * langs)
{
    if(langs == NULL) return -1;
    if(langs[0] == NULL) return -1;

    current_lang_pack = langs;
    current_lang = langs[0];     /*Automatically select the first language*/
    return 0;
}

/**
 * Change the localization (language)
 * @param l_name name of the translation locale to use. E.g. "en-GB"
 */
int lv_i18n_set_locale(const char * l_name)
{
    if(current_lang_pack == NULL) return -1;

    uint16_t i;

    for(i = 0; current_lang_pack[i] != NULL; i++) {
        // Found -> finish
        if(strcmp(current_lang_pack[i]->locale_name, l_name) == 0) {
            current_lang = current_lang_pack[i];
            return 0;
        }
    }

    return -1;
}


static const char * __lv_i18n_get_text_core(lv_i18n_phrase_t * trans, const char * msg_id)
{
    uint16_t i;
    for(i = 0; trans[i].msg_id != NULL; i++) {
        if(strcmp(trans[i].msg_id, msg_id) == 0) {
            /*The msg_id has found. Check the translation*/
            if(trans[i].translation) return trans[i].translation;
        }
    }

    return NULL;
}


/**
 * Get the translation from a message ID
 * @param msg_id message ID
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text(const char * msg_id)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;

    // Search in current locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    return msg_id;
}

/**
 * Get the translation from a message ID and apply the language's plural rule to get correct form
 * @param msg_id message ID
 * @param num an integer to select the correct plural form
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text_plural(const char * msg_id, int32_t num)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;
    lv_i18n_plural_type_t ptype;

    // Search in current locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    return msg_id;
}

/**
 * Get the name of the currently used locale.
 * @return name of the currently used locale. E.g. "en-GB"
 */
const char * lv_i18n_get_current_locale(void)
{
    if(!current_lang) return NULL;
    return current_lang->locale_name;
}
