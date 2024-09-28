//====================== INCLUDES =====================
#include "trainingData.h"

//Look Up Table - LUT - 1K Hz
const uint32_t triangularWaveLUT[] = { 
    0x10,   0x20,   0x31,   0x41,   0x52,   0x62,   0x72,   0x83,   0x93,   0xA3,
    0xB4,   0xC4,   0xD5,   0xE5,   0xF5,   0x106,  0x116,  0x127,  0x137,  0x147,
    0x158,  0x168,  0x179,  0x189,  0x199,  0x1AA,  0x1BA,  0x1CB,  0x1DB,  0x1EB,
    0x1FC,  0x20C,  0x21D,  0x22D,  0x23D,  0x24E,  0x25E,  0x26F,  0x27F,  0x290,
    0x2A0,  0x2B0,  0x2C1,  0x2D1,  0x2E2,  0x2F2,  0x302,  0x313,  0x323,  0x334,
    0x344,  0x354,  0x365,  0x375,  0x386,  0x396,  0x3A6,  0x3B7,  0x3C7,  0x3D8,
    0x3E8,  0x3F8,  0x409,  0x419,  0x42A,  0x43A,  0x44A,  0x45B,  0x46B,  0x47C,
    0x48C,  0x49C,  0x4AD,  0x4BD,  0x4CE,  0x4DE,  0x4EE,  0x4FF,  0x50F,  0x520,
    0x530,  0x540,  0x551,  0x561,  0x572,  0x582,  0x592,  0x5A3,  0x5B3,  0x5C4,
    0x5D4,  0x5E4,  0x5F5,  0x605,  0x616,  0x626,  0x636,  0x647,  0x657,  0x668,
    0x678,  0x688,  0x699,  0x6A9,  0x6BA,  0x6CA,  0x6DA,  0x6EB,  0x6FB,  0x70C,
    0x71C,  0x72C,  0x73D,  0x74D,  0x75E,  0x76E,  0x77E,  0x78F,  0x79F,  0x7B0,
    0x7C0,  0x7D0,  0x7E1,  0x7F1,  0x802,  0x812,  0x822,  0x833,  0x843,  0x854,
    0x864,  0x874,  0x885,  0x895,  0x8A6,  0x8B6,  0x8C6,  0x8D7,  0x8E7,  0x8F8,
    0x908,  0x918,  0x929,  0x939,  0x94A,  0x95A,  0x96A,  0x97B,  0x98B,  0x99B,
    0x9AC,  0x9BC,  0x9CD,  0x9DD,  0x9ED,  0x9FE,  0xA0E,  0xA1F,  0xA2F,  0xA3F,
    0xA50,  0xA60,  0xA71,  0xA81,  0xA91,  0xAA2,  0xAB2,  0xAC3,  0xAD3,  0xAE3,
    0xAF4,  0xB04,  0xB15,  0xB25,  0xB35,  0xB46,  0xB56,  0xB67,  0xB77,  0xB87,
    0xB98,  0xBA8,  0xBB9,  0xBC9,  0xBD9,  0xBEA,  0xBFA,  0xC0B,  0xC1B,  0xC2B,
    0xC3C,  0xC4C,  0xC5D,  0xC6D,  0xC7D,  0xC8E,  0xC9E,  0xCAF,  0xCBF,  0xCCF,
    0xCE0,  0xCF0,  0xD01,  0xD11,  0xD21,  0xD32,  0xD42,  0xD53,  0xD63,  0xD73,
    0xD84,  0xD94,  0xDA5,  0xDB5,  0xDC5,  0xDD6,  0xDE6,  0xDF7,  0xE07,  0xE17,
    0xE28,  0xE38,  0xE49,  0xE59,  0xE69,  0xE7A,  0xE8A,  0xE9B,  0xEAB,  0xEBB,
    0xECC,  0xEDC,  0xEED,  0xEFD,  0xF0D,  0xF1E,  0xF2E,  0xF3F,  0xF4F,  0xF5F,
    0xF70,  0xF80,  0xF91,  0xFA1,  0xFB1,  0xFC2,  0xFD2,  0xFE3,  0xFF3,  0xFFF,
    0xFF3,  0xFE3,  0xFD2,  0xFC2,  0xFB1,  0xFA1,  0xF91,  0xF80,  0xF70,  0xF5F,
    0xF4F,  0xF3F,  0xF2E,  0xF1E,  0xF0D,  0xEFD,  0xEED,  0xEDC,  0xECC,  0xEBB,
    0xEAB,  0xE9B,  0xE8A,  0xE7A,  0xE69,  0xE59,  0xE49,  0xE38,  0xE28,  0xE17,
    0xE07,  0xDF7,  0xDE6,  0xDD6,  0xDC5,  0xDB5,  0xDA5,  0xD94,  0xD84,  0xD73,
    0xD63,  0xD53,  0xD42,  0xD32,  0xD21,  0xD11,  0xD01,  0xCF0,  0xCE0,  0xCCF,
    0xCBF,  0xCAF,  0xC9E,  0xC8E,  0xC7D,  0xC6D,  0xC5D,  0xC4C,  0xC3C,  0xC2B,
    0xC1B,  0xC0B,  0xBFA,  0xBEA,  0xBD9,  0xBC9,  0xBB9,  0xBA8,  0xB98,  0xB87,
    0xB77,  0xB67,  0xB56,  0xB46,  0xB35,  0xB25,  0xB15,  0xB04,  0xAF4,  0xAE3,
    0xAD3,  0xAC3,  0xAB2,  0xAA2,  0xA91,  0xA81,  0xA71,  0xA60,  0xA50,  0xA3F,
    0xA2F,  0xA1F,  0xA0E,  0x9FE,  0x9ED,  0x9DD,  0x9CD,  0x9BC,  0x9AC,  0x99B,
    0x98B,  0x97B,  0x96A,  0x95A,  0x94A,  0x939,  0x929,  0x918,  0x908,  0x8F8,
    0x8E7,  0x8D7,  0x8C6,  0x8B6,  0x8A6,  0x895,  0x885,  0x874,  0x864,  0x854,
    0x843,  0x833,  0x822,  0x812,  0x802,  0x7F1,  0x7E1,  0x7D0,  0x7C0,  0x7B0,
    0x79F,  0x78F,  0x77E,  0x76E,  0x75E,  0x74D,  0x73D,  0x72C,  0x71C,  0x70C,
    0x6FB,  0x6EB,  0x6DA,  0x6CA,  0x6BA,  0x6A9,  0x699,  0x688,  0x678,  0x668,
    0x657,  0x647,  0x636,  0x626,  0x616,  0x605,  0x5F5,  0x5E4,  0x5D4,  0x5C4,
    0x5B3,  0x5A3,  0x592,  0x582,  0x572,  0x561,  0x551,  0x540,  0x530,  0x520,
    0x50F,  0x4FF,  0x4EE,  0x4DE,  0x4CE,  0x4BD,  0x4AD,  0x49C,  0x48C,  0x47C,
    0x46B,  0x45B,  0x44A,  0x43A,  0x42A,  0x419,  0x409,  0x3F8,  0x3E8,  0x3D8,
    0x3C7,  0x3B7,  0x3A6,  0x396,  0x386,  0x375,  0x365,  0x354,  0x344,  0x334,
    0x323,  0x313,  0x302,  0x2F2,  0x2E2,  0x2D1,  0x2C1,  0x2B0,  0x2A0,  0x28F,
    0x27F,  0x26F,  0x25E,  0x24E,  0x23D,  0x22D,  0x21D,  0x20C,  0x1FC,  0x1EB,
    0x1DB,  0x1CB,  0x1BA,  0x1AA,  0x199,  0x189,  0x179,  0x168,  0x158,  0x147,
    0x137,  0x127,  0x116,  0x106,  0xF5,   0xE5,   0xD5,   0xC4,   0xB4,   0xA3,
    0x93,   0x83,   0x72,   0x62,   0x51,   0x41,   0x31,   0x20,   0x10,   0x0 };

