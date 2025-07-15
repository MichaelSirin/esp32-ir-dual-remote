#ifndef SK_IR_CODES_H
#define SK_IR_CODES_H

// Define LG TV IR remote structure (NEC, 38 kHz, 32 bit)
typedef struct LG_Remote_t {
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
} LG_Remote_t;

const LG_Remote_t RemoteCodes =
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

#endif // SK_IR_CODES_H