import queue
CAN = {
    "0x0000": queue.Queue(),  # STR_WHEEL_ID
    "0x0001": queue.Queue(),  # STR_WHEEL2_ID
    "0x0002": queue.Queue(),  # STR_WHEEL3_ID
    "0x0007": queue.Queue(),  # STR_WHEEL6_ID
    "0x0080": queue.Queue(),  # m_GMU_1_ID
    "0x0081": queue.Queue(),  # m_GMU_2_ID
    "0x0450": queue.Queue(),  # GPS1_ID
    "0x0451": queue.Queue(),  # GPS2_ID
    "0x0452": queue.Queue(),  # GPS3_ID
    "0x0453": queue.Queue(),  # GPS4_ID
    "0x0500": queue.Queue(),  # BB_ID
    "0x0501": queue.Queue(),  # PEDALS_ID
    "0x0710": queue.Queue(),  # pmu1_ID
    "0x0400": queue.Queue(),  # ADU_ID
    "0x0401": queue.Queue(),  # ADU_ACC_ID
    "0x0402": queue.Queue(),  # ADU_GYR_ID
    "0x0200": queue.Queue(),  # m_WH_MODULES_1_ID
    "0x0201": queue.Queue(),  # m_WH_MODULES_2_ID
    "0x0202": queue.Queue(),  # m_WH_MODULES_3_ID
    "0x0203": queue.Queue(),  # m_WH_MODULES_4_ID
    "0x0204": queue.Queue(),  # m_WH_MODULES_5_ID
    "0x0205": queue.Queue(),  # m_WH_MODULES_6_ID
    "0x0206": queue.Queue(),  # m_WH_MODULES_7_ID
    "0x0207": queue.Queue(),  # m_WH_MODULES_8_ID
    "0x0660": queue.Queue(),  # EGT_TO_CAN1_ID
    "0x0661": queue.Queue(),  # EGT_TO_CAN2_ID
}