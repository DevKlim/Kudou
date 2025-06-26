#ifndef WSLAY_FRAME_H
#define WSLAY_FRAME_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <wslay/wslay.h>

enum wslay_frame_state {
  PREP_HEADER,
  PREP_HEADER_NOBUF,
  SEND_HEADER,
  SEND_PAYLOAD,
  RECV_HEADER1,
  RECV_PAYLOADLEN,
  RECV_EXT_PAYLOADLEN,
  RECV_MASKKEY,
  RECV_PAYLOAD
};

struct wslay_frame_opcode_memo {
  uint8_t fin;
  uint8_t opcode;
  uint8_t rsv;
};

struct wslay_frame_context {
  uint8_t ibuf[4096];
  uint8_t *ibufmark;
  uint8_t *ibuflimit;
  struct wslay_frame_opcode_memo iom;
  uint64_t ipayloadlen;
  uint64_t ipayloadoff;
  uint8_t imask;
  uint8_t imaskkey[4];
  enum wslay_frame_state istate;
  size_t ireqread;

  uint8_t oheader[14];
  uint8_t *oheadermark;
  uint8_t *oheaderlimit;
  uint64_t opayloadlen;
  uint64_t opayloadoff;
  uint8_t omask;
  uint8_t omaskkey[4];
  enum wslay_frame_state ostate;

  struct wslay_frame_callbacks callbacks;
  void *user_data;
};

#endif /* WSLAY_FRAME_H */
