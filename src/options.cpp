/*
    Copyright (c) 2007-2011 iMatix Corporation
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "options.hpp"
#include "err.hpp"

zmq::options_t::options_t () :
    sndhwm (1000),
    rcvhwm (1000),
    affinity (0),
    rate (100),
    recovery_ivl (10000),
    multicast_hops (1),
    sndbuf (0),
    rcvbuf (0),
    type (-1),
    linger (-1),
    reconnect_ivl (100),
    reconnect_ivl_max (0),
    backlog (100),
    maxmsgsize (-1),
    rcvtimeo (-1),
    sndtimeo (-1),
    immediate_connect (true),
    delay_on_close (true),
    delay_on_disconnect (true),
    filter (false)
{
}

int zmq::options_t::setsockopt (int option_, const void *optval_,
    size_t optvallen_)
{
    switch (option_) {

    case ZMQ_SNDHWM:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        sndhwm = *((int*) optval_);
        return 0;

    case ZMQ_RCVHWM:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        rcvhwm = *((int*) optval_);
        return 0;

    case ZMQ_AFFINITY:
        if (optvallen_ != sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        affinity = *((uint64_t*) optval_);
        return 0;

    case ZMQ_IDENTITY:

        //  Empty identity is invalid as well as identity longer than
        //  255 bytes. Identity starting with binary zero is invalid
        //  as these are used for auto-generated identities.
        if (optvallen_ < 1 || optvallen_ > 255 ||
              *((const unsigned char*) optval_) == 0) {
            errno = EINVAL;
            return -1;
        }
        identity.assign ((const unsigned char*) optval_, optvallen_);
        return 0;

    case ZMQ_RATE:
        if (optvallen_ != sizeof (int) || *((int*) optval_) <= 0) {
            errno = EINVAL;
            return -1;
        }
        rate = *((int*) optval_);
        return 0;
        
    case ZMQ_RECOVERY_IVL:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        recovery_ivl = *((int*) optval_);
        return 0;

    case ZMQ_SNDBUF:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        sndbuf = *((int*) optval_);
        return 0;

    case ZMQ_RCVBUF:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        rcvbuf = *((int*) optval_);
        return 0;

    case ZMQ_LINGER:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        linger = *((int*) optval_);
        return 0;

    case ZMQ_RECONNECT_IVL:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        if (*((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        reconnect_ivl = *((int*) optval_);
        return 0;

    case ZMQ_RECONNECT_IVL_MAX:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        if (*((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        reconnect_ivl_max = *((int*) optval_);
        return 0;

    case ZMQ_BACKLOG:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        backlog = *((int*) optval_);
        return 0;

    case ZMQ_MAXMSGSIZE:
        if (optvallen_ != sizeof (int64_t)) {
            errno = EINVAL;
            return -1;
        }
        maxmsgsize = *((int64_t*) optval_);
        return 0;

    case ZMQ_MULTICAST_HOPS:
        if (optvallen_ != sizeof (int) || *((int*) optval_) <= 0) {
            errno = EINVAL;
            return -1;
        }
        multicast_hops = *((int*) optval_);
        return 0;

    case ZMQ_RCVTIMEO:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        rcvtimeo = *((int*) optval_);
        return 0;

    case ZMQ_SNDTIMEO:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        sndtimeo = *((int*) optval_);
        return 0;

    }

    errno = EINVAL;
    return -1;
}

int zmq::options_t::getsockopt (int option_, void *optval_, size_t *optvallen_)
{
    switch (option_) {

    case ZMQ_SNDHWM:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndhwm;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_RCVHWM:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvhwm;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_AFFINITY:
        if (*optvallen_ < sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        *((uint64_t*) optval_) = affinity;
        *optvallen_ = sizeof (uint64_t);
        return 0;

    case ZMQ_IDENTITY:
        if (*optvallen_ < identity.size ()) {
            errno = EINVAL;
            return -1;
        }
        memcpy (optval_, identity.data (), identity.size ());
        *optvallen_ = identity.size ();
        return 0;

    case ZMQ_RATE:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rate;
        *optvallen_ = sizeof (int);
        return 0;
        
    case ZMQ_RECOVERY_IVL:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = recovery_ivl;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_SNDBUF:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndbuf;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_RCVBUF:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvbuf;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_TYPE:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = type;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_LINGER:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = linger;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_RECONNECT_IVL:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = reconnect_ivl;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_RECONNECT_IVL_MAX:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = reconnect_ivl_max;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_BACKLOG:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = backlog;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_MAXMSGSIZE:
        if (*optvallen_ < sizeof (int64_t)) {
            errno = EINVAL;
            return -1;
        }
        *((int64_t*) optval_) = maxmsgsize;
        *optvallen_ = sizeof (int64_t);
        return 0;

    case ZMQ_MULTICAST_HOPS:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = multicast_hops;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_RCVTIMEO:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvtimeo;
        *optvallen_ = sizeof (int);
        return 0;

    case ZMQ_SNDTIMEO:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndtimeo;
        *optvallen_ = sizeof (int);
        return 0;

    }

    errno = EINVAL;
    return -1;
}
