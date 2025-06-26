#ifndef WSLAY_NET_H
#define WSLAY_NET_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <wslay/wslay.h>

#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif /* HAVE_ARPA_INET_H */
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif /* HAVE_NETINET_IN_H */
/* For Mingw build */
#ifdef HAVE_WINSOCK2_H
#  include <winsock2.h>
#endif /* HAVE_WINSOCK2_H */

#ifdef WORDS_BIGENDIAN
#  define ntoh64(x) (x)
#  define hton64(x) (x)
#else /* !WORDS_BIGENDIAN */
uint64_t wslay_byteswap64(uint64_t x);
#  define ntoh64(x) wslay_byteswap64(x)
#  define hton64(x) wslay_byteswap64(x)
#endif /* !WORDS_BIGENDIAN */

#endif /* WSLAY_NET_H */
