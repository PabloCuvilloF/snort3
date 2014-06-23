/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * Copyright (C) 2002-2013 Sourcefire, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
// pps_rpc_decode.cc author Josh Rosenbaum <jorosenba@cisco.com>

#include <sstream>
#include <vector>
#include <string>

#include "conversion_state.h"
#include "converter.h"
#include "snort2lua_util.h"

namespace {

class RpcDecode : public ConversionState
{
public:
    RpcDecode(Converter* cv)  : ConversionState(cv) {};
    virtual ~RpcDecode() {};
    virtual bool convert(std::stringstream& data_stream);
};

} // namespace

bool RpcDecode::convert(std::stringstream& data_stream)
{

    bool retval = true;
    std::string port_list = "";
    std::string keyword;
    int i_val;
    bool blah;

    cv->open_table("rpc_decode");

    // parse the port list first.
    while(data_stream >> i_val)
        port_list += ' ' + std::to_string(i_val);

    util::ltrim(port_list);
    cv->add_option_to_table("--ports", port_list);
    data_stream.clear();  // necessary since badbit() is set
    
    while(data_stream >> keyword)
    {
        bool tmpval = true;

        
        if(!keyword.compare("no_alert_multiple_requests"))
            cv->add_deprecated_comment("no_alert_multiple_requests");

        else if(!keyword.compare("alert_fragments"))
            cv->add_deprecated_comment("alert_fragments");

        else if(!keyword.compare("no_alert_large_fragments"))
            cv->add_deprecated_comment("no_alert_large_fragments");

        else if(!keyword.compare("no_alert_incomplete"))
            cv->add_deprecated_comment("no_alert_incomplete");

        else
            tmpval = false;

        if (retval)
            retval = tmpval;
    }

    return retval;   
}

/**************************
 *******  A P I ***********
 **************************/

static ConversionState* ctor(Converter* cv)
{
    return new RpcDecode(cv);
}

static const ConvertMap preprocessor_rpc_decode =
{
    "rpc_decode",
    ctor,
};

const ConvertMap* rpc_decode_map = &preprocessor_rpc_decode;
