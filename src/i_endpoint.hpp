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

#ifndef __ZMQ_I_ENDPOINT_HPP_INCLUDED__
#define __ZMQ_I_ENDPOINT_HPP_INCLUDED__

#include "blob.hpp"

namespace zmq
{

    struct i_endpoint
    {
        virtual ~i_endpoint () {}

        virtual void attach_pipes (class reader_t *inpipe_,
            class writer_t *outpipe_, const blob_t &peer_identity_) = 0;
        virtual void detach_inpipe (class reader_t *pipe_) = 0;
        virtual void detach_outpipe (class writer_t *pipe_) = 0;
        virtual void kill (class reader_t *pipe_) = 0;
        virtual void revive (class reader_t *pipe_) = 0;
    };

}

#endif
