#ifndef SK_IR_CODES_H
#define SK_IR_CODES_H

// Define Remote structure (NEC, 38 kHz, 32 bit)
typedef struct Remote_t {
  unsigned int  codeLen;
  unsigned long btnOnOff;
  unsigned long btn123;       // not used
  unsigned long btnExtra;     // not used
  unsigned long btnUp;
  unsigned long btnDown;
  unsigned long btnLeft;
  unsigned long btnRight;
  unsigned long btnOK;
  unsigned long btnReturn;
  unsigned long btnHome;
  unsigned long btnPlayPause;
  unsigned long btnVolUp;
  unsigned long btnVolDown;
  unsigned long btnVolEnter;  // Mute
  unsigned long btnChUp;
  unsigned long btnChDown;
  unsigned long btnChEnter;   // not used
} Remote_t;

const Remote_t RemoteCodes_LG =
{
  .codeLen      = 32,

  // Power
  .btnOnOff     = 0x20DF10EF,

  // non-used buttons
  .btn123       = 0x00000000,
  .btnExtra     = 0x00000000,

  // Navigation
  .btnUp        = 0x20DF02FD,
  .btnDown      = 0x20DF827D,
  .btnLeft      = 0x20DFE01F,
  .btnRight     = 0x20DF609F,
  .btnOK        = 0x20DF22DD,
  .btnReturn    = 0x20DF14EB,  // Back/Return
  .btnHome      = 0x20DF3EC1,
  .btnPlayPause = 0x20DF0DF2,

  // Volume and Channels
  .btnVolUp     = 0x20DF40BF,  // Vol+
  .btnVolDown   = 0x20DFC03F,  // Vol–
  .btnVolEnter  = 0x20DF906F,  // Mute
  .btnChUp      = 0x20DF00FF,  // Ch+
  .btnChDown    = 0x20DF807F,  // Ch–
  .btnChEnter   = 0x00000000   // not used
};

const Remote_t RemoteCodes_SAMSUNG =
{
    .codeLen      = 32,

    // Power
    .btnOnOff     = 0xE0E040BF,

    // non-used buttons
    .btn123       = 0xE0E04BB4,
    .btnExtra     = 0xE0E0807F, // Source :contentReference[oaicite:2]{index=2

    // Navigation
    .btnUp        = 0xE0E006F9,
    .btnDown      = 0xE0E08679,
    .btnLeft      = 0xE0E0A659,
    .btnRight     = 0xE0E046B9,
    .btnOK        = 0xE0E016E9,
    .btnReturn    = 0xE0E01AE5,
    .btnHome      = 0xE0E09E61,
    .btnPlayPause = 0xE0E0E21D,  

    // Volume and Channels
    .btnVolUp     = 0xE0E0E01F, // Vol+ :contentReference[oaicite:11]{index=11}
    .btnVolDown   = 0xE0E0D02F, // Vol– :contentReference[oaicite:12]{index=12}
    .btnVolEnter  = 0xE0E0F00F, // Mute :contentReference[oaicite:13]{index=13}

    .btnChUp      = 0xE0E048B7, // Prog+ (Channel Up) :contentReference[oaicite:14]{index=14}
    .btnChDown    = 0xE0E008F7, // Prog– (Channel Down) :contentReference[oaicite:15]{index=15}
    .btnChEnter   = 0xE0E0C837  // Prev-Ch (Last Channel) :contentReference[oaicite:16]{index=16}
};

#endif // SK_IR_CODES_H