/*
    Copyright (c) 2007-2010 iMatix Corporation

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

int main (int argc, char *argv [])
{
    if (argc != 4) {
        printf ("usage: remote_lat <connect-to> <message-size> "
            "<roundtrip-count>\n");
        return 1;
    }
    const char *connect_to = argv [1];
    size_t message_size = (size_t) atoi (argv [2]);
    int roundtrip_count = atoi (argv [3]);

    zmq::context_t ctx (1, 1);

    zmq::socket_t s (ctx, ZMQ_REQ);
    s.connect (connect_to);

    zmq::message_t msg (message_size);
    memset (msg.data (), 0, message_size);

    void *watch = zmq_stopwatch_start ();

    for (int i = 0; i != roundtrip_count; i++) {
        s.send (msg);
        s.recv (&msg);
        assert (msg.size () == message_size);
    }

    unsigned long elapsed = zmq_stopwatch_stop (watch);

    double latency = (double) elapsed / (roundtrip_count * 2);

    printf ("message size: %d [B]\n", (int) message_size);
    printf ("roundtrip count: %d\n", (int) roundtrip_count);
    printf ("mean latency: %.3f [us]\n", (double) latency);

    return 0;
}
