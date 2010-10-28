#!/bin/sh
############################################################################
#    This file is part of OpenRadio - RadioMixer                           #
#    Copyright (C) 2010 by Jan Boysen                                      #
#    trekkie@media-mission.de                                              #
#                                                                          #
#    This program is free software; you can redistribute it and/or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################

# This little Demo script can be used to publish the metadata from played tracks
# in RadioMixer to liquidsoap via its telnet command interface
# ( Assuming it runs on localhost to stream the jack audiodata to the streamingserver )
#
# This is a demo liquidsoap config snippet to enable that feature:
#
# stream = input.jack( id="stream" )
# stream = insert_metadata( id="meta",stream )
# output.icecast(%vorbis, host="stream.domain.tld",port=800,password="hackme",mount="live-stream", stream)
#

/bin/echo meta.insert artist=\"$1\",title=\"$2\" | /usr/bin/telnet localhost 1234 > /dev/null 2>&1

