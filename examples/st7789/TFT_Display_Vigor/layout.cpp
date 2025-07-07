#include "layout.h"

std::unordered_map<std::string, TextBoxDefinition> textBoxDefs = {
    {"tb_vend_ist", {"hmi_vend_ist", "1234", 42, 48, 96, 48}},
    {"tb_vend_ist_l", {"hmi_vend_ist", "1234", 42, 48, 96, 16}},
    {"tb_vend_ist_r", {"hmi_vend_ist", "1234", 172, 48, 96, 16}},
    {"tb_richtig", {"", "richtig kalibriert?", 8, 10, 304, 32}},
    {"tb_vend_soll", {"hmi_vend_soll", "1234", 10, 96, 128, 48}},
    {"tb_pos_l", {"hmi_pos_l", "12%", 10, 96, 128, 48}},
    {"tb_pos_r", {"hmi_pos_r", "12%", 140, 96, 128, 48}},
    {"tb_soll_l", {"hmi_soll_l", "12%", 42, 159, 32, 16}},
    {"tb_soll_r", {"hmi_soll_r", "12%", 172, 159, 32, 16}},
    {"tb_speed", {"hmi_speed", "1234.56 km/h", 10, 214, 88, 16}},
    {"tb_gps", {"hmi_gps", "GPS: N00.0000 E00.0000", 148, 214, 88, 16}},
    {"tb_feldname", {"hmi_feldname", "Feldname: ", 10, 10, 120, 16}},
    {"tb_fehler", {"", "Fehlercode: 0xGG", 16, 16, 288, 32}},
    {"tb_fehlerbeschrieb", {"hmi_fehler", "Fehlerbeschreibung", 16, 52, 288, 16}},
    {"tb_b1_auf", {"", "auf", 262, 14, 48, 32}},
    {"tb_b1_+", {"", "+", 294, 14, 16, 32}},
    {"tb_b2_zu", {"", "zu", 278, 74, 48, 32}},
    {"tb_b2_-", {"", "-", 294, 74, 16, 32}},
    {"tb_b3_ja", {"", "Ja", 278, 134, 32, 32}},
    {"tb_b3_auf", {"", "auf", 278, 134, 48, 32}},
    {"tb_b4_lr", {"", "L/R", 262, 194, 48, 32}},
    {"tb_b4_nein", {"", "Nein", 246, 194, 64, 32}},
    {"tb_b4_zu", {"", "zu", 262, 194, 48, 32}},
    {"tb_b4_quittieren", {"", "quittieren", 1150, 194, 64, 32}},
};


std::unordered_map<std::string, Screen> screens = {
    {
        "INIT", Screen{
            {        
                {"tb_richtig", RVLC_YELLOW},
                {"tb_vend_ist", RVLC_YELLOW},
                {"tb_b3_ja", RVLC_YELLOW},
                {"tb_b4_nein", RVLC_YELLOW}
            }
        }
    },
    {
        "CALIB", Screen{
            {
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_soll", RVLC_YELLOW},
                {"tb_b1_+", RVLC_YELLOW},
                {"tb_b2_-", RVLC_YELLOW},
                {"tb_b3_auf", RVLC_YELLOW},
                {"tb_b4_zu", RVLC_YELLOW}
            }
        }
    },
    {
        "MANUAL_L", Screen{
            {
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_YELLOW},
                {"tb_pos_r", RVLC_GREY},
                {"tb_b1_auf", RVLC_YELLOW},
                {"tb_b2_zu", RVLC_YELLOW},
                {"tb_b4_lr", RVLC_YELLOW}
            }
        }
    },
    {
        "MANUAL_R", Screen{
            {
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_GREY},
                {"tb_pos_r", RVLC_YELLOW},
                {"tb_b1_auf", RVLC_YELLOW},
                {"tb_b2_zu", RVLC_YELLOW},
                {"tb_b4_lr", RVLC_YELLOW}
            }
        }
    },
    {
        "SEMI", Screen{
            {
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_YELLOW},
                {"tb_pos_r", RVLC_YELLOW},
                {"tb_soll_l", RVLC_YELLOW},
                {"tb_soll_r", RVLC_YELLOW},
                {"tb_b1_auf", RVLC_YELLOW},
                {"tb_b2_zu", RVLC_YELLOW}
            }
        }
    },
    {
        "AUTO", Screen{
            {
                {"tb_feldname", RVLC_YELLOW},
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_YELLOW},
                {"tb_pos_r", RVLC_YELLOW},
                {"tb_soll_l", RVLC_YELLOW},
                {"tb_soll_r", RVLC_YELLOW},
                {"tb_speed", RVLC_YELLOW},
                {"tb_gps", RVLC_YELLOW}
            }
        }
    },
    {
        "EDGE_L", Screen{
            {
                {"tb_feldname", RVLC_YELLOW},
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_YELLOW},
                {"tb_pos_r", RVLC_GREY},
                {"tb_soll_l", RVLC_YELLOW},
                {"tb_soll_r", RVLC_GREY},
                {"tb_speed", RVLC_YELLOW},
                {"tb_gps", RVLC_YELLOW},
                {"tb_b1_auf", RVLC_YELLOW},
                {"tb_b2_zu", RVLC_YELLOW},
                {"tb_b4_lr", RVLC_YELLOW}
            }
        }
    },
    {
        "EDGE_R", Screen{
            {
                {"tb_feldname", RVLC_YELLOW},
                {"tb_vend_ist_l", RVLC_GREY},
                {"tb_vend_ist_r", RVLC_GREY},
                {"tb_pos_l", RVLC_GREY},
                {"tb_pos_r", RVLC_YELLOW},
                {"tb_soll_l", RVLC_GREY},
                {"tb_soll_r", RVLC_YELLOW},
                {"tb_speed", RVLC_YELLOW},
                {"tb_gps", RVLC_YELLOW},
                {"tb_b1_auf", RVLC_YELLOW},
                {"tb_b2_zu", RVLC_YELLOW},
                {"tb_b4_lr", RVLC_YELLOW}
            }
        }
    },
    {
        "ERROR", Screen{
            {
                {"tb_fehler", RVLC_RED},
                {"tb_fehlerbeschrieb", RVLC_RED},
                {"tb_b4_quittieren", RVLC_YELLOW}
            }
        }
    }
};
