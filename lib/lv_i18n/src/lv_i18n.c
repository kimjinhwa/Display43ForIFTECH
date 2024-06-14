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
    {"ESC", "ESC"},
    {"ON", "RUN"},
    {"OFF", "STOP"},
    {"INTRO", "INTRO"},
    {"SYSTEM", "SYSTEM"},
    {"UPS_SETUP", "UPS SETUP"},
    {"Gain_SETUP", "Gain SETUP"},
    {"GainVIEW", "Gain View"},
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
    {"EEPROM_Error", "EEPROM Error"},
    {"Bypass_mode", "Bypass mode"},
    {"screen_set", "SET"},
    {"controll", "CONTROLL"},
    {"MEASURE", "MEASURE"},
    {"info", "LOG Data"},
    {"Language", "언어"},
    {"English", "한국어"},
    {"Brigtness", "Brigtness"},
    {"OFFTIME", "OFF TIME"},
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
    {"RUN", "RUN"},
    {"STOP", "STOP"},
    {"UPS_SYSTEM", "UPS SYSTEM"},
    {"dcLink", "DC LINK"},
    {"Battery", "BATTERY"},
    {"Inverter", "INVERTER"},
    {"output", "OUTPUT"},
    {"input", "INPUT"},
    {"Mesurement", "Mesurement"},
    {"LogData", "Log Data"},
    {"Prev", "Prev"},
    {"Next", "Next"},
    {"alarmStatus", "Alarm History"},
    {"alarmHistory", "Alarm Status"},
    {"Capacity", "Capacity"},
    {"Input_Voltage", "Input Voltage"},
    {"Output_Voltage", "Output Voltage"},
    {"Battery_Voltagte", "Battery Voltagte"},
    {"UPS_ON", "UPS_ON"},
    {"UPS_OFF", "UPS_OFF"},
    {"ALARM_RST", "ALARM RST"},
    {"Bat_Curr_Set", "Bat Curr Set"},
    {"Bat_Volt_Set", "Bat Volt Set"},
    {"Inv_Volt_Set", "Inv Volt Set"},
    {"HF_Mode", "HF Mode"},
    {"R_C_Start_Run_Event", "R/C Start Run Event"},
    {"R_C_Re_Start_Event", "R/C Re-Start Event"},
    {"R_C_Stop_by_Command_Event", "R/C Stop by Command Event"},
    {"R_C_Stop_by_Fault_Event", "R/C Stop by Fault Event"},
    {"DC_DC_Start_Run_Event", "DC/DC Start Run Event"},
    {"DC_DC_Re_Start_Event", "DC/DC Re-Start Event"},
    {"DC_DC_Stop_by_Command_Event", "DC/DC Stop by Command Event"},
    {"DC_DC_Stop_by_Fault_Event", "DC/DC Stop by Fault Event"},
    {"Inverter_Start_Run_Event", "Inverter Start Run Event"},
    {"Inverter_Re_Start_Run_Event", "Inverter Re-Start Run Event"},
    {"Inverter_Stop_by_Command_Event", "Inverter Stop by Command Event"},
    {"Inverter_Stop_by_Fault_Event", "Inverter Stop by Fault Event"},
    {"Transfer_to_Inverter_Event", "Transfer to Inverter Event"},
    {"Transfer_to_Bypass_by_Com_Event", "Transfer to Bypass by Com Event"},
    {"Transfer_to_Bypass_by_fault_Event", "Transfer to Bypass by fault Event"},
    {"Inverter_Over_Current_fault", "Inverter Over Current fault"},
    {"DC_Link_OV_fault", "DC Link OV fault"},
    {"Converter_GDU_fault", "Converter GDU fault"},
    {"Inverter_GDU_fault", "Inverter GDU fault"},
    {"DC_DC_GDU_fault", "DC/DC GDU fault"},
    {"Common_GDU_fault", "Common GDU fault"},
    {"Module_OT", "Module OT"},
    {"Battery_Fuse_fault", "Battery Fuse fault"},
    {"EEPROM_ERR", "EEPROM ERR"},
    {"Battery_MCCB_Fault", "Battery MCCB Fault"},
    {"converter_current_limit_fault", "converter current limit fault"},
    {"Vdc_link_OV_limit_fault", "Vdc_link OV limit fault"},
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
    {"Output_Overload", "Output Overload"},
    {"Inverter_Overload_Stop", "Inverter Overload Stop"},
    {"OFFSET_Check_ERR", "OFFSET Check ERR"},
    {"input_volt_rms", "input_volt_rms"},
    {"input_curr_rms", "input_curr_rms"},
    {"vdc_link_volt_rms", "vdc_link_volt_rms"},
    {"bat_volt_rms", "bat_volt_rms"},
    {"bat_current_rms", "bat_current_rms"},
    {"inv_volt_rms", "inv_volt_rms"},
    {"inv_current_rms", "inv_current_rms"},
    {"output_volt_rms", "output_volt_rms"},
    {"output_curr_rms", "output_curr_rms"},
    {"input_freqency", "input_freqency"},
    {"inv_freqency", "inv_freqency"},
    {"bypass_freqency", "bypass_freqency"},
    {"Battery_Capacity", "Battery Capacity"},
    {"Load_Percent", "Load Percent"},
    {"UPS_Inner_Temp", "UPS Inner Temp"},
    {"Input_volt_gain", "Input_volt_gain"},
    {"input_curr_gain", "input_curr_gain"},
    {"vdc_link_volt_gain", "vdc_link_volt_gain"},
    {"vbat_volt_gain", "vbat_volt_gain"},
    {"bat_current_gain", "bat_current_gain"},
    {"inverter_volt_gain", "inverter_volt_gain"},
    {"inverter_curr_gain", "inverter_curr_gain"},
    {"output_curr_gain", "output_curr_gain"},
    {"SAVED", "SAVED"},
    {"SAVE", "SAVE"},
    {"CANCEL", "CANCEL"},
    {"BACKSPACE", "DEL"},
    {"Comm_Error", "Comm Error"},
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
    {"ESC", "초기화면"},
    {"ON", "운전"},
    {"OFF", "정l'"},
    {"INTRO", "초기화면"},
    {"SYSTEM", "시스템"},
    {"UPS_SETUP", "UPS 설정"},
    {"Gain_SETUP", "이득 설정"},
    {"GainVIEW", "이득 보기"},
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
    {"EEPROM_Error", "메모리에러"},
    {"Bypass_mode", "바이패스 운전모드"},
    {"screen_set", "설  정"},
    {"controll", "제 어"},
    {"MEASURE", "계 측"},
    {"info", "로  그"},
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
    {"RUN", "운 전"},
    {"STOP", "정 지"},
    {"UPS_SYSTEM", "무정전전원공급장치"},
    {"dcLink", "직류링크"},
    {"Battery", "축 전 지"},
    {"Inverter", "인  버  터"},
    {"output", "출        력"},
    {"input", "입        력"},
    {"Mesurement", "계 측 값"},
    {"LogData", "경보이력"},
    {"Prev", "이 전"},
    {"Next", "다 음"},
    {"alarmStatus", "알람이력"},
    {"alarmHistory", "경보상태"},
    {"Capacity", "정격용량"},
    {"Input_Voltage", "입력전압"},
    {"Output_Voltage", "출력전압"},
    {"Battery_Voltagte", "배터리전압"},
    {"UPS_ON", "UPS 운전"},
    {"UPS_OFF", "UPS 정지"},
    {"ALARM_RST", "알람정지"},
    {"Bat_Curr_Set", "충전전류 설정"},
    {"Bat_Volt_Set", "배터리전압 설정"},
    {"Inv_Volt_Set", "출력전압 설정"},
    {"HF_Mode", "고효율 모드"},
    {"R_C_Start_Run_Event", "충전기 운전"},
    {"R_C_Re_Start_Event", "충전기 자동운전"},
    {"R_C_Stop_by_Command_Event", "충전기 정지"},
    {"R_C_Stop_by_Fault_Event", "이상발생 충전기 정지"},
    {"DC_DC_Start_Run_Event", "DC/DC 운전"},
    {"DC_DC_Re_Start_Event", "DC/DC 자동운전"},
    {"DC_DC_Stop_by_Command_Event", "DC/DC 정지"},
    {"DC_DC_Stop_by_Fault_Event", "이상발생 DC/DC 정지"},
    {"Inverter_Start_Run_Event", "인버터 운전"},
    {"Inverter_Re_Start_Run_Event", "인버터 자동운전"},
    {"Inverter_Stop_by_Command_Event", "인버터 정지"},
    {"Inverter_Stop_by_Fault_Event", "이상발생 인버터 정지"},
    {"Transfer_to_Inverter_Event", "인버터로 절환"},
    {"Transfer_to_Bypass_by_Com_Event", "바이패스 절환"},
    {"Transfer_to_Bypass_by_fault_Event", "이상발생 바이패스 절환"},
    {"Inverter_Over_Current_fault", "인버터 과전류 이상"},
    {"DC_Link_OV_fault", "직류링크 과전압 이상"},
    {"Converter_GDU_fault", "충전기 GDU 이상"},
    {"Inverter_GDU_fault", "인버터 GDU 이상"},
    {"DC_DC_GDU_fault", "DC/DC GDU 이상"},
    {"Common_GDU_fault", "N상 게이트 이상"},
    {"Module_OT", "모듈 과온도"},
    {"Battery_Fuse_fault", "배터리 퓨즈 이상"},
    {"EEPROM_ERR", "메모리 이상"},
    {"Battery_MCCB_Fault", "배터리 차단기 이상"},
    {"converter_current_limit_fault", "충전기 전류제한 이상"},
    {"Vdc_link_OV_limit_fault", "직류링크 과전압제한이상"},
    {"Vdc_link_UV_limit_fault", "직류링크 저전압제한이상"},
    {"input_under_voltage_fault", "입력 저전압 이상"},
    {"input_over_voltage_fault", "입력 고전압 이상"},
    {"Input_Frequency_fault", "입력 주파수 이상"},
    {"Inverter_Frequency_fault", "인버터 주파수 이상"},
    {"utility_line_failure", "정전"},
    {"battery_current_limit_fault", "배터리 전류제한 이상"},
    {"Battery_OV_limit_fault", "배터리 과전압제한이상"},
    {"battery_UV_limit_fault", "배터리 저전압제한이상"},
    {"inverter_output_voltage_fault", "인버터 출력전압이상"},
    {"Output_Overload", "출력 과부하"},
    {"Inverter_Overload_Stop", "인버터 과부하 정지"},
    {"OFFSET_Check_ERR", "옵셋체크 이상"},
    {"input_volt_rms", "입력 전압"},
    {"input_curr_rms", "입력 전류"},
    {"vdc_link_volt_rms", "직류링크 전압"},
    {"bat_volt_rms", "배터리 전압"},
    {"bat_current_rms", "배터리 전류"},
    {"inv_volt_rms", "인버터 전압"},
    {"inv_current_rms", "인버터 전류"},
    {"output_volt_rms", "출력 전압"},
    {"output_curr_rms", "출력 전류"},
    {"input_freqency", "입력 주파수"},
    {"inv_freqency", "인버터 주파수"},
    {"bypass_freqency", "바이패스 주파수"},
    {"Battery_Capacity", "배터리 용량"},
    {"Load_Percent", "부하율"},
    {"UPS_Inner_Temp", "UPS 내부 온도"},
    {"Input_volt_gain", "입력전압 이득"},
    {"input_curr_gain", "입력전류 이득"},
    {"vdc_link_volt_gain", "직류링크전압 이득"},
    {"vbat_volt_gain", "배터리전압 이득"},
    {"bat_current_gain", "배터리전류 이득"},
    {"inverter_volt_gain", "인버터전압 이득"},
    {"inverter_curr_gain", "인버터전류 이득"},
    {"output_curr_gain", "출력 전류이득"},
    {"SAVED", "저장완료"},
    {"SAVE", "저장"},
    {"CANCEL", "취소"},
    {"BACKSPACE", "삭제"},
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
